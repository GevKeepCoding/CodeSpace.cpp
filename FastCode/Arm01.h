#pragma once

#include <arm_neon.h>

namespace Learning_Arm
{
	void types() {
		{	uint8x8_t va, vb, vr;

		vr = vadd_u8(va, vb);
		}
		{
			uint8x8_t a, b; // 8 8bit
			uint16x8_t a16, b16, c; // 8 16bit
			/* the naive way*/
			a16 = vmovl_u16(a);
			b16 = vmovl_u16(b);
			
			//c = vadd_u16(a16, b16);
			
			// better way: use lenghtening instruction
			c = vaddl_u8(a, b); // 1 instruction instead of 3


			c = vaddw_u8(a16, b);//widening instruction

		}
	}

}