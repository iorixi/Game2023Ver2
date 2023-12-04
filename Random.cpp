#include "Random.h"
#include <stdlib.h>

C_Random::C_Random()
{
}

C_Random::~C_Random()
{
}

int C_Random::Get_Random(int min, int max)
{


	return min + (int)(rand() *(max - min + 1.0) / (1.0 + RAND_MAX));


}