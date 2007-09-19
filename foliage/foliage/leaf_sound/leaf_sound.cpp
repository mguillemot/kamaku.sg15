#include <iostream>
#include <memory>
#include "leaf_sound.hpp"

#ifdef __PPC__
	#include <xac97_l.h>
	#include <xparameters.h>
	#include <xintc_l.h>
	#include <xexception_l.h>
#endif

Foliage::Sound *Foliage::SoundManager::_bg = NULL;
Foliage::Sound *Foliage::SoundManager::_sfx = NULL;

void Foliage::SoundManager::AC97_Callback(void *useless)
{
	Uint32 sample_sfx, sample_bg, sample_mixed;
	Sint32 g1, g2, g, d1, d2, d;
	for (Sint32 i = 0; i < 256; i++)
	{
		sample_bg = (_bg != NULL) ? _bg->getNextSample() : 0;
		sample_sfx = (_sfx != NULL) ? _sfx->getNextSample() : 0;
		g1 = (Sint32)((Sint16)(sample_bg >> 16));
		d1 = (Sint32)((Sint16)(sample_bg & 0xffff));
		g2 = (Sint32)((Sint16)(sample_sfx >> 16));
		d2 = (Sint32)((Sint16)(sample_sfx & 0xffff));
		g = (g1 + g2) >> 1;
		d = (d1 + d2) >> 1;
		/*
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
		*/
		sample_mixed = ((Uint32)g << 16);
		sample_mixed |= ((Uint32)d & 0xffff);
		XAC97_WriteFifo(XPAR_AUDIO_CODEC_BASEADDR, sample_mixed);
	}
	if (_bg != NULL && _bg->ended())
	{
		_bg = NULL;
	}
	if (_sfx != NULL && _sfx->ended())
	{
		_sfx = NULL;
	}
}

void Foliage::SoundManager::init()
{
	// hard reset & initialization
	XAC97_HardReset(XPAR_AUDIO_CODEC_BASEADDR);
	XAC97_InitAudio(XPAR_AUDIO_CODEC_BASEADDR, AC97_ANALOG_LOOPBACK);
	XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_MIC_INPUT);
	XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_LINE_INPUT);
	XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);
	
	// volume settings
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVol, AC97_VOL_MAX);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_AuxOutVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVolMono, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCBeepVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PhoneInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MicVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_LineInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_CDVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_VideoVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_AuxInVol, AC97_VOL_MUTE);
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_PCMOutVol, AC97_VOL_MAX);
	XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);
	
	// VRA mode OFF
	XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_ExtendedAudioStat, 0);
	
	// clear FIFOs
	XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);

	// interrupt
	XIntc_RegisterHandler(XPAR_OPB_INTC_0_BASEADDR,
		XPAR_OPB_INTC_0_AUDIO_CODEC_INTERRUPT_INTR,
		(XInterruptHandler)Foliage::SoundManager::AC97_Callback,
		NULL);
	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
	
	std::cout << " * sound manager initialized" << std::endl;
}

void Foliage::SoundManager::disableSound()
{
	XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, 0);
	std::cout << "LEAF_sound: sound disabled" << std::endl;
}

void Foliage::SoundManager::playBg(Foliage::Sound *bg)
{
	bg->rewind();
	_bg = bg;
}

void Foliage::SoundManager::playSfx(Foliage::Sound *sfx)
{
	sfx->rewind();
	_sfx = sfx;
}
