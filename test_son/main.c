#include "sound.h"
#include "xparameters.h"
#include "xintc_l.h"
#include "xexception_l.h"
#include "xac97_l.h"
#include "xgpio_l.h"
#include "xgpio.h"
#include "idma.h"
#include "xtime_l.h"

XGpio Btn;
int but_int = 0;
volatile Xuint32 *ptr;
volatile Xuint32 *end_ptr;
volatile Xuint32 *bg_ptr;
volatile Xuint32 *bg_end_ptr;
volatile int sound_playing = 0;
volatile int bg_ending = 0;
volatile int bg_playing = 0;
Xuint32 duration = 0;
Xuint32 called = 0;

void son(void* useless)
{
	//cli();
	called++;
	XTime start, end;
	XTime_GetTime(&start);
	if (bg_playing)
	{
		Xint32 i;
		Xuint32 sample_sound, sample_bg, sample_mixed;
		Xint32 g1, g2, g, d1, d2, d;
		for (i = 0; i < 256; i++)
		{
			sample_bg = *(bg_ptr++);
			if (sound_playing)
			{
				sample_sound = *(ptr++);
			}
			else
			{
				sample_sound = 0;
			}
			// et on mixe
			g1 = (Xint32)((Xint16)(sample_bg >> 16));
			d1 = (Xint32)((Xint16)(sample_bg & 0xffff));
			g2 = (Xint32)((Xint16)(sample_sound >> 16));
			d2 = (Xint32)((Xint16)(sample_sound & 0xffff));
			g = (g1 + g2) / 2;
			d = (d1 + d2) / 2;
			if (g > 32767)
			{
				g = 32767;
			}
			else if (g < -32768)
			{
			g = -32768;
				}
			if (d > 32767)
			{
				d = 32767;
			}
			else if (d < -32768)
			{
				d = -32768;
			}
			sample_mixed = ((Xuint32)g << 16);
			sample_mixed |= ((Xuint32)d & 0xffff);
			XAC97_WriteFifo(XPAR_AUDIO_CODEC_BASEADDR, sample_mixed);
		}
		if (bg_ptr >= bg_end_ptr)
		{
			bg_playing = 0;
			bg_ending = 1;
		}
		if (ptr >= end_ptr)
		{
			sound_playing = 0;
		}
	}
	if (bg_ending && XAC97_isInFIFOEmpty(XPAR_AUDIO_CODEC_BASEADDR))
	{
		// c'est fini
		XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, 0); // cut interrupt
		bg_playing = 0;
		bg_ending = 0;
		XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);
		xil_printf("bg end, close audio\r\n");
		xil_printf("total duration=%d called=%d\r\n", duration, called);
		xil_printf("sizeof(XTime)=%d\r\n", sizeof(XTime));
		XTime a = 0x1234567812345678;
		XTime b = 0x1234567800000000;
		Xuint32 c = a - b;
		xil_printf("a=%016x b=%016x a-b=%08x\r\n", a, b, c);
	}
	XTime_GetTime(&end);
	duration += end - start;
	//sti();
}

void but(void *BtnPtr)
{
	Xuint32 tmp = XGpio_DiscreteRead(BtnPtr, XGPIO_IR_CH1_MASK);
	but_int = 1;
    XGpio_InterruptClear(BtnPtr, XGPIO_IR_CH1_MASK);
}

XStatus play_bg(Sound *s)
{
  bg_end_ptr = s->start + (s->size / 4);

  if (! is_valid(s))
    return XST_FAILURE;

  /** Wait for the ready signal **/
  XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);

  /** Disable VRA Mode **/
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_ExtendedAudioStat, 0);

  /** Play Volume Settings **/
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVolMono, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCBeepVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCMOutVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_LineInVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MicVol, AC97_VOL_MID);

  /** Clear FIFOs **/
  XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);
  
  /** Playing sound **/
  bg_ptr = s->start;
  bg_playing = 1;
  XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);

  return XST_SUCCESS;
}

XStatus play_sound(Sound *s)
{
  end_ptr = s->start + (s->size / 4);

  if (! is_valid(s))
    return XST_FAILURE;

  /** Playing sound **/
  ptr = s->start;
  sound_playing = 1;
  
  return XST_SUCCESS;
}

int main()
{
	cli();
    XExc_Init();
	XExc_RegisterHandler(XEXC_ID_NON_CRITICAL_INT, (XExceptionHandler)XIntc_DeviceInterruptHandler, (void *)XPAR_OPB_INTC_0_DEVICE_ID);
	
  XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
				 XPAR_OPB_INTC_0_AUDIO_CODEC_INTERRUPT_INTR,
				 (XInterruptHandler)son,
				 NULL
				 );
				 
	XGpio_Initialize(&Btn, XPAR_PUSHBUTTONS_5BIT_DEVICE_ID);
    XGpio_InterruptEnable(&Btn, XGPIO_IR_CH1_MASK);
    XGpio_InterruptGlobalEnable(&Btn);
  XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
				 XPAR_OPB_INTC_0_PUSHBUTTONS_5BIT_IP2INTC_IRPT_INTR,
				 (XInterruptHandler)but,
				 &Btn
				 );
				 
	XIntc_mMasterEnable(XPAR_OPB_INTC_0_BASEADDR);
	XIntc_mEnableIntr(XPAR_OPB_INTC_0_BASEADDR, XPAR_PUSHBUTTONS_5BIT_IP2INTC_IRPT_MASK | XPAR_AUDIO_CODEC_INTERRUPT_MASK);
	sti();

	init_sound();

	/*
	xil_printf("test DMA\r\n");
	init_dma(XPAR_IDMA_0_BASEADDR);
	#define N 12340
	Xuint32 *src = (Xuint32 *)sbrk(N * 4);
	Xuint32 *dst = (Xuint32 *)sbrk(N * 4);
	xil_printf("src=%08x dst=%08x\r\n", src, dst);
	Xuint32 i, l;
	for (i = 0; i < N; i++)
	{
		*(src + i) = i;
	}
	for (l = 0; l < 42; l++)
	{
		for (i = 0; i < N; i++)
		{
			*(dst + i) = 0;
		}
		idma_synchrone_request(src, dst, N * 4);
		for (i = 0; i < N; i++)
		{
			if (*(dst + i) != i)
			{
				xil_printf("error at l=%d in address %08x : read %d instead of %d\r\n", l, (dst + i), *(dst + i), i);
			}
		}
	}
	xil_printf("test DMA ended\r\n");
	*/

	Sound s;
	Sound back;
	load_sound(&back, "kloops.wav");
	load_sound(&s, "break.wav"); // kloops.wav break.wav kloop11.wav
	
	xil_printf("bg start\r\n");
	play_bg(&back);
	
	while(1)
	{
		xil_printf("bing!\r\n");
		play_sound(&s);
		sleep(5);
	}
	
	return 0;
}
