#pragma once
#include <chrono>
#include <iostream>
#include <assert.h>
#include <ranges>
#include <algorithm>
#include <print>
#include <filesystem>
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;


namespace chrono4
{
	void Examples()
	{
		//https://www.youtube.com/watch?v=adSAN282YIw
		//Opening Keynote Meeting C++ 2019 - Howard Hinnant - Design Rationale for the chrono Library
		auto limit = 2h;
		milliseconds x = limit; // 7'200'000ms
		auto y = duration_cast<hours>(x); // 2h
		duration<double> z = x; // 7'200.0s


		auto tp = system_clock::now(); // UTC time
		{
			auto tp2 = system_clock::now();
			auto diff = tp2 - tp;
		}

		{
			auto ymd = 31d / October / 2019y;	ymd += months{1};

			if (not ymd.ok()) ymd = ymd.year() / ymd.month() / last; // to snap to the end of the month:

			if (not ymd.ok()) ymd = sys_days{ymd}; // to overflow to the next month:
		}

		// year_month_weekday
		auto date1 = Thursday[4] / November / 2019;
		std::cout << date1.ok();
		auto FirstMonday = weekday_indexed{Monday,1};// first Monday of the month
		auto LastMonday1 = Monday[last];// last Monday of the month
		auto LastMonday2 = weekday_last{Monday};

		{
			auto date = Friday[5] / November / 2019;
			date += years{1};
			//* to snap to the end of the month(4th Friday of November 2020):
			if (!date.ok())
				date = sys_days{date.year() / date.month() / date.weekday()[last]};
			// To overflow into the next month(1th Friday of December/2020):
			if (!date.ok())
				date = sys_days{date};
		}

		{
			auto lt = zoned_time{current_zone(), tp}; // The current local time:

			// Difference between sys_days and local_days
			// sys_days is utc time
			// local_days is local time
			zoned_time tp1{"Europe/Berlin", sys_days{2019y / 11 / 14}};
			std::println("Default: {}", tp1);
			std::println("sys_time: {}", tp1.get_sys_time());
			std::println("local time: {}", tp1.get_local_time());

			zoned_time tp2{"Europe/Berlin", local_days{2019y / 11 / 14}};
			std::println("default: {}", tp2);
		}
		// ex2
		{
			zoned_time BerlinTime{"Europe/Berlin", local_days{2019y / 11 / 14} + 1h};
			zoned_time tp2{"America/New_York", BerlinTime};
			std::cout << BerlinTime << "\n";
			std::cout << tp2 << "\n";
		}
		{
			sys_days sd{2019y / 11 / 14};// A UTC time_point
			local_days d{2019y / 11 / 14};// same as local_time<days>
			// A somewhat nebulous time_point, until you pair it with a time_zone:
		}
		// Example: Directions Group meeting times.
		for (auto d = January / 9 / 2019; d.year() < 2020y; d = sys_days{d} + weeks{2}) {
			zoned_time London{"Europe/London", local_days{d} + 18h};
			println("{}", London);
			println("{}", zoned_time{"America/New_York", London});
		}
		{
			// example of using strftime and put_time
			auto tp = system_clock::now();
			auto lt = zoned_time{current_zone(), tp};
			std::cout << std::format("{:%Y-%m-%d %H:%M:%S}", lt) << "\n";
		}
		{
			auto tp = system_clock::now();
			// create time zone for "Europe/Berlin"
			auto tz = locate_zone("Europe/Berlin");
			// same as default
			std::cout << std::format("{:%F %T %Z}\n", zoned_time{tz,tp}) << "\n";
			// other format
			std::cout << std::format(
				"{:%d.%m.%Y %T%z}\n", zoned_time{tz,tp}) << "\n";
			std::cout << std::format(
				std::locale("de_DE"),
				"{:%d.%m.%Y %T%z}\n", zoned_time{tz,tp}) << "\n";

			// to see only seconds
			std::cout << std::format(
				std::locale("de_DE"),
				"{:%d.%m.%Y %T}\n", zoned_time{tz, floor<seconds>(tp)}) << "\n";
		}
		{
			// parsing in
			std::cout << "Parsing Example\n";
			std::istringstream in1{"2019-11-14 18:00:00"};
			system_clock::time_point tp;
			in1 >> parse("%F %T", tp);
			std::cout << tp << "\n";
			std::istringstream in2{"14.11.2019 11:13:40.785346+0100"};//with utc 1 hour offset
			in2 >> parse("%d.%m.%Y %T %z", tp);
			std::cout << tp << "\n";
		}
		{
			// file_clock is the same type as
			// std::file_system::file_time_type::clock
			// file_clock's epoch is unspecified.
			std::filesystem::path p{"file.txt"};
			auto ft = std::filesystem::last_write_time(p);
			// file_time can be cast to sys_time(and vice-versa) via clock_cast
			auto tp = clock_cast<system_clock>(last_write_time(p));

			last_write_time(p, clock_cast<file_clock>(tp));

		}
		{
			// utc_time is just like sys_time except that it counts leap seconds.
			// * Useful when subtracting time_points across a leap second insertion point 
			// and 1s accuracy is required.
			// * clock_cast can be used to convert between utc_time, file_time , and sys_time.
			// * utc_clock::now() is allowed but not required to be accurate during a leap second insertion
			// * Formatting and parsing utc_time allows for 61s in a minute, but only for a utc_time that is
			// actually referencing a leap second insertion.

		}
	}

}



