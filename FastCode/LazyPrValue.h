#pragma once
#include <list>
#include <mutex>
#include <typeindex>

namespace utils {
	// [Антон Полухин — C++ трюки из userver] https://youtu.be/FcQC19CX-AY?t=574
	template<class Func>
	class LazyPrValue final {
	public:
		constexpr explicit LazyPrValue(Func&& func) : func_(std::move(func)) {}

		LazyPrValue(LazyPrValue&&) = delete;
		LazyPrValue& operator=(LazyPrValue&&) = delete;

		constexpr operator std::invoke_result_t<Func&&>()&& {
			return std::move(func_)();
		}
	private:
		Func func_;
	};

	namespace LazyPrValue_usage_examples {

		class Something {
		public:
			static Something Create(std::size_t value) {
				//...
				return Something(value);
			}
			//...
		private:
			Something(size_t value) {};
			std::mutex mutex_;
		};

		void Usage_Example(std::list<Something>& clients) {
			for (size_t i = 0; i < 10; ++i) {
				clients.emplace_back(utils::LazyPrValue([i]() { return Something::Create(i); }));
			}
		}
	}

	//----------------------------------------------------------------------
	//https://youtu.be/FcQC19CX-AY?t=1128
	/*
	template <typename T>
	typename traits::IO<T>::FormatterType BufferWrite(const T& value) {
		using Formatter = typename traits::IO<T>::FormatterType;
		return Formatter(value);
	}
	// above code will sometimes cause problems with ODR.

	template <typename T>
	typename traits::IO<T>::FormatterType BufferWrite(const T& value) {
		using Formatter = typename traits::IO<T>::FormatterType;
		#ifndef NDEBUG
		detail::l <T,Formatter>::content.RequireInstance();
		#endif
		return Formatter(value);
	}

	


	*/

#ifndef NDEBUG
	class WriterRegistrator final {
	public:
		WriterRegistrator(std::type_index type, std::type_index formatter_type,
						  const char* base_file);
		// ctor has unordered_map from (type, formatter_type) => base_file

		void RequireInstance() const;
		// in first call of RequireInstance we will try to see if we have 
		// duplicates or not and can give error message to user

	};


	namespace {
		template<class Type,class Writer>
		struct CheckForBufferWriterODR final {
			static inline WriterRegistrator content{typeid(type), typeid(Writer),
			__BASE_FILE__};
		};
	}
#endif // !NDEBUG

	/// llvm::StringSwitch class.


}