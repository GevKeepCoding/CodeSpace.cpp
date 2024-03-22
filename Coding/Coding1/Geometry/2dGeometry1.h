#pragma once
import std;
using namespace std;




struct CodingInterview1
{
	/*
Google Coding Interview With A Competitive Programmer
https://www.youtube.com/watch?v=EuPSibuIKIg

// calculate number of rectangles in a given set of points

ex. 6 points.
(0,0) (0,1) (0,2)
(1,0) (1,1) (1,2)

. . .
. . .

The answer for above points is 3.

// (x,y) (x,y2) (x2, y) (x2,y2)
// x!=x2 and y!=y2

*/

	struct Point { int x; int y; };
	std::map<pair<int, int>, int> count{};
	int GetRectangleCount(const std::vector<Point>& points)
	{
		int answer{0};
		for (Point p : points)
			for (Point p_above : points)
				if (p.x == p_above.x && p.y < p_above.y) {
					pair<int, int> pair_y{p.y,p_above.y};
					answer += count[pair_y];
					count[pair_y]++;
				}
		return answer;
	}
};

struct Question2 {
	/*
	. * .
	* . *
	. * .

	The '*'s will make rect.

	(0,0) (x,y) (x2,y2)
	x * y2 = x2 * y
	ex. (0,0) (1,5) (-5, 1)

	O(N^4) for(A) for(B) for (C) for (D) if(....)

	*/



};


