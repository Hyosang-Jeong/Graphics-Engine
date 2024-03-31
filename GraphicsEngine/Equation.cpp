#include"Equation.h"

BLOCK BuildHammersleyPoints(int block_N)
{
	BLOCK block;
	int kk;
	int pos = 0;

	for (int k = 0; k < block_N; k++)
	{
		kk = k;
		float u = 0.0f;
		for (float p = 0.5f; kk; p *= 0.5f, kk >>= 1)
		{
			if (kk & 1)
				u += p;
		}
		float v = (k + 0.5) / (float)block_N;
		block.hammersley[pos++] = u;
		block.hammersley[pos++] = v;
	}

	return block;
}