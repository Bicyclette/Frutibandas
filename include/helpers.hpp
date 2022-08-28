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

template <typename T>
T get_max_array(T a[], int count)
{
	T maximum = a[0];
	for (int i = 1; i < count; ++i) {
		if (a[i] > maximum && a[i] != -1) {
			maximum = a[i];
		}
	}
	return maximum;
}

template <typename T>
T get_min_array(T a[], int count)
{
	T minimum;
	int j = 0;
	do
	{
		minimum = a[j];
		j++;
	} while (minimum == -1 && j < count);
	for (int i = 1; i < count; ++i) {
		if (a[i] < minimum && a[i] != -1) {
			minimum = a[i];
		}
	}
	return minimum;
}

#endif
