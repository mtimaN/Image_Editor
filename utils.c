// Copyright Mantu Matei-Cristian 312CA 2022-2023
double clamp(double x)
{
	if (x < 0)
		x = 0;
	if (x > 255)
		x = 255;
	return x;
}

int is_power_of_2(int x)
{
	return x && (!(x & (x - 1)));
}

void my_swap_int(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

void swap_F(double *a, double *b)
{
	double c = *b;
	*b = *a;
	*a = c;
}
