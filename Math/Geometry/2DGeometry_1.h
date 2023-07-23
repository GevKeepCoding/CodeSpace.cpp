#pragma once

import std;


namespace Util
{
	// Vector2 implementation
	struct Vector2 {
		double x{0.0};
		double y{0.0};

	public:
		Vector2() = default;
		Vector2(double x, double y) : x(x), y(y) {}

		// Getter
		double getX() const { return x; }
		double getY() const { return y; }

		// Setter
		void setX(double x) { this->x = x; }
		void setY(double y) { this->y = y; }

		// Add two vectors
		Vector2 operator+(const Vector2& other) const {
			return Vector2(x + other.x, y + other.y);
		}

		// Subtract two vectors
		Vector2 operator-(const Vector2& other) const {
			return Vector2(x - other.x, y - other.y);
		}

		// Multiply vector by scalar
		Vector2 operator*(double scalar) const {
			return Vector2(x * scalar, y * scalar);
		}

		// Divide vector by scalar
		Vector2 operator/(double scalar) const {
			return Vector2(x / scalar, y / scalar);
		}

		// Dot product of two vectors
		double dot(const Vector2& other) const {
			return x * other.x + y * other.y;
		}

		// Cross product of two vectors
		double cross(const Vector2& other) const {
			return x * other.y - y * other.x;
		}

		// Get the length of the vector
		double length() const {
			return std::sqrt(x * x + y * y);
		}

		// Get the squared length of the vector
		double lengthSquared() const {
			return x * x + y * y;
		}

		// Normalize the vector
		Vector2 normalize() const {
			double l = length();
			if (l < 1e-8) {
				return Vector2(0, 0);
			}
			return *this / l;
		}

		// Get the angle of the vector
		double angle() const {
			return std::atan2(y, x);
		}

		// Rotate the vector by an angle
		Vector2 rotate(double angle) const {
			double c = std::cos(angle);
			double s = std::sin(angle);
			return Vector2(x * c - y * s, x * s + y * c);
		}

		// Get the distance between two vectors
		static double Distance(const Vector2& a, const Vector2& b) {
			return (a - b).length();
		}
	};
	//Point Line - Segment Distance : https://www.youtube.com/watch?v=egmZJU-1zPU
	//  
	double PointSegmentDistance(Vector2 p, Vector2 a, Vector2 b, Vector2& cp)
	{
		Vector2 ab = b - a;
		Vector2 ap = p - a;
		double proj = ap.dot(ab);
		double abLenSq = ab.lengthSquared();
		double d = proj / abLenSq;
		if (d <= 0.0) {
			cp = a;
		}
		else if (d >= 1.0) {
			cp = b;
		}
		else {
			cp = a + ab * d;
		}
		return Vector2::Distance(p, cp);
	}

	bool arePerpendicular(Vector2 a1, Vector2 a2, Vector2 b1, Vector2 b2) {
		double slopeA = (a2.getY() - a1.getY()) / (a2.getX() - a1.getX());
		double slopeB = (b2.getY() - b1.getY()) / (b2.getX() - b1.getX());
		return std::abs(slopeA * slopeB + 1.0) < 1e-8; // account for floating point precision
	}



	/**
 * @param p Point for which we want to calculate distance from the line segment.
 * @param start starting point of the segment
 * @param end end point of the segment.
 * @return closest distance from point to segment
 */
	inline double pointLineDistance3(const Vector2& p, const Vector2& start, const Vector2& end) {
		double dx = end.x - start.x;
		double dy = end.y - start.y;

		// Handle degenerate case where start and end points are the same
		if (dx == 0 && dy == 0) {
			return std::sqrt((p.x - start.x) * (p.x - start.x) + (p.y - start.y) * (p.y - start.y));
		}

		double t = ((p.x - start.x) * dx + (p.y - start.y) * dy) / (dx * dx + dy * dy);

		if (t < 0) {
			return std::sqrt((p.x - start.x) * (p.x - start.x) + (p.y - start.y) * (p.y - start.y));
		}
		else if (t > 1) {
			return std::sqrt((p.x - end.x) * (p.x - end.x) + (p.y - end.y) * (p.y - end.y));
		}

		double projX = start.x + t * dx;
		double projY = start.y + t * dy;

		return std::sqrt((p.x - projX) * (p.x - projX) + (p.y - projY) * (p.y - projY));
	}