namespace Calendar_And_Time_Zones_in_MSVC {

	using namespace std::chrono_literals;
// https://youtu.be/WX3OmVu4IAs
// https://youtu.be/MODhhr7m-5s
// https://youtu.be/c7DT28TV0AY


	using namespace std::chrono;

	void Calendar_Samples1() {

#pragma region Weekdays
		weekday wd{Thursday};
		auto result = wd + days{4};
		std::cout << result << "\n"; // Monday

		weekday sun1{0};
		weekday sun2{7};
		std::cout << sun1 << " " << sun2 << std::endl;

		weekday_indexed wdi{wd, 4};//fourth Thursday
		std::cout << wdi << std::endl;
#pragma endregion

#pragma region Compound_Calendar_Types
		year this_year{2021};
		year last_year{2020};

		year_month_day ymd{this_year, October, day{28}};
		std::cout << ymd << "\n";

		month_weekday mwd{November, Thursday[4]};
		std::cout << mwd << "\n";

		month_day_last mdlast{February};

		year_month_day_last ymdlast_leap{last_year, mdlast};
		year_month_day_last ymdlast_noleap{this_year, mdlast};

		std::cout << ymdlast_leap << "\t" << ymdlast_leap.day() << "\n";
		std::cout << ymdlast_noleap << "\t" << ymdlast_noleap.day() << "\n";

#pragma endregion
	}

	void Calendar_Samples2() {

		year_month_day ymd1{October / 28d / 2021y};
		year_month_day ymd2{2021y / October / 28d};

		std::cout << ymd1 << "\t" << ymd2 << "\n";

		year_month_day_last
			ymdlast{2021y / (October + months{1}) / last};
		std::cout << ymdlast << "\n";
		std::cout << year_month_day{ymdlast} << "\n";

		month_day md{October / 28};
		std::cout << md << "\n";

		month_weekday mwd{October / Wednesday[2]};
		std::cout << mwd << "\n";
	}

	void Windows_Patch_Tuesdays() {
		std::cout << "Patch Tuesdays in 2024:\n";
		year target_year{2024};
		for (int mo = 1; mo <= 12; ++mo) {
			year_month_weekday patch_tuesday{mo / Tuesday[2] / target_year};
			year_month_day ymd{sys_days{patch_tuesday}};
			std::cout << ymd.month() << "/" << ymd.day() << "\n";
		}
	}

	void Examples_Of_Clocks() {
		/*
		* https://github.com/CppCon/CppCon2021/blob/main/Presentations/Cpp20_Chrono.pptx
		* https://www.youtube.com/watch?v=Dq7rqjatxz8
		system_clock, file_clock
		c++20 added: utc_clock, tai_clock, gps_clock, file_clock(new feature)

		system_clock Jan 1 1970 00:00:00, Tracks UTC or GMT* time.
		utc_clock    Jan 1 1970 00:00:00, Tracks UTC or GMT* time. Tracks Leap seconds

		tai_clock Tracks International Atomic Time Jan 1 1958 00:00:00
		gps_clock Tracks GPS time Jan 6 1980 00:00:00

		file_clock has 2 epochs: Jan 1 1970 00:00:00 On POSIX and Jan 1 1601 00:00:00 On Windows
			used to create time_point system used for file_time_type, Tracks Leaps seconds (Unspecified)
		*/
		tai_time tai_now = tai_clock::now();
		std::cout << tai_now << "\n";

		utc_time utc_now = clock_cast<utc_clock>(tai_now);
		std::cout << utc_now << "\n";

		// above have difference of 37 seconds
		// This is because:
		// * there is a 10 second offset between TAI and UTC
		// * there have been 27 leap seconds since 1970

	}

