//Performance Tuning		https://www.youtube.com/watch?v=fV6qYho-XVs&t=1379s

#include <sstream>

using namespace std;

namespace godbolt_perf_tuning {


	string newOrder(string stock, int price, int quantity)
	{
		stringstream s;
		s << "New " << stock << " " << price << " " << quantity;
		return s.str();
	}//550ns per order (on 3.5GHz Haswell)

	// perf record ./app 
	// perf report

	void newOrder(char* buf, const char* stock, int price, int quantity)
	{
		//sprintf(buf, "New %s %d %d", stock, price, quantity);
	}// 130 ns per order	4x faster


	// handwritten algo results:
	// 20ns per order		27x faster than stringstream
	// 6.5x faster than sprintf



	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqa3F5ckphYkg3V0ZaaWFNVzJ3RUhuMmtvd2NWd3xBQ3Jtc0trX3ItSkltUUZuNjM0VDVBMC1rN2lpM2FSTGFpY0xnSFBGYmdZSmJDVGwwZTNJOXBEVXFmLTcyZFJNeUdSMGJnZ3IzTlpFQTBqY0JEOWxjSnpyWEt4Smh1RWJCTmtJOXpzZnJhYjVVVUJiQ210bFFtaw&q=https%3A%2F%2Fgodbolt.org%2Fg%2FCa47rA&v=fV6qYho-XVs
	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbUEzSjFySTFtMEFvSGUwSDVYRGNQUzNiWHJUUXxBQ3Jtc0tsMUYwVld4U3B1OWxqYk5TUEszektXTWkxT1Y0eEg5NGUtSWxjRVI2MldicDdGMEd6Y1VhX3pRclFkWUtyY1Z3NF83elk3c0k2UnZyRGhqZFJES1RIRzBuVWNSTVAySU5tNTVGTWltN1JWQnJmd2d6NA&q=https%3A%2F%2Fgodbolt.org%2Fg%2FGaVAyF&v=fV6qYho-XVs
	//https://www.youtube.com/redirect?event=video_description&redir_token=QUFFLUhqbHZDUEJudldQTlg4SUxxRm55RFUtWTI1WWJQQXxBQ3Jtc0tsZnVURzZlNEtueEtmajhKT1NSZHA5V1ZUaHUyclh5Uko0M0pIY3JUMUpfZ0d0aFdPUFcxWnBhWTFEX1hTZFNOTjJBa3h4V2NFVjZCWTBvRFJJSXFNY0phLXpONkgtM2t1aXlxclpkeUVoYXdSZDFIYw&q=https%3A%2F%2Fgodbolt.org%2Fg%2FGfsr2m&v=fV6qYho-XVs




















}