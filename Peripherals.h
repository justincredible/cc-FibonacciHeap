#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#define NIL 0

// computes the least 32-bit integer, i.e., the integer with greatest magnitude (which is negative).
int negInfinity()
{
	int int_min = 1;
	for ( int i = 1; i < 32; i++ )
		int_min *= 2;
	return int_min;
}

#endif