
#include <sstream>
#include <immintrin.h>

using namespace std;

//Performance Tuning		https://www.youtube.com/watch?v=fV6qYho-XVs&t=1379s
namespace godbolt_perf_tuning {


	string newOrder(string stock, int price, int quantity)
	{
		stringstream s;
		s << "New " << stock << " " << price << " " << quantity;
		return s.str();
	}//550ns per order (on 3.5GHz Haswell)

	// perf record ./app 
	// perf report

#pragma warning(disable: 4996)
	void newOrder(char* buf, const char* stock, int price, int quantity)
	{
		sprintf(buf, "New %s %d %d", stock, price, quantity);
	}// 130 ns per order	4x faster



	class Format {
		char _buffer[2048];
		int  _ptr;
	public:
		Format() : _ptr(0) {}

		void append(char c) { _buffer[_ptr++] = c; }

		void append(const char* str) {
			while (*str)
				_buffer[_ptr++] = *str++;
		}

		void decimalAppendNonNeg(unsigned value) {
			int startPos = _ptr;
			do {
				append((char)(value % 10) + '0');
				value /= 10;
			} while (value);
			// Reverse the digits.
			auto end = &_buffer[_ptr - 1];
			auto start = &_buffer[startPos];
			while (end > start) swap(*start++, *end--);
		}

		void decimalAppend(int value) {
			if (value < 0) {
				append('-');
				value = -value;
			}
			decimalAppendNonNeg(value);
		}

		const char* c_str() {
			_buffer[_ptr] = 0;
			return _buffer;
		}

		void test(Format& f, int value) {
			f.decimalAppendNonNeg(value);
		}
		void finish() { append('\x00'); }
	};

	void newOrder(Format& format, const char* id, int price, int quantity) {
		format.append("NEW ");
		format.append(id);
		format.append(' ');
		format.decimalAppend(price);
		format.append(' ');
		format.decimalAppendNonNeg(unsigned(quantity));
		format.finish();
	}


	// handwritten algo results:
	// 20ns per order		27x faster than stringstream
	// 6.5x faster than sprintf