	// Get the distance between two vectors
	inline auto Distance(const Vector2& a, const Vector2& b) {
		auto ab_x = b.x - a.x;
		auto ab_y = b.y - a.y;
		return std::sqrt(ab_x * ab_x + ab_y * ab_y);
	}

	inline bool is_vertical(Vector2 a, Vector2 b) {
		return a.x == b.x;
	}

	inline bool is_horizontal(Vector2 a, Vector2 b) {
		return a.y == b.y;
	}


	// returns new top_left if it should be changed
	std::optional<Vector2> find_reducer_vertex(Vector2 a, Vector2 b, Vector2 c, Vector2 d, double minDistance)
	{
		//auto bc_distance = Distance(b, c);
		////Vector2 closest_point;
		////auto closest_distance_from_c_point_to_ab = pointSegmentDistance(c, a, b, closest_point);
		//auto closest_distance_from_c_point_to_ab = pointLineDistance3(c, a, b);
		//if (bc_distance != closest_distance_from_c_point_to_ab) {
		//	return std::nullopt;
		//}


		Vector2 result;
		if (is_horizontal(a, b)) {
			result.y = b.y;
			result.x = c.x;
		}
		else {
			result.x = b.x;
			result.y = c.y;
		}

		if (Distance(b, result) < minDistance || Distance(result, c) < minDistance)
		{
			return result;
		}
		return std::nullopt;
	}


	std::vector<Vector2> simplify_with_reducer(const std::vector<Vector2>& InputPolygon, double minDistance = 4.0) {

		if (InputPolygon.size() < 4) { return InputPolygon; }

		std::vector<Vector2> polygon = InputPolygon;
		//std::vector<Vector2> sp;
		size_t reduced_count = 0;
		for (size_t i = 0; i + 1 < polygon.size() && polygon.size() - reduced_count > 3; i++) {
			auto a = polygon[i];
			auto b = polygon[(i + 1) % polygon.size()];
			auto c = polygon[(i + 2) % polygon.size()];
			auto d = polygon[(i + 3) % polygon.size()];

			// check line that goes from a->b is perpendicular to b->c segment without using is_horizontal and is_vertical functions
			bool found_reducible_edge = (is_horizontal(a, b) && is_vertical(c, d));
			found_reducible_edge = found_reducible_edge || (is_vertical(a, b) && is_horizontal(c, d));

			// the other way to find reducible edge todo: test this
			// bool found_reducible_edge2 = arePerpendicular(a, b, c, d);
			if (!found_reducible_edge) {
				//sp.push_back(a);
				continue;
			}

			auto new_top = find_reducer_vertex(a, b, c, d, minDistance);

			//sp.push_back(a);
			if (new_top.has_value()) {
				//sp.push_back(new_top.value());

				{
					// if last iteration then
				}
				polygon[i + 1] = new_top.value();
				polygon.erase(polygon.begin() + (i + 2) % polygon.size());
				reduced_count++;
			}
			else {
				//assert(false, "should not happen");
				//sp.push_back(a);
			}
		}

		return polygon;
	}




	//*c-d
	//b
	//|
	//a
	//*c-d
	// or
	//b
	//|\
	//|*c--d
	//|*****
	// a








}







using namespace Util;
int main() {
	// example of vector distance
	Vector2 a(0, 3);
	Vector2 b(4, 0);
	Vector2 p(0, 0);

	// write triangle sides that has height a whole number



	Vector2 cp;
	std::println("{:f}", PointSegmentDistance(p, a, b, cp));

	std::println("{:f}", Vector2::Distance(a, b));



}