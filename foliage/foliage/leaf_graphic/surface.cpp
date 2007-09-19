#include <iostream>
#include "../foliage.hpp"
#include "../settings.hpp"
#include "../memory.hpp"
#include "surface.hpp"
#include "screen.hpp"
#include "colors.hpp"

#ifdef __PPC__
	#include <sysace_stdio.h>
	#include <ivga.h>
#endif

Foliage::Surface::Surface(const Foliage::Size s, Foliage::Color *pixels, const std::string &name) 
	: _size(s), _pixels(pixels), _name(name), _instancized(-1)
{
}

Foliage::Surface::~Surface()
{
	delete _pixels;
}

const std::string &Foliage::Surface::getName() const
{
	return _name;
}

Foliage::Color Foliage::Surface::getPixel(const Foliage::Point p) const
{
	#ifdef SCREEN_VERTICAL
	return _pixels[_size.h - 1 - p.x + p.y * _size.h];
	#else
	return _pixels[p.y * _size.w + p.x];
	#endif
}

void Foliage::Surface::setPixel(const Foliage::Point p, const Foliage::Color color)
{
	#ifdef SCREEN_VERTICAL
	_pixels[_size.h - 1 - p.x + p.y * _size.h] = color;
	#else
	_pixels[p.y * _size.w + p.x] = color;
	#endif
}

void Foliage::Surface::drawAt(const Foliage::Point p) const
{
	if (_instancized != -1)
	{
		// try to use ivga
		bool ok = Foliage::Screen::instancizeSurface(this, p);
		if (!ok)
		{
			// use isprite
			Foliage::Screen::blitSurface(this, p);
		}
	}
	else if (_size.h <= 64 && _size.w <= 64)
	{
		// use isprite
		Foliage::Screen::blitSurface(this, p);
	}
	else
	{
		// draw manually (sloooooow!)
		Foliage::Screen::drawSurface(this, p);
	}
}
	
Foliage::Size Foliage::Surface::getSize() const
{
	return _size;
}

typedef struct BMP_HEADER
{
   Uint16 type;                     // Magic identifier
   Uint32 size;                     // File size in bytes
   Uint16 reserved1, reserved2;     // --
   Uint32 offset;                   // Offset to image data, bytes
} HEADER;

typedef struct BMP_INFOHEADER
{
   Uint32 size;                     // Header size in bytes
   Sint32 width, height;            // Width and height of image
   Uint16 planes;                   // Number of color planes
   Uint16 bits;                     // Bits per pixel
   Uint32 compression;              // Compression type
   Uint32 imagesize;                // Image size in bytes
   Sint32 xresolution, yresolution; // Pixels per meter
   Uint32 ncolors;                  // Number of colors
   Uint32 importantcolors;          // Important colors
} INFOHEADER;

// reads two bytes from file
inline Uint16 ReadUShort(SYSACE_FILE *fptr)
{
	Uint8 readBuffer[2];
	sysace_fread(readBuffer, 1, 2, fptr);
	return ((readBuffer[1] << 8) | readBuffer[0]);
}

// reads four bytes from file in little endian order
inline Uint32 ReadUIntLil(SYSACE_FILE *fptr)
{
	Uint8 readBuffer[4];	
	sysace_fread(readBuffer, 1, 4, fptr);
	return ((((((readBuffer[3] << 8) | readBuffer[2]) << 8) | readBuffer[1]) << 8) | readBuffer[0]);
}

inline void PadToNextMultipleOfFour(Sint32 &val)
{
	if (val % 4 == 1)
	{
		val += 3;
	}
	else if (val % 4 == 2)
	{
		val += 2;
	}
	else if (val % 4 == 3)
	{
		val++;
	}
}

