#ifndef HELPERS_HPP
#define HELPERS_HPP

float my_lerp(float a, float b, float k);

template <typename T>
T my_min(T a, T b)
{
	return (a <= b) ? a : b;
}

template <typename T>
T my_max(T a, T b)
{
	return (a >= b) ? a : b;
}

#endif
