#include "swap.h"

void Swap(char *left, char *right)
{
	// ваш код здесь
    char t = *left;
    *left = *right;
    *right = t;
}
