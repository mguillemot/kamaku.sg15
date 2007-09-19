#include "xparameters.h"
#include "sleep.h"
#include "isuperdma.h"

int main()
{
	xil_printf("start\r\n");
	isuperdma_init(XPAR_ISUPERDMA_0_BASEADDR);
	ivga_init(XPAR_IVGA_0_BASEADDR);
	ivga_on();
	
	Xuint32 fb = 0x2000000;
	Xuint32 addr = 0x4000000;
	Xuint16 *p = (Xuint16 *)addr;
	int i, j;
	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			*(p++) = i+j;
		}
	}
	p = (Xuint16 *)fb;
	for (i = 0; i < 320; i++)
	{
		for (j = 0; j < 240; j++)
		{
			*(p++) = 0;
		}
	}	
	xil_printf("preparation done\r\n");
	ivga_set_framebuffer(fb);
	isuperdma_blit(addr, 32, 0, 0, 32, 32, fb, 0, 0);
	xil_printf("blit done\r\n");
	sleep(1);
	xil_printf("sleep done\r\n");
	
	while(1);
	return 0;
}
