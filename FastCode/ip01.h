#pragma once
#include <immintrin.h>
#include <cstdint>

//C++ SSE Optimization - Lesson 1   https://www.youtube.com/watch?v=GwII1AJzKN4
//C++ SSE Optimization - Lesson 2	https://www.youtube.com/watch?v=M67gsdzpVME

namespace image_processing {
	int* v;
	const int N = 128 * 4;

	
	/*
	class Color {
		union
		{
			D3DCOLOR c;
			struct {
				unsigned char b;
				unsigned char g;
				unsigned char r;
				unsigned char x;
			};
		};

	};

	void FadeShift(unsigned char a) {
		const unsigned int alpha = a;
		const unsigned int mask = 0xFF;

		for (Color* i = buffer, *end = &buffer[pixelPitch * height];
			i < end; i++)
		{
			const Color src = *i;
			const unsigned int r = (((src >> 16) & mask) * a) / 255;// replace 255 with 256 (will have very small error but ~2x faster)
			const unsigned int g = (((src >> 8) & mask) * a) / 255;
			const unsigned int b = (((src)&mask) * a) / 255;

			*i = (r << 16) | (g << 8) | b;
		}
	}

	*/


	// c1*alpha / 256
	void FadeSSE(unsigned char a)
	{
		using reg = __m128i;
		const __m128i alpha = _mm_set1_epi16(a);
		const __m128i zero = _mm_setzero_si128();

		for (__m128i* i = reinterpret_cast<reg*>(v),
			*end = reinterpret_cast<reg*>(v + N);
			i < end; ++i)
		{
			const reg srcPixel = _mm_load_si128(i);//argb argb argb argb
			// we need to expand 8 bit to 16 bit to be albe to multiply

			//www.officedaytime.com/tips/simding/punpcklbw.png
			const reg srcLo16 = _mm_unpacklo_epi8(srcPixel, zero);// interleave lower 64-bit's. | 0a0r0g0b 0a0r0g0b
			const reg srcHi16 = _mm_unpackhi_epi8(srcPixel, zero);

			reg rsltLo16 = _mm_mullo_epi16(srcLo16, alpha);// will multiply and extract lower 16 bit's
			reg rsltHi16 = _mm_mullo_epi16(srcHi16, alpha);

			// shift right by 8-bit (every 16-bit)
			// from XXRRGGBB XXRRGGBB => XRGBXRGB XRGBXRGB
			rsltLo16 = _mm_srli_epi16(rsltLo16, 8);
			rsltHi16 = _mm_srli_epi16(rsltHi16, 8);

			// combine lower 8-bits of every 16-bit number
			// XRGBXRGB XRGBXRGB (every symbol is 16-bit) => XRGBXRGB XRGBXRGB (every symbol is 8_bit)
			// then combine  rsltLo16 and rsltLo16
			// then store into i
			_mm_store_si128(i, _mm_packus_epi16(rsltLo16, rsltHi16));
		}
	}
		// c1*alpha / 256
		// if alpha is 128 then we can just devide by 2 (same as shift right by one)
	void FadeHalfSSE()
	{
		using reg = __m128i;
		const reg  zero = _mm_setzero_si128();

		for (reg* i = reinterpret_cast<reg*>(v),
			*end = reinterpret_cast<reg*>(v + N);
			i < end; i++)
		{
			const reg srcPixels = _mm_load_si128(i);
			const reg srcLo16 = _mm_unpacklo_epi8(srcPixels, zero);
			const reg srcHi16 = _mm_unpackhi_epi8(srcPixels, zero);

			const reg rsltLo16 = _mm_srli_epi16(srcLo16, 1); 
			const reg rsltHi16 = _mm_srli_epi16(srcHi16, 1); 

			_mm_store_si128(i, _mm_packus_epi16(rsltLo16, rsltHi16));
		}// 20% better than above function if alpha is 1
	}

	void FadeHalfPackedSSE()
	{
		using reg = __m128i;
		const reg  shiftMask = _mm_set1_epi8(0x7F);

		for (reg* i = reinterpret_cast<reg*>(v),
			*end = reinterpret_cast<reg*>(v + N);
			i < end; i++)
		{
			const reg srcPixels = _mm_load_si128(i);
			reg rslt = _mm_srli_epi16(srcPixels, 1);
			rslt = _mm_and_si128(rslt, shiftMask);
			_mm_store_si128(i, rslt);
		}// 40% faster

	}
	void FadeHalfAvgSSE() {
		using reg = __m128i;
		const reg  zero = _mm_setzero_si128();

		for (reg* i = reinterpret_cast<reg*>(v),
			*end = reinterpret_cast<reg*>(v + N);
			i < end; i++)
		{
			const reg srcPixels = _mm_load_si128(i);
			_mm_store_si128(i, _mm_avg_epu8(srcPixels, zero));
		}// 40% faster
	}


	//C++ SSE Optimization [Shuffle/Instruction Pipeline] Lesson 3
	/*
	
	void Tint(Color c)
	{
		for (Color* c =buffer,*end=&buffer[pixelPitch*height]; i < end;i++)
		{
		const Color d = *i;

		// blend channels
		const unsigned char red = (c.r * c.x + d.r * (255 - c.x))/256;
		const unsigned char red = (c.g * c.x + d.g * (255 - c.x))/256;
		const unsigned char red = (c.b * c.x + d.b * (255 - c.x))/256;
		
		*i = {r,g,b};
		}
	}
		
	*/
	void TintSSE(uint32_t c)
	{
		using reg = __m128i;
		const reg zero = _mm_setzero_si128();
		const reg ones = _mm_set1_epi16(0x00FF);
		const __m128i color = _mm_set1_epi32(c);

		for (reg* i = reinterpret_cast<reg*>(v),
			*end = reinterpret_cast<reg*>(v + N);
			i < end; i++)
		{
			const reg dstPixels = _mm_load_si128(i);
			const reg dstLo16 = _mm_unpacklo_epi8(dstPixels, zero);
			const reg dstHi16 = _mm_unpackhi_epi8(dstPixels, zero);
			const reg color16 = _mm_unpackhi_epi8(color, zero);

			reg alpha = _mm_shufflelo_epi16(color16, _MM_SHUFFLE(3, 3, 3, 3));
			alpha = _mm_shufflelo_epi16(alpha, _MM_SHUFFLE(3, 3, 3, 3));

			const reg calpha = _mm_sub_epi16(ones, alpha);
			const reg mdstLo16 = _mm_mullo_epi16(dstLo16, calpha);
			const reg mdstHi16 = _mm_mullo_epi16(dstHi16, calpha);
			const reg mcolor16 = _mm_mullo_epi16(color16, alpha);
			reg rsltLo16 = _mm_add_epi16(mdstLo16, mcolor16);
			reg rsltHi16 = _mm_add_epi16(mdstHi16, mcolor16);
			rsltLo16 = _mm_srli_epi16(rsltLo16, 8);
			rsltHi16 = _mm_srli_epi16(rsltHi16, 8);
			_mm_store_si128(i, _mm_packus_epi16(rsltLo16, rsltHi16));
		}
	}



}