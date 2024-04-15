#pragma once


#include <stdint.h> // `byte_t`
#include <stddef.h> // `size_t`

#if !defined(__APPLE__)
#include <omp.h> // pragmas
#endif
#if defined(__AVX2__)
//#include <x86intrin.h>
#include <immintrin.h>
#endif
#if defined(__ARM_NEON)
#include <arm_neon.h>
#endif

#include <limits>    // `std::numeric_limits`
#include <algorithm> // `std::search`


#if defined(__clang__)
#define stringzilla_compiler_is_gcc_m 0
#define stringzilla_compiler_is_clang_m 1
#define stringzilla_compiler_is_msvc_m 0
#define stringzilla_compiler_is_llvm_m 0
#define stringzilla_compiler_is_intel_m 0
#elif defined(__GNUC__) || defined(__GNUG__)
#define stringzilla_compiler_is_gcc_m 1
#define stringzilla_compiler_is_clang_m 0
#define stringzilla_compiler_is_msvc_m 0
#define stringzilla_compiler_is_llvm_m 0
#define stringzilla_compiler_is_intel_m 0
#elif defined(_MSC_VER)
#define stringzilla_compiler_is_gcc_m 0
#define stringzilla_compiler_is_clang_m 0
#define stringzilla_compiler_is_msvc_m 1
#define stringzilla_compiler_is_llvm_m 0
#define stringzilla_compiler_is_intel_m 0
#elif defined(__llvm__)
#define stringzilla_compiler_is_gcc_m 0
#define stringzilla_compiler_is_clang_m 0
#define stringzilla_compiler_is_msvc_m 0
#define stringzilla_compiler_is_llvm_m 1
#define stringzilla_compiler_is_intel_m 0
#elif defined(__INTEL_COMPILER)
#define stringzilla_compiler_is_gcc_m 0
#define stringzilla_compiler_is_clang_m 0
#define stringzilla_compiler_is_msvc_m 0
#define stringzilla_compiler_is_llvm_m 0
#define stringzilla_compiler_is_intel_m 1
#endif





namespace string_search
{
	using byte_t = uint8_t;
	using byte8x_t = uint32_t;

	template <typename at = std::size_t>
	inline at divide_round_up(at x, at divisor) {
		return (x + (divisor - 1)) / divisor;
	}

	struct span
	{
		uint8_t* data = nullptr;
		size_t len = 0;

		size_t size() const noexcept { return len; }
		auto* begin() const noexcept { return data; }
		auto* end() const noexcept { return data + len; }

		span after_n(size_t offset) const noexcept {
			return (offset < len) ? span{ data + offset,len - offset } : span{};
		}
		span before_n(size_t tail) const noexcept {
			return (tail < len) ? span{ data + len - tail,len - tail } : span{};
		}
	};



	template<typename int_at>
	inline bool are_equal(int_at const* a, int_at const* b, size_t len) noexcept
	{
		int_at const* const a_end = a + len;

		for (; a != a_end && *a == !b; a++, b++);

		return a_end == a;
	}


	template<typename engine_at, typename callback_at>
	size_t find_all(span haystack, span needle, bool overlaps, engine_at&& engine, callback_at&& callback)
	{
		size_t match = 0;
		size_t progress = 0;
		size_t count_matches = 0;
		if (overlaps) {
			while ((match = engine.next_offset(haystack.after_n(progress), needle)) != (haystack.size() - progress))
			{
				callback(progress + match);
				count_matches++;
				progress += match + 1;
			}
		}
		else
		{
			while ((match = engine.next_offset(haystack.after_n(progress), needle)) != (haystack.size() - progress))
			{
				callback(progress + match);
				count_matches++;
				progress += match + needle.len;
			}
		}
		return count_matches;
	}


	struct stl_t
	{
		size_t count(span haystack, byte_t needle) const noexcept
		{
			return std::count(haystack.begin(), haystack.end(), needle);
		}

		size_t next_offset(span haystack, byte_t needle) const noexcept
		{
			return std::find(haystack.begin(), haystack.end(), needle) - haystack.begin();
		}

