#pragma once


struct BLOCK {
	float hammersley[80];
};

BLOCK BuildHammersleyPoints(int block_N);
