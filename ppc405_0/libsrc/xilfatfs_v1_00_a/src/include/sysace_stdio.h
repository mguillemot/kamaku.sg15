/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002-2004 Xilinx, Inc.  All rights reserved.
//
// Xilinx, Inc.
//
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.  
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS FOR A PARTICULAR PURPOSE.
/////////////////////////////////////////////////////////////////////////////

 
#ifndef _SYSACE_STDIO_H_
#define _SYSACE_STDIO_H_


#define SYSACE_FILE   void 
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// IF YOU WANT TO USE fopen instead of sysace_fopen, 
// and fclose, fread and fwrite instead of the corresponding
// sysace_ prefixed function names uncomment the following few lines
// and comment out the preceding two lines of code
// WARNING: This will cause link time failures for functions
// such as printf and scanf that depend on standard definitions of fwrite/fread
// so use with caution
// #define SYSACE_FILE    FILE
// typedef void FILE;
// 
// #define fopen  sysace_fopen
// #define fclose sysace_fclose
// #define fread  sysace_fread
// #define fwrite sysace_fwrite


SYSACE_FILE *sysace_fopen(const char *file, const char *mode );
int sysace_fclose(SYSACE_FILE *stream );
int sysace_fread(void *buffer, int size, int count, SYSACE_FILE *stream );
int sysace_fwrite(void *buffer, int size, int count, SYSACE_FILE *stream );
int sysace_mkdir(const char *path);
int sysace_chdir(const char *path);


#ifdef __cplusplus
}
#endif

#endif