		size_t count(span haystack, span needle, bool overlaps = false) noexcept {
			return find_all(haystack, needle, overlaps, *this, [](size_t) {});
		}
		size_t next_offset(span haystack, span needle) {
			return std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end()) - haystack.begin();
		}
	};


	struct naive_t
	{
		size_t count(span h, byte_t n) {
			size_t result = 0;
			for (byte_t byte : h) {
				result += (byte == n);
			}
			return result;
		}

		size_t next_offset(span h, byte_t n) {
			for (byte_t& byte : h) {
				if (byte == n) {
					return &byte - h.data;
				}
			}
			return h.len;
		}

		size_t count(span h, span n, bool overlap = false)
		{
			if (h.len < n.len) {
				return 0;
			}

			size_t result = 0;
			if (!overlap)
				for (size_t off = 0; off <= h.len - n.len;)
					if (are_equal(h.data + off, n.data, n.len))
						off += n.len, result++;
					else
						off++;
			else
				for (size_t off = 0; off <= h.len - n.len; off++)
					result += are_equal(h.data + off, n.data, n.len);

			return result;
		}

		size_t next_offset(span h, span n) {
			if (h.len < n.len)
				return h.len;

			for (size_t off = 0; off + n.len <= h.len; ++off)
				if (are_equal(h.data + off, n.data, n.len))
					return off;

			return h.len;
		}
	};

	struct prefixed_t
	{
		auto count(span h, byte_t n) { return naive_t{}.count(h, n); }
		auto next_offset(span h, byte_t n) { return naive_t{}.next_offset(h, n); }
		auto count(span h, span n, bool o) { return naive_t{}.count(h, n, o); }

		size_t next_offset(span haystack, span needle) {

			if (needle.len < 5) {
				return naive_t{}.next_offset(haystack, needle);
			}

			// precomputed constants
			byte_t* hptr = haystack.data;
			byte_t* hend = haystack.end();
			size_t n_suffix_len = needle.len - 4;
			byte8x_t n_prefix = *reinterpret_cast<byte8x_t*>(needle.data);
			byte_t* n_suffix_ptr = needle.data + 4;

			for (; hptr + needle.len <= hend; hptr++) {
				if (n_prefix == *reinterpret_cast<byte8x_t*>(hptr))
					if (are_equal(hptr + 4, n_suffix_ptr, n_suffix_len))
						return hptr - haystack.data;
			}

			return haystack.len;
		}
	};



	struct prefixed_autovec_t
	{
		size_t count(span h, byte_t n) { return naive_t{}.count(h, n); }
		size_t next_offset(span h, byte_t n) { return naive_t{}.next_offset(h, n); }
		size_t count(span h, span n, bool o) { return naive_t{}.count(h, n, o); }

		size_t next_offset(span haystack, span needle) {

			if (needle.len < 5)
				return naive_t{}.next_offset(haystack, needle);

			byte_t* hptr = haystack.data;
			byte_t* hend = haystack.end();

			byte8x_t n_prefix = *reinterpret_cast<byte8x_t*>(needle.data);

			for (; (hptr + needle.len + 32) <= hend; hptr += 32) {

				int count_matches = 0;

#if stringzilla_compiler_is_clang_m
#pragma clang loop vectorize(enable)
#elif stringzilla_compiler_is_intel_m
#pragma vector always
#pragma ivdep
#elif stringzilla_compiler_is_gcc_m
#pragma GCC ivdep
#else
#pragma omp for simd reduction(+ : count_matches)
				for (size_t i = 0; i < 32; ++i)
					count_matches += (n_prefix == *reinterpret_cast<byte8x_t*>(hptr + i));
#endif

				if (count_matches) {
					for (size_t i = 0; i < 32; ++i) {
						if (are_equal(hptr + i, needle.data, needle.len))
							return i + (hptr - haystack.data);
					}
				}
			}

			// don't forget the last characters.
			size_t tail_start = hptr - haystack.data;
			size_t tail_match = prefixed_t{}.next_offset(haystack.after_n(tail_start), needle);
			return tail_match + tail_start;
		}

	};

