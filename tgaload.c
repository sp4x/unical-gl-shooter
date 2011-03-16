/*------------------------------------------------------------------------------
* Alpha Shooter
* A 3D OpenGL First-Person Shooter game
* Copyright (C) 2005, 2006, 2007 Nicola Cocchiaro <ncocchiaro@users.sf.net>
* Alpha Shooter can be found at http://www.sf.net/projects/alphashooter.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 or, (at your option) any later
* version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*-------------------------------------------------------------------------------
*/

// tgaload.c
// Loader for TGA image files
// (only RGB images are currently supported, RLE or not)

#include "tgaload.h"
#include <errno.h>
#include <string.h>

#define SWAP_2B(h,l,s)	s|=h;s=s<<16;s|=l;

/* loads a TGA image file returning a pointer to it */
tImageTGA *loadTGA(const char *filename, int *status)
{
	tImageTGA *pImageData = NULL;
	FILE *pFile = NULL;
	// TGA header fields	
	unsigned short width = 0, height = 0;
	unsigned char length = 0;
	unsigned char colormaptype = 0;
	unsigned char imageType = 0;
	unsigned char bits = 0;
	unsigned short cmap_num = 0;
	unsigned char cmap_bits = 0;
	unsigned char channels = 0;
	// stride = channels * width
	int stride = 0;
	int i = 0, y = 0, res = 0;
	unsigned char *pLine = NULL, temp = 0;
#ifdef __BIG_ENDIAN
	unsigned char width_hi = 0, width_lo = 0, height_hi = 0, height_lo = 0;
	unsigned char cmap_hi = 0, cmap_lo = 0;
#endif


	// sanity check
	if ((!filename) || (!status))
	{
		if (status)
			*status = -EINVAL;
		fprintf(stderr, "Error: Invalid parameters in TGAload: filename is %p, status is %p\n", filename, status);
		return NULL;
	}
	
	// open file, rewind file pointer for safety
#ifndef WIN32
	if ((pFile = fopen(filename, "r")) == NULL)
#else
	if ((pFile = fopen(filename, "rb")) == NULL)
#endif
	{
		*status = -errno;
		//fprintf(stderr, "Error: Unable to open file %s: %s\n", filename, strerror(errno));
		return NULL;
	}
	if (fseek(pFile, 0L, SEEK_SET) != 0)
	{
		*status = -errno;
		fprintf(stderr, "Error: Unable to rewind file pointer for %s: %s\n", filename, strerror(errno));
		fclose(pFile);
		return NULL;
	}
		
	pImageData = (tImageTGA*)malloc(sizeof(tImageTGA));
	if (pImageData == NULL)
	{
		*status = -errno;
		fprintf(stderr, "Error: Unable to allocate memory for image data structure while loading TGA file %s: %s\n", filename, strerror(errno));
		fclose(pFile);
		return NULL;
	}
	memset(pImageData, 0, sizeof(tImageTGA));

	// Read in the length of the image identification field
	res = fread(&length, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	
	// Read in the color map type (0 for RGB images)
	// (color-mapped images are currently not supported)
	res = fread(&colormaptype, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	if (colormaptype != 0)
	{
		*status = -EINVAL;
		fprintf(stderr, "Error: Image %s is color-mapped, must use an RGB image.\n", filename);
		fclose(pFile);
		free(pImageData);
		return NULL;
	}
	
	// Jump over one byte
//	fseek(pFile, 1, SEEK_CUR); 

	// Read in the imageType (RLE, RGB, etc...)
	res = fread(&imageType, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);

	// Skip past general information we don't care about
	// (color map origin)
	//if (fseek(pFile, 2, SEEK_CUR) != 0)
	if (fseek(pFile, 5, SEEK_SET) != 0)
	{
		*status = -errno;
		fprintf(stderr, "Error: Could not fseek in file %s: %s\n", filename, strerror(errno));
		fclose(pFile);
		free(pImageData);
		return NULL;
	}
	
	// Testing for spurious color map information in RGB file
	// (color map length and entry size in bits)

#ifndef __BIG_ENDIAN
	res = fread(&cmap_num, 2, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
#else
	res = fread(&cmap_lo, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	res = fread(&cmap_hi, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	SWAP_2B(cmap_hi, cmap_lo, cmap_num);
#endif
	res = fread(&cmap_bits, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);

	// Skip X and Y origin of the image
	//if (fseek(pFile, 4, SEEK_CUR) != 0)
	if (fseek(pFile, 12, SEEK_SET) != 0)
	{
		*status = -errno;
		fprintf(stderr, "Error: Could not fseek in file %s: %s\n", filename, strerror(errno));
		fclose(pFile);
		free(pImageData);
		return NULL;
	}
	
#ifndef __BIG_ENDIAN
	// Read the width, height and bits per pixel (16, 24 or 32)
	res = fread(&width, 2, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	res = fread(&height, 2, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
#else
	res = fread(&width_lo, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	res = fread(&width_hi, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	SWAP_2B(width_hi, width_lo, width);
	res = fread(&height_lo, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	res = fread(&height_hi, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);
	SWAP_2B(height_hi, height_lo, height);
#endif

	res = fread(&bits, 1, 1, pFile);
	CHECK_READ_ERR(filename, res, pFile);

	// debug prints
	//fprintf(stderr, "\n%s: len %d, cmap_type: %d, imgtype: %d, cmap_num: %d, cmap_bits: %d\n", filename, length, colormaptype, imageType, cmap_num, cmap_bits);
	//fprintf(stderr, "%s: width %d, height %d, bits %d\n", filename, width, height, bits);

	// Now we move the file pointer to the pixel data
	// skipping what we've read so far, then image descriptor byte and Image Identification Field
	fseek(pFile, 17 + length + 1 + (cmap_num * (cmap_bits >> 3)), SEEK_SET); 

	// Check if the image is RLE compressed or not
	if (imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if (bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits >> 3;
			stride = ((short)channels) * width;
			pImageData->data = (unsigned char*)malloc(stride*((int)height));
			if (pImageData->data == NULL)
			{
				*status = -errno;
				fprintf(stderr, "Error: Unable to allocate memory for TGA image data for %s: %s\n", filename, strerror(errno));
				fclose(pFile);
				free(pImageData);
				return NULL;
			}
			memset(pImageData->data, 0, stride*((int)height));

			// Load in all the pixel data line by line
			for (y = 0; y < height; y++)
			{
				pLine = &(pImageData->data[stride * y]);

				// Read in the current line of pixels
				res = fread(pLine, stride, 1, pFile);
				CHECK_READ_ERR(filename, res, pFile);
			
				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB
				for (i = 0; i < stride; i += (int)channels)
				{
					temp = pLine[i];
					pLine[i] = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if (bits == 16)
		{
			unsigned short pixels = 0;
			int r = 0, g = 0, b = 0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = ((short)channels) * width;
			pImageData->data = (unsigned char*)malloc((size_t)(stride * ((int)height)));
			if (pImageData->data == NULL)
			{
				*status = -errno;
				fprintf(stderr, "Error: Unable to allocate memory for TGA image data for %s: %s\n", filename, strerror(errno));
				fclose(pFile);
				free(pImageData);
				return NULL;
			}
			memset(pImageData->data, 0, (size_t)(stride * ((int)height)));

			// Load in all the pixel data pixel by pixel
			for(i = 0; i < (int)(width*height); i++)
			{
				// Read in the current pixel
				res = fread(&pixels, 2, 1, pFile);
				CHECK_READ_ERR(filename, res, pFile);
				
				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}	
		// Else return a NULL for a bad or unsupported pixel format
		else
		{
			*status = -EINVAL;
			fprintf(stderr, "Error: Bad or unsupported pixel format in file %s.\n", filename);
			fclose(pFile);
			free(pImageData);
			return NULL;
		}
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		unsigned char rleID = 0;
		int colorsRead = 0;
		unsigned char *pColors;
		
		channels = bits >> 3;
		stride = ((short)channels) * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->data = (unsigned char*)malloc((size_t)(stride * ((int)height)));
		if (pImageData->data == NULL)
		{
			*status = -errno;
			fprintf(stderr, "Error: Unable to allocate memory for TGA image data for %s: %s\n", filename, strerror(errno));
			fclose(pFile);
			free(pImageData);
			return NULL;
		}
		memset(pImageData->data, 0, (size_t)(stride * ((int)height)));
		
		pColors = (unsigned char*)malloc((size_t)channels);
		if (pColors == NULL)
		{
			*status = -errno;
			fprintf(stderr, "Error: Unable to allocate memory for image colors for %s: %s\n", filename, strerror(errno));
			fclose(pFile);
			free(pImageData->data);
			free(pImageData);
			return NULL;
		}
		memset(pImageData->data, 0, (size_t)channels);

		// Load in all the pixel data
		while (i < (int)(width*height))
		{
			// Read in the current color count + 1
			res = fread(&rleID, 1, 1, pFile);
			CHECK_READ_ERR(filename, res, pFile);
			
			// Check if we don't have an encoded string of colors
			if (rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while (rleID)
				{
					// Read in the current color
					res = fread(pColors, channels, 1, pFile);
					CHECK_READ_ERR(filename, res, pFile);

					// Store the current pixel in our image array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += (int)channels;
				}
			}
			// Else, let's read in a string of the same character
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				res = fread(pColors, channels, 1, pFile);
				CHECK_READ_ERR(filename, res, pFile);

				// Go and read as many pixels as are the same
				while (rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if (bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += (int)channels;
				}
				
			}
				
		}
		free(pColors);
	}

	fclose(pFile);

	pImageData->channels = channels;
	pImageData->sizeX = width;
	pImageData->sizeY = height;

	*status = 0;
	return pImageData;
}