Foliage::Surface *Foliage::Surface::readBMP(const std::string &filename)
{
	SYSACE_FILE *infile = sysace_fopen(filename.c_str(), "r");
	if (infile == NULL)
	{
		std::cout << "Could not open file " << filename << std::endl;
		return NULL;
	}
	
	// Read check the headers
	HEADER header;
	INFOHEADER infoheader;
	header.type = ReadUShort(infile);
	header.size = ReadUIntLil(infile);
	header.reserved1 = ReadUShort(infile);
	header.reserved2 = ReadUShort(infile);
	header.offset = ReadUIntLil(infile);
	infoheader.size = ReadUIntLil(infile);
	infoheader.width = ReadUIntLil(infile);
	infoheader.height = ReadUIntLil(infile);
	infoheader.planes = ReadUShort(infile);
	infoheader.bits = ReadUShort(infile);
	infoheader.compression = ReadUIntLil(infile);
	infoheader.imagesize = ReadUIntLil(infile);
	infoheader.xresolution = ReadUIntLil(infile);
	infoheader.yresolution = ReadUIntLil(infile);
	infoheader.ncolors = ReadUIntLil(infile);
	infoheader.importantcolors = ReadUIntLil(infile);
	
	// Check maximum buffer width
	if (infoheader.width > 1024)
	{
		std::cout << "Cannot read BMP with width > 1024." << std::endl;
		return NULL;
	}
	
	// Compute line (or column) padding on 4 pixels (due to 64-bit PLB)
	Sint32 width = infoheader.width;
	Sint32 height = infoheader.height;
	#ifdef SCREEN_VERTICAL
	PadToNextMultipleOfFour(height);
	#else
	PadToNextMultipleOfFour(width);
	#endif

	// Allocate surface memory
	Color *baseaddr = new(Foliage::Eternal) Color[height * width];
	if (baseaddr == NULL)
	{
		std::cout << "Not enough memory." << std::endl;
		sysace_fclose(infile);
		return NULL;
	}

	// Read raw 24-bit data, transform them to system colors, and write them in memory
	Uint8 readBuffer[1024*3];
	Sint32 linewidth = infoheader.width * 3;
	PadToNextMultipleOfFour(linewidth);
	for (Sint32 j = infoheader.height - 1; j >= 0; j--)
	{
		sysace_fread(readBuffer, 1, linewidth, infile);
		for (Sint32 i = 0; i < infoheader.width; i++)
		{
			Uint32 temp = ((((readBuffer[i*3+2] << 8) | readBuffer[(i*3)+1]) << 8) | readBuffer[(i*3)]);
			Color *writeaddr = baseaddr;
			#ifdef SCREEN_VERTICAL
			// If TATE mode, transform the line data to column data
			writeaddr += (i * height + (height - 1 - j));
			#else
			writeaddr += (j * width + i);
			#endif
			*writeaddr = ivga_create_color_from_truecolor(temp);
		}
	}
	sysace_fclose(infile);
	
	// Add transparent padding
	#ifdef SCREEN_VERTICAL
	for (Sint32 j = infoheader.height; j < height; j++)
	{
		for (Sint32 i = 0; i < infoheader.width; i++)
		{
			Color *writeaddr = baseaddr;
			writeaddr += (i * height + (height - 1 - j));
			*writeaddr = Foliage::Colors::Transparent;
		}
	}
	#else
	for (Sint32 j = 0; j < height; j++)
	{
		for (Sint32 i = infoheader.width; i < width; i++)
		{
			Color *writeaddr = baseaddr;
			writeaddr += (j * infoheader.width + i);
			*writeaddr = Foliage::Colors::Transparent;
		}
	}
	#endif
		
	// Flush processor cache
	Foliage::flushDCache();
	
	// Allocates and create the surface
	return new(Foliage::Eternal) Surface(Foliage::Surface(Foliage::Size(width, height), baseaddr, filename));
}

Foliage::Surface *Foliage::Surface::createNewShiftedSurface(const Sint32 shift) const
{
	// Allocate surface memory
	Foliage::Color *baseaddr = new(Foliage::Eternal) Foliage::Color[_size.w * _size.h];
	if (baseaddr == NULL)
	{
		std::cout << "Not enough memory." << std::endl;
		return NULL;
	}
	
	// Compute the shifted surface
	#ifdef SCREEN_VERTICAL
	for (Sint32 j = shift; j < _size.h; j++)
	{
		for (Sint32 i = 0; i < _size.w; i++)
		{
			Foliage::Color *writeaddr = baseaddr;
			writeaddr += (i * _size.h + (_size.h - 1 - j));
			*writeaddr = _pixels[(i * _size.h + (_size.h - 1 - (j - shift)))];
		}
	}
	#else
	for (Sint32 j = 0; j < _size.h; j++)
	{
		for (Sint32 i = shift; i < _size.w; i++)
		{
			Foliage::Color *writeaddr = baseaddr;
			writeaddr += (j * width + i);
			*writeaddr = _pixels[(j * _size.w + (i - shift))];
		}
	}
	#endif
	
	// Flush processor cache
	Foliage::flushDCache();
	
	// Allocates and create the surface
	std::string newname = _name;
	newname += '+';
	newname += ('0' + shift);
	return new(Foliage::Eternal) Foliage::Surface(_size, baseaddr, newname);
}