	//------
	//https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwskEF8LAhuwcADAEERghcFUsmAW0yCADkwmrd6DX3OmzBAE9nTCwAM1URBQIANgAWAH0CVQJdB2dE7z8/VII8ZAiWK2AWENU0QqSABVQAd0xiQQB5UNoTDgBWPnaAEXVZXqyAdh56VVbSMZMTCdapmam5yYXpv1U1peX55cXZ7emNvY5B7u9VAHoz1UwCTl9zHLyCopL0WxF7brxgQkEIEUKvi9VAA3ACU6mGq1UTBERFUDFQwC4fV6sloqgAtKp4vEAEaRBi5FjxJQALwgYMy5jWMLhCOArTMzmcxFQAA8UaoIPTkfwxuCAFRjLiyWR9YyybDCqlmGmw1DwxGtTn0xnM1kcrEUvqaVTVOoNZqtDo8VXmdXsjrdUEytbEa5iFiKhkmdS8MYyo4nO5mJRMQSCVQAMRI9iYSSOPll62QCCYxGx%2BNCoXqJq4JliAA4rbb1g98pEDLRoolsQjUABrETOE2zHOyKPrVRRbHOAjEGXOES4hh5EBQtb5kGoPCvBSECDgyMDpuqUIkLktvCctoN5s6panfj8PBTyHU2eH2kK/AMzlgKQmC9uniLt3tSY2hszw9yuGn5Hny/XvlQPBnVopy4B9ANzV91nicsqxrTo8CtTkPwhXVT3RDRNDQ1Rs2fA9Dy9Gc8Jw4NQ3DSdVBAVt2wgEw9z4Y4ZwUXsSmHUdVHtQRrlIyMKITDRelXW8COjNZyl0Mo42IIVkHiXRiFIkSI2GViHWIJ08REZNUzXQSm2BEdXiYZlMBYdAIFjeMyhoxN1JTDtOniNsO14bcek5TgtLowjdJY0Jx0EBBOMUgzgmMiALw6TQ2SmC8nwEjyzBnLz9MMkL5LE8yBXQcMmBol91hqBA8AYTAuQyrLwSCoyTNKggmG3XgYvwuKZwITB7GcZgWrQlsADkjHolhGOKxLVCwZA8DDBhKiYdAmWC9AuD%2BAFilKYEmGUTAcsIps8HCCBuvFXjVC4TahPAucFwgJdOX2rEuFOZc%2Bv6VR%2BPXDFDuOiFGzOo95WSGpUE%2Bb4Uk5Vb1vvTdsNO761lBkRirOQ660h6HZwFe0onqFlrhJf1DC0QsCGLUsBSMCA1I0nj3Xs9sb2bKcnsg1BK2rE0CH%2BwGfnrL7vu019ecPHauX27hon5KyKZNanbIGJ7YeK7gQNdPkLyvGRkdfKWb0O3r1bWQSEr0kbMDGibZsq7rUBYAAtepUEW54VrWuGTvA48Rq%2BH5ORYd4OZSCknY2z0RTCBRis0MwAGVsAgAANGPwWQf1irj8jRvGtappm5L5rQuPSblmLVFxe0mArXLBxqQhYy5U8fhd6Hw6jiBaAarbX0b6PjrAs6O4gWRW6h9vI%2Bj2IB5R3v2jHhvh4gaIp%2B%2B3vBnnnuZ8zZfwN7gBOdeh6b0DddnMIYUJfs28PHF8UKol4n%2BEuzJ7TBJ278Di8wUvn/WfmIW9eLPMNtPTbZ0ugYEEAd64C12nLFEh1qKfXLrOCqIUVbRQ/oee0BBHSoO/lcBg7FmyQIDhuWB04z4IKAReDEKCD7gSgYdDEcssFfybAAtaZtjIW2trbf261l76zopDPww0Sg1EaMQLAskQzEDDBGLg0R5xSPDBMVKZkEwCl0KgZAFYJgthZHkTA2iQEAEcRCsFyIEcB50FEEAAHSIJMtwLg3VsAAHU3RdwPvI6RtigHqM0cvTx4ZvFzVClIVQVDuaWK8SwhgbCTK6IkP44iNi7EhLCVIRJVjrHRNiRwm2rIIDGNMYQAIGSvE%2BRYHgPyT8BFxXSYwaQ7QpCkBYNIEwTTUDSHQk5d0wgxDuDkLQJpBBWnpNBKQCsIB2jTAYNIWITSWmXnadIJpggQDTGGYssZcBYBIDQG1Qq9RyCUD2c4A5xAQDAHaOmUgPlCT1FWRAXEIzSC4gUPGAI0hBmkD2Y4AwjQBofMWaQLAYY2BFWefge0yBcjAicM8zAbJjawkkFIL5GMZlAvbONT5dTmBsBQHVAQvZcSrMgGM1AbY8CW1WVIDEjQxQYk8LxTg3SRixExGGcQCBeJxkEHUBgDAVmiHEJIFu9SpCNOac8jpUg2SZmiBiOIqhgDIHyO0axyIIC4EIAuAZExNCoH2UVSmaJwRdL4LwIZIzQRjImVM8VcypVAplSstZpANltJteKrg8zpXLPddasZsKGhUpabEIAA%3D

	using namespace std;

	//typedef uint64_t tstamp_t;

	static unsigned const PowersOf102[] =
	{1, 10, 100, 1000, 10000, 100000,
		1000000, 10000000, 100000000, 1000000000}; // etc

	static unsigned numDigits(unsigned v) {
		auto log2 = 31 - _lzcnt_u32(v); // Windows msvc
		//auto log2 = 31 - __builtin_clz(v);  // Linux
		auto log10Approx = (log2 + 1) * 1233 >> 12;
		auto log10 = log10Approx - (v < PowersOf102[log10Approx]);
		return log10 + 1;
	}

	class Format2 {
		char _buffer[2048];
		inline static uint16_t _lookup[100];
		int _ptr;
	public:
		static void init() {
			for (int i = 0; i < 100; ++i) {
				auto dig1 = '0' + (i % 10);
				auto dig2 = '0' + ((i / 10) % 10);
				_lookup[i] = dig2 | dig1 << 8;
			}
		}
		Format2() : _ptr(0) {}
		inline void reset() { _ptr = 0; }
		const char* c_str() const { return _buffer; }
		void append(char c) { _buffer[_ptr++] = c; }
		void finish() { append('\x00'); }