	void Time_Zone_Examples() {
		// zoned_time represents a pairing of a time_zone and a time_point
		// Can produce the local_time or equivalent sys_time
		/*
			template<class Duration>
			using sys_time = time_point<system_clock, Duration>;
			template<class Duration>
			using local_time = time_point<local_t, Duration>;

			Conversions between local_time and sys_time may be tricky because of daylight savings.
			 two exceptions:
			* ambiguous_local_time
				* Consider local_time that occurs during the daylight saving time transition when an extra hour
				* is inserted. When that happens, that one-hour block essentially occurs twice.
				* If the local_time occurs in that period of time, there are two potential sys_times that it
				* could be converted to.
				* If the choice of earlier or later isn't specified (through choose::earliest or choose::latest),
				* an ambiguous_local_time exception is thrown.
			* nonexistent_local_time
				* Consider local_time that occurs during a daylight saving time transition when an hour is lost.
				* If the local_time to be converted occurs in that period of time, there is no sys_time that it can
				* correspond to so a nonexistent_local_time exception is thrown.
		*/


		try
		{
			auto ld = local_days{Sunday[1] / November / 2016};
			auto lt = ld + 1h + 30min;
			auto zt = zoned_time{"America/New_York", lt};
		}
		catch (const ambiguous_local_time& e)
		{
			std::cout << e.what() << "\n";
		}
		// sys_time is utc time

		try
		{
			auto lt = local_days{Sunday[2] / March / 2016} + 2h + 30min;
			auto zt = zoned_time{"America/New_York", lt};
		}
		catch (const nonexistent_local_time& e)
		{
			std::cout << e.what() << "\n";
		}

		// tzdb
		const auto& db = get_tzdb();

		std::cout << "Time zone descriptions:\n";
		std::for_each(db.zones.begin(), db.zones.end(), [](const auto& z) {
			std::cout << "Zone: " << z.name() << "\n";
		});
	}
}

namespace chrono2
{
	// To check if time has expired.
	bool IsDurationExpired(long long durationInSeconds)
	{
		// Get the current time point
		auto currentTime = std::chrono::system_clock::now();

		// Convert current time to seconds
		auto now_seconds = std::chrono::time_point_cast<std::chrono::seconds>(currentTime);
		// or
		auto now_seconds2 = duration_cast<seconds>(currentTime.time_since_epoch()).count();

		// Check if the current time is after the expiry time
		return now_seconds.time_since_epoch().count() > durationInSeconds;
	}

	void seconds_to_time_t_and_from_time_t()
	{
		constexpr __int64 s1 = 100;
		// convert 10 seconds to time_point
		auto time_point1 = system_clock::from_time_t(s1);
		// convert time_point to seconds
		auto s2 = system_clock::to_time_t(time_point1);
		if (s1 == s2) {
			std::puts("times are the same");
		}
	}
}

namespace chrono1
{
	struct user { system_clock::time_point afk; };
	auto afk_display(user u) -> std::string {
		using five_minutes = duration<system_clock::rep, std::ratio<5 * 60>>;

		const minutes afk_period =
			duration_cast<five_minutes>(system_clock::now() - u.afk);
		return std::format("AFK for {}", afk_period);
	}

	using rep_t = std::chrono::system_clock::duration::rep;

	using season_of_love = std::chrono::duration<rep_t, std::ratio<525'600 * 60>>;

	using medication_period = std::chrono::duration<rep_t, std::ratio<6 * 60 * 60>>;

	constexpr auto today = day{8} / May / 2023;
	constexpr auto today1 = May / 8 / 2023;
	constexpr auto today2 = year{2023} / May / 8;

	constexpr auto day_of_the_week(year_month_day ymd) -> weekday
	{
		return year_month_weekday{static_cast<sys_days>(ymd)}.weekday();
	}

	constexpr auto thanksgiving_us(int year) -> year_month_day {
		return static_cast<sys_days>(November / Thursday[4] / year);
	}

	constexpr auto thanksgiving_ca(int year) -> year_month_day {
		return static_cast<sys_days>(October / Monday[4] / year);
	}

	constexpr auto enemy_meetup(int year, auto month)
		-> year_month_day {
		const auto last_Thursday =
			month / Thursday[std::chrono::last] / year;
		return static_cast<sys_days>(last_Thursday);
	}

	constexpr auto advent(int year) -> year_month_day {
		const auto xmas_eve =
			--static_cast<sys_days>(December / 25 / year);
		const auto Sunday = xmas_eve
			- (year_month_weekday{xmas_eve}.weekday() - Sunday);
		return Sunday - weeks{3};
	}

	// how many Fridays in 13th of any month.
	constexpr auto unlucky_fridays(int year) {
		return std::ranges::count_if(
			std::ranges::views::iota(1u, 13u),
			[&](auto m) {
			const auto d = static_cast<sys_days>(month{m} / 13 / year);
			return year_month_weekday{d}.weekday() == Friday;
		}
		);
	}
}