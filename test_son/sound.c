/*
 * Yggdrasil Operating System
 * $Id: sound.c,v 1.1.1.1 2006-03-31 19:01:34 root Exp $
 */

#include "sound.h"
#include "sysace_stdio.h"
#include "xac97_l.h"
#include "xio.h"
#include "xparameters.h"
#include "xstatus.h"

#define malloc sbrk

/*
 * sound driver
 *
 * @author: Matthieu Guillemot <g@npng.org>
 * @date: 07/06/2005
 * @version: 1.00
 *
 *
 * Driver son & primitives de gestion des objets sonores
 *
 */

void (*AC97_Callback)() = NULL;

/* structs for wave files */
typedef struct {
   unsigned int chunkID;
   unsigned int chunkSize;
   unsigned int format;
} RIFFCHUNK;

typedef struct {
   unsigned int subchunkID;
   unsigned int subchunkSize;
   unsigned short int audioFormat;
   unsigned short int numChannels;
   unsigned int sampleRate;
   unsigned int byteRate;
   unsigned short int blockAlign;
   unsigned short int bitsPerSample;
   unsigned short int extraParamSize;
} FMTCHUNK;

typedef struct {
   unsigned int subchunkID;
   unsigned int subchunkSize;
} DATACHUNK;

/* reads two bytes from file */
static unsigned short ReadUShort(SYSACE_FILE *fptr)
{
  unsigned char b0, b1;
  int numread;
  char readBuffer[2];

  numread = sysace_fread(readBuffer, 1, 2, fptr);

  b0 = readBuffer[0];
  b1 = readBuffer[1];

  return ((b1 << 8) | b0);
}

/* reads four bytes from file in little endian order */
static unsigned int ReadUIntLil(SYSACE_FILE *fptr)
{
  unsigned char b0, b1, b2, b3;
  int numread;
  char readBuffer[4];

  numread = sysace_fread(readBuffer, 1, 4, fptr);

  b0 = readBuffer[0];
  b1 = readBuffer[1];
  b2 = readBuffer[2];
  b3 = readBuffer[3];

  return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/* reads four bytes from file in big endian order */
static unsigned int ReadUIntBig(SYSACE_FILE *fptr)
{
  unsigned char b0, b1, b2, b3;
  int numread;
  char readBuffer[4];

  numread = sysace_fread(readBuffer, 1, 4, fptr);

  b0 = readBuffer[0];
  b1 = readBuffer[1];
  b2 = readBuffer[2];
  b3 = readBuffer[3];

  return ((((((b0 << 8) | b1) << 8) | b2) << 8) | b3);
}

/*
 * Opens and reads from CF a wave file
 *
 * File is assumed to have only RIFF, FMT, and DATA chunks and to be 44.1 kH,
 * 16 bit/sample, stereo.
 */
XStatus load_sound(Sound *s, char FileName[])
{
  unsigned char readBuffer[2048];
  SYSACE_FILE *infile;
  Xint32 i, numread, percent, new_percent;
  Xuint32 writeaddr, temp;
  RIFFCHUNK riffchunk;
  FMTCHUNK fmtchunk;
  DATACHUNK datachunk;

  percent = 0;
  infile = sysace_fopen(FileName, "r");
  if (infile)
  {
  xil_printf("file opened: %s\r\n", FileName);

    /* Read the RIFF chunk */
    riffchunk.chunkID = ReadUIntBig(infile);
    riffchunk.chunkSize = ReadUIntLil(infile);
    riffchunk.format = ReadUIntBig(infile);

    /* Read the FMT chunk */
    fmtchunk.subchunkID = ReadUIntBig(infile);
    fmtchunk.subchunkSize = ReadUIntLil(infile);
    fmtchunk.audioFormat = ReadUShort(infile);
    fmtchunk.numChannels = ReadUShort(infile);
    fmtchunk.sampleRate = ReadUIntLil(infile);
    fmtchunk.byteRate = ReadUIntLil(infile);
    fmtchunk.blockAlign = ReadUShort(infile);
    fmtchunk.bitsPerSample = ReadUShort(infile);

    /* Read the DATA chunk */
    datachunk.subchunkID = ReadUIntBig(infile);
    datachunk.subchunkSize = ReadUIntLil(infile);

    /*
    xil_sc_printff("ChunkID is: %x, should be %x\n\r",riffchunk.chunkID,0x52494646);
    xil_sc_printff("ChunkSize is %d bytes\n\r",riffchunk.chunkSize);
    xil_sc_printff("Format is: %x, should be %x\n\r",riffchunk.format,0x57415645);
    xil_sc_printff("SubchunkID is: %x, should be %x\n\r",fmtchunk.subchunkID,0x666d7420);
    xil_sc_printff("SubchunkSize is %d bytes\n\r",fmtchunk.subchunkSize);
    xil_sc_printff("SubchunkID is: %x, should be %x\n\r",datachunk.subchunkID,0x64617461);
    xil_sc_printff("SubchunkSize is %d bytes\n\r",datachunk.subchunkSize); // taille du buffer
    */

	Xuint32 totalSize = (datachunk.subchunkSize / 4) * 4;
	Xuint32 totalSizeEx = ((totalSize / 512) + 1) * 512;
    s->start = (Xuint32 *)malloc(totalSizeEx);
    if (s->start == NULL)
    {
      s->size = 0;
      xil_printf("Malloc failed.\r\n");
      return XST_FAILURE;
    }
    Xuint32 endaddr = (Xuint32)s->start + totalSize;
    s->size = totalSize;
    writeaddr = (Xuint32)s->start;
    XIo_Out32(writeaddr, datachunk.subchunkSize); // MAIS POURQUOI DONC ?
    writeaddr += 4;
    xil_printf("writeaddr=%08x endaddr=%08x size=%d start=%08x\r\n", writeaddr, endaddr, s->size, s->start);
    while (writeaddr < endaddr)
    {
      numread = sysace_fread(readBuffer, 1, 2048, infile);
      for (i = 0; i < numread && writeaddr < endaddr; i += 4)
      {
        temp = ((((((readBuffer[i+3] << 8) | readBuffer[i+2]) << 8) | readBuffer[i+1]) << 8) | readBuffer[i]);
        XIo_Out32(writeaddr, temp);
        writeaddr += 4;
      }
      new_percent = (100 * (writeaddr - (Xuint32)s->start)) / s->size;
      if (I_WANNA_SEE_MY_SOUNDS_BE_LOADED && new_percent >= percent + 10)
      {
        xil_printf("Loading sound... %d / 100 \r\n", new_percent);
        percent = new_percent;
      }
    }
    sysace_fclose(infile);
	 endaddr = (Xuint32)s->start + totalSizeEx;
	 while (writeaddr < endaddr) // fill end with blanks
	 {
	   XIo_Out32(writeaddr, 0);
		writeaddr += 4;
	 }
	 /*
    if (writeaddr != (Xuint32)s->start + s->size)
    {
      // on a alloué trop
      free(s->start);
      s->start = NULL;
      s->size = 0;
  xil_printf("allocated too much\r\n");
      return XST_FAILURE;
    }
	 */
    return XST_SUCCESS;
  }
  s->start = NULL;
  s->size = 0;
  xil_printf("could not open file\r\n");
  return XST_FAILURE;
}

XStatus unload_sound(Sound *s)
{
  free(s->start);
  return XST_SUCCESS;
}

void ac97_handler(void* useless)
{
  if (AC97_Callback != NULL)
  {
    AC97_Callback();
  }
}

void Set_AC97_Callback(void (*fn)())
{
	AC97_Callback = fn;
}

/*
 * Initialize AC'97
 */
XStatus init_sound()
{
  // gros reset & initialisation
  XAC97_HardReset(XPAR_AUDIO_CODEC_BASEADDR);
  XAC97_InitAudio(XPAR_AUDIO_CODEC_BASEADDR, AC97_ANALOG_LOOPBACK);
  XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_MIC_INPUT);
  XAC97_DisableInput(XPAR_AUDIO_CODEC_BASEADDR, AC97_LINE_INPUT);
  XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);

  // volumes
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVol, AC97_VOL_MID);
  XAC97_WriteReg(XPAR_AUDIO_CODEC_BASEADDR, AC97_MasterVolMono, AC97_VOL_MID);
  XAC97_AwaitCodecReady(XPAR_AUDIO_CODEC_BASEADDR);

  return XST_SUCCESS;
}

Xboolean is_valid(Sound *s)
{
  if (s->start == NULL || s->size == 0)
  {
    return XFALSE;
  }
  return XTRUE;
}

XStatus play_sound_background(Sound *s)
{
  Xuint32 sample;
  volatile Xuint32 *ptr;
  const Xuint32 *end_ptr = s->start + (s->size / 4);

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
  
  // Enable interrupt
  XAC97_Delay(1000);
  XAC97_mSetControl(XPAR_AUDIO_CODEC_BASEADDR, AC97_ENABLE_IN_FIFO_INTERRUPT);
  XAC97_Delay(1000);
  Xuint32 status = XAC97_mGetStatus(XPAR_AUDIO_CODEC_BASEADDR);
  Xuint32 inter = ((status >> 9) & 1);
  xil_printf("inter=%d\r\n", inter);

  /** Playing sound **/
  ptr = s->start;
  Xint32 i;
  do
  {
    for (i = 0; i < 512; i++)
	 {
      sample = *(ptr++);
		XAC97_WriteFifo(XPAR_AUDIO_CODEC_BASEADDR, sample);
    }
    while (!XAC97_isInFIFOEmpty(XPAR_AUDIO_CODEC_BASEADDR));
  } while (ptr < end_ptr);

  XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);
  return XST_SUCCESS;
}

XStatus stop_sound()
{
  XAC97_ClearFifos(XPAR_AUDIO_CODEC_BASEADDR);
  return XST_SUCCESS;
}