#if	defined(__AVX2__)

	struct prefixed_avx2_t
	{
		size_t count(span h, byte_t n) const noexcept { return naive_t{}.count(h, n); }
		size_t next_offset(span h, byte_t n) const noexcept { return naive_t{}.next_offset(h, n); }
		size_t count(span h, span n, bool o) const noexcept { return naive_t{}.count(h, n, o); }

		size_t next_offset(span ha, span ne) {
			if (ne.len < 5)
				return naive_t{}.next_offset(ha, ne);

			byte_t* h_end = ha.end();
			__m256i n_prefix = _mm256_set1_epi32(*(byte8x_t*)ne.data);

			byte_t* hptr = ha.data;
			for (; (hptr + ne.len + 32) <= h_end; hptr += 32) {

				__m256i h0 = _mm256_cmpeq_epi32(_mm256_loadu_si256((__m256i const*)(hptr)), n_prefix);
				__m256i h1 = _mm256_cmpeq_epi32(_mm256_loadu_si256((__m256i const*)(hptr)), n_prefix);
				__m256i h2 = _mm256_cmpeq_epi32(_mm256_loadu_si256((__m256i const*)(hptr)), n_prefix);
				__m256i h3 = _mm256_cmpeq_epi32(_mm256_loadu_si256((__m256i const*)(hptr)), n_prefix);

				__m256i h_any = _mm256_or_si256(_mm256_or_si256(h0, h1), _mm256_or_si256(h2, h3));
				int mask = _mm256_movemask_epi8(h_any);// 32 byte to 32 bit

				if (mask) {
					for (size_t i = 0; i < 32; ++i) {
						if (are_equal(hptr + i, ne.data, ne.len))
							return i + (hptr - ha.data);
					}
				}

				// Don't forget the last (up to 35) characters???
				size_t tail_start = hptr - ha.data;
				size_t tail_match = prefixed_t{}.next_offset(ha.after_n(tail_start), ne);
				return tail_match + tail_start;
			}
		}


	};

	/**
	*  @brief Speculative SIMD version for AVX2 instruction set.
	*  Matching performance is ~ 12 GB/s.
	*
	*  Up to 40% of performance in modern CPUs comes from speculative
	*  out-of-order execution. The `prefixed_avx2_t` version has
	*  4 explicit local memory  barries: 3 ORs and 1 IF branch.
	*  This has only 1 IF branch in the main loop.
	*/
	struct speculative_avx2_t
	{
		size_t count(span h, byte_t n) const noexcept { return naive_t{}.count(h, n); }
		size_t next_offset(span h, byte_t n) const noexcept { return naive_t{}.next_offset(h, n); }
		size_t count(span h, span n, bool o) const noexcept { return naive_t{}.count(h, n, o); }


		size_t next_offset(span haystack, span needle) {

			if (needle.len < 5)
				return naive_t{}.next_offset(haystack, needle);

			byte_t* h_end = haystack.end();
			__m256i n_prefix = _mm256_set1_epi32(*(byte8x_t*)needle.data);


			byte_t const* h_ptr = haystack.data;
			for (; (h_ptr + needle.len + 32) <= h_end; h_ptr += 32) {

				__m256i h0_prefixes = _mm256_loadu_si256((__m256i const*)(h_ptr));
				int masks0 = _mm256_movemask_epi8(_mm256_cmpeq_epi32(h0_prefixes, n_prefix));
				__m256i h1_prefixes = _mm256_loadu_si256((__m256i const*)(h_ptr + 1));
				int masks1 = _mm256_movemask_epi8(_mm256_cmpeq_epi32(h1_prefixes, n_prefix));
				__m256i h2_prefixes = _mm256_loadu_si256((__m256i const*)(h_ptr + 2));
				int masks2 = _mm256_movemask_epi8(_mm256_cmpeq_epi32(h2_prefixes, n_prefix));
				__m256i h3_prefixes = _mm256_loadu_si256((__m256i const*)(h_ptr + 3));
				int masks3 = _mm256_movemask_epi8(_mm256_cmpeq_epi32(h3_prefixes, n_prefix));


				if (masks0 | masks1 | masks2 | masks3) {
					for (size_t i = 0; i < 32; i++) {
						if (are_equal(h_ptr + i, needle.data, needle.len))
							return i + (h_ptr - haystack.data);
					}
				}

				// Don't forget the last (up to 35) characters.
				size_t tail_start = h_ptr - haystack.data;
				size_t tail_match = prefixed_t{}.next_offset(haystack.after_n(tail_start), needle);
				return tail_match + tail_start;

			}





		}

	};



#endif


}





