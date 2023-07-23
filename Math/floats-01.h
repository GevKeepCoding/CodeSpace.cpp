#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iomanip>

void PrintCurrentRoundingMode()
{
	unsigned int cw;
	_controlfp_s(&cw, 0, 0);
	cw &= _MCW_RC;
	printf("Rounding mode is ");
	switch (cw) {
	case _RC_NEAR:
		printf("round to nearest\n");
		break;
	case _RC_UP:
		printf("round up\n");
		break;
	case _RC_DOWN:
		printf("round down\n");
		break;
	case _RC_CHOP:
		printf("chop\n");
		break;
	}
}


int main() {
	float f = 1.2f;
	int* asInt = reinterpret_cast<int*>(&f);
	printf("%1.30f\n", f);
	
	std::cout << "Float "<<std::fixed << std::setprecision(30) << f << std::endl;
	std::cout << "AsInt " << *asInt << std::endl;

	++(*asInt);
	std::cout << "After increment"<<std::fixed << std::setprecision(30) << f << std::endl;
}