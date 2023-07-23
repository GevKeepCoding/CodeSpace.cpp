#pragma once
#include <chrono>
#include <string>
#include <format>
#include <iostream>

using namespace std;
using namespace std::chrono;

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

	using seasin_of_love = std::chrono::duration<
		rep_t, std::ratio<525'600 * 60>>;

	constexpr auto today = day{ 8 } / May / 2023;
	constexpr auto today1 = May / 8 / 2023;
	constexpr auto today2 = year{ 2023 } / May / 8;

	constexpr auto day_of_the_week(year_month_day ymd) -> weekday
	{
		return year_month_weekday{
			static_cast<sys_days>(ymd) }.weekday();
	}

	// Listing the Days
	auto first_day(std::chrono::year year)
	{
		//using namespace std::literals;
		using std::chrono::January;
		return std::chrono::sys_days{ year / January / 1d };
	}
	/*auto dates(std::chrono::year start, std::chrono::year stop)
	{
		using date = std::chrono::year_month_day;

		return views::iota(first_day(start), first_day(stop))
			| views::transform([](const auto day) { return date(day); });
	}*/
}



int main_chrono1() {
	chrono1::user u;
	u.afk = std::chrono::system_clock::now();

	auto res = chrono1::afk_display(u);

	std::print(std::cout, "{}", res);

	return 0;
}