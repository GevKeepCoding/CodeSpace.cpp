#pragma once

//https://www.youtube.com/watch?v=ReTetN51r7A
namespace floatings_points1
{
/* 1.
Instead of multiplying by - 1.0f or subtracting them fromo 0.0f we can flip the sign bit
by xoring the number with "-0.0f" (same as 0x80000000).
It has all 0's for the exponent and mantissa but a 1 for the sign bit. This trick works for
negative, positive, NaN, 0 and infinity.
*/
inline unsigned int AsInt(float f) {
	return *(unsigned int*)&f;
}
inline float AsFloat(unsigned int i) {
	return *(float*)&i;
}


float flip_sign(float x)
{
	return AsFloat(AsInt(x) ^ 0x80000000);
}

// 2. log2(x)
const unsigned int OneAsInt = 0x3F800000; // 1.0f as int
const float ScaleUp = float(0x00800000); // 2^23
const float ScaleDown = 1.0f / ScaleUp; // 2^-23

// only works for positive numbers
float log2(float x)
{
	return (AsInt(x) - OneAsInt) * ScaleDown;
}

// 3. Exp2(x)
float exp2(float x)
{
	return AsFloat(int(x * ScaleUp) + OneAsInt);
}
// 4. pow(x, p)
// x^p = 2^(p * log2(x)) -- this provides only a very rough approximation
float Pow1(float x, float p)
{
	return exp2(p * log2(x));
}

float Pow(float x, float p)
{
	return AsFloat(int(p * (AsInt(x) - OneAsInt)) + OneAsInt);
}
// 5. Sqrt(x)
float Sqrt(float x)
{
	return AsFloat((AsInt(x) >> 1) + (OneAsInt >> 1));
}

float SqrtNewRap(float x)
{
	float y = Sqrt(x); // get initial guess
	// Newton-Raphson iteration as many times as we want: It lwill converge to sqrt(x)
	return (y*y + x)/(2 * y);
}

// 6. Fast inverse square root
// "Carmack's Hack" or "Fast Inverse Square Root"
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float InvSqrt1(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // get bits for floating value
	i = 0x5f3759df - (i >> 1); // gives initial guess y0
	x = *(float*)&i; // convert bits back to float
	x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
	return x;
}
// Lomont's fast inverse square root seems to be better approximation.
float InvSqrt2(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x; // get bits for floating value
	i = 0x5f375a86 - (i >> 1); // gives initial guess y0
	x = *(float*)&i; // convert bits back to float
	x = x * (1.5f - xhalf * x * x); // Newton-Raphson iteration
	return x;
}

} // namespace floatings_points1
