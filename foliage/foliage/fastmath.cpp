#include <iostream>
#include <cmath>
#include "basic_types.hpp"
#include "fastmath.hpp"

#ifdef __PPC__
	#include <xparameters.h>
#endif

Foliage::Fixed *Foliage::FastMath::atan_t0 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 0);
Foliage::Fixed *Foliage::FastMath::atan_t1 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 1);
Foliage::Fixed *Foliage::FastMath::atan_t2 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 2);
Foliage::Fixed *Foliage::FastMath::atan_t3 = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 3);
Foliage::Fixed *Foliage::FastMath::cos_t = (Foliage::Fixed *)(XPAR_PLB_BRAM_IF_CNTLR_1_BASEADDR + 1024 * sizeof(Foliage::Fixed) * 4);

Foliage::Fixed Foliage::FastMath::cos(const Foliage::Fixed x)
{
	if (x < 0)
	{
		return cos(x.opposite());
	}	
	if (x > F_TWOPI)
	{
		return cos(x - F_TWOPI);
	}
	if (x > F_PI)
	{
		return cos(x - F_PI).opposite();
	}
	if (x > F_PI_2)
	{
		return cos(F_PI - x).opposite();
	}
	Fixed x2(x);
	x2 *= F_2000_OVER_PI;
	Sint32 i = int(x2);
	return Foliage::FastMath::cos_t[i];
}

Foliage::Fixed Foliage::FastMath::sin(const Foliage::Fixed x)
{
	return Foliage::FastMath::cos(F_PI_2 - x);
}

Foliage::Fixed Foliage::FastMath::atan(const Foliage::Fixed x)
{
	if (x < 0)
	{
		return atan(x.opposite()).opposite();
	}
	if (x < F_0_DOT_1)
	{
		Sint32 i = int(x * F_10000);
		return Foliage::FastMath::atan_t0[i];
	}
	else if (x < F_1)
	{
		Sint32 i = int(x * F_1000);
		return Foliage::FastMath::atan_t1[i];
	}
	else if (x < F_10)
	{
		Sint32 i = int(x * F_100);
		return Foliage::FastMath::atan_t2[i];
	}
	else if (x < F_100)
	{
		Sint32 i = int(x * F_10);
		return Foliage::FastMath::atan_t3[i];
	}
	else
	{
		return F_PI_2;
	}
}

void Foliage::FastMath::init()
{
	for (Sint32 i = 0; i < 1000; i++)
	{
		atan_t0[i] = Foliage::Fixed(std::atan(i * (0.1f / 1000)));
		atan_t1[i] = Foliage::Fixed(std::atan(i * (1.0f / 1000)));
		atan_t2[i] = Foliage::Fixed(std::atan(i * (10.0f / 1000)));
		atan_t3[i] = Foliage::Fixed(std::atan(i * (100.0f / 1000)));
		cos_t[i] = Foliage::Fixed(std::cos(i * ((float)M_PI_2 / 1000)));
	}
}