		void append(const char* data) {
			while (*data) append(*data++);
		}

		template<int n>
		inline void decimalPadAppend2(unsigned value) {
			if (n >= 2) {
				for (int i = n - 2; i >= 0; i -= 2) {
					auto twoDigits = value % 100;
					value /= 100;
					*reinterpret_cast<uint16_t*>(_buffer + _ptr + i) = _lookup[twoDigits];
				}
			}
			if (n & 1) _buffer[_ptr] = value % 10 + '0';
			_ptr += n;
		}

		void decimalAppendNonZero(unsigned value) {
			auto digits = numDigits(value);
#define CASE(XX) case XX: decimalPadAppend2<XX>(value); break
			switch (digits) {
				CASE(1);
				CASE(2);
				CASE(3);
				CASE(4);
				CASE(5);
				CASE(6);
				CASE(7);
				CASE(8);
				CASE(9);
				CASE(10);
			default:
				//__builtin_unreachable();
				break;
			}
		}

		void decimalAppend(int value) {
			if (value == 0) {
				append('0');
				return;
			}
			else if (value < 0) {
				append('-');
				value = -value;
			}
			decimalAppendNonZero(value);
		}
	};

	void newOrder(Format2& format, const char* stock, int price, int quantity) {
		format.append("NEW ");
		format.append(stock);
		format.append(' ');
		format.decimalAppend(price);
		format.append(' ');
		format.decimalAppendNonZero(quantity);
		format.finish();
	}












	static unsigned int PowersOf10[] =
	{1,
	 10,
	 100,
	 1000,
	 10000,
	 100000,
	 1000000,
	 10000000,
	 100000000,
	 1000000000};

    static unsigned numOfDigits(unsigned v) {
        unsigned log2 = 31 - _lzcnt_u32(v);
        unsigned log10Approx = (log2 + 1) * 1233 >> 12;
        unsigned log10 = log10Approx - (v < PowersOf10[log10Approx]);
        return log10 + 1;
    }


	bool test = []() {
		
		int x = 10;
		auto a = _lzcnt_u32(x);
		//auto a = __builtin_clz(x);//__builtin_clz


		auto digits = numOfDigits(10);

		
		
		
		return true;
	}();



	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqa3F5ckphYkg3V0ZaaWFNVzJ3RUhuMmtvd2NWd3xBQ3Jtc0trX3ItSkltUUZuNjM0VDVBMC1rN2lpM2FSTGFpY0xnSFBGYmdZSmJDVGwwZTNJOXBEVXFmLTcyZFJNeUdSMGJnZ3IzTlpFQTBqY0JEOWxjSnpyWEt4Smh1RWJCTmtJOXpzZnJhYjVVVUJiQ210bFFtaw&q=https%3A%2F%2Fgodbolt.org%2Fg%2FCa47rA&v=fV6qYho-XVs
	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbUEzSjFySTFtMEFvSGUwSDVYRGNQUzNiWHJUUXxBQ3Jtc0tsMUYwVld4U3B1OWxqYk5TUEszektXTWkxT1Y0eEg5NGUtSWxjRVI2MldicDdGMEd6Y1VhX3pRclFkWUtyY1Z3NF83elk3c0k2UnZyRGhqZFJES1RIRzBuVWNSTVAySU5tNTVGTWltN1JWQnJmd2d6NA&q=https%3A%2F%2Fgodbolt.org%2Fg%2FGaVAyF&v=fV6qYho-XVs
	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbHZDUEJudldQTlg4SUxxRm55RFUtWTI1WWJQQXxBQ3Jtc0tsZnVURzZlNEtueEtmajhKT1NSZHA5V1ZUaHUyclh5Uko0M0pIY3JUMUpfZ0d0aFdPUFcxWnBhWTFEX1hTZFNOTjJBa3h4V2NFVjZCWTBvRFJJSXFNY0phLXpONkgtM2t1aXlxclpkeUVoYXdSZDFIYw&q=https%3A%2F%2Fgodbolt.org%2Fg%2FGfsr2m&v=fV6qYho-XVs




















}