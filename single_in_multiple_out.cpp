// ==========================================================
// Batch loader
//
// Design and implementation by 
// - Floris van den Berg
// - Herv� Drolon
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at own risk!
// ==========================================================

//
//  This example shows how to easily batch load a directory
//  full of images. Because not all formats can be identified
//  by their header (some images don't have a header or one
//  at the end of the file) we make use of the
//  FreeImage_GetFIFFromFilename function. This function
//  receives a file name, for example 'myfile.bmp', and returns
//  a FREE_IMAGE_TYPE enum which identifies that bitmap.
//
//  Functions used in this sample : 
//  FreeImage_GetFileType, FreeImage_GetFIFFromFilename, FreeImage_FIFSupportsReading, 
//  FreeImage_Load, FreeImage_GetBPP, FreeImage_FIFSupportsWriting, FreeImage_GetFormatFromFIF
//  FreeImage_FIFSupportsExportBPP, FreeImage_Save, FreeImage_Unload,
//  FreeImage_SetOutputMessage, FreeImage_GetVersion, FreeImage_GetCopyrightMessage
//
// ==========================================================

#include <assert.h>
#include <stdio.h>
//#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "FreeImage.h"

// ----------------------------------------------------------

/** Generic image loader
	@param lpszPathName Pointer to the full file name
	@param flag Optional load flag constant
	@return Returns the loaded dib if successful, returns NULL otherwise
*/
FIBITMAP* GenericLoader(const char* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(lpszPathName, 0);
	if(fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// ok, let's load the file
		FIBITMAP *dib = FreeImage_Load(fif, lpszPathName, flag);
		// unless a bad file format, we are done !
		return dib;
	}
	return NULL;
}

/** Generic image writer
	@param dib Pointer to the dib to be saved
	@param lpszPathName Pointer to the full file name
	@param flag Optional save flag constant
	@return Returns true if successful, returns false otherwise
*/
bool GenericWriter(FIBITMAP* dib, const char* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	BOOL bSuccess = FALSE;

	if(dib) {
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
		if(fif != FIF_UNKNOWN ) {
			// check that the plugin has sufficient writing and export capabilities ...
			WORD bpp = FreeImage_GetBPP(dib);
			if(FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)) {
				// ok, we can save the file
				bSuccess = FreeImage_Save(fif, dib, lpszPathName, flag);
				// unless an abnormal bug, we are done !
			}
		}
	}
	return (bSuccess == TRUE) ? true : false;
}

// ----------------------------------------------------------

/**
	FreeImage error handler
	@param fif Format / Plugin responsible for the error 
	@param message Error message
*/
void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
	printf("\n*** "); 
	if(fif != FIF_UNKNOWN) {
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	printf("%s", message);
	printf(" ***\n");
}

// ----------------------------------------------------------

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

int 
main(int argc, char *argv[]) {

	FIBITMAP *dib = NULL;

	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif // FREEIMAGE_LIB

	// initialize your own FreeImage error handler

	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	// print version & copyright infos

	printf("%s", FreeImage_GetVersion());
	printf("\n");
	printf("%s", FreeImage_GetCopyrightMessage());
	printf("\n");

	// open the log file

	FILE *log_file = fopen("log_file.txt", "w");

	char *directory = new char[MAX_PATH];
	strcpy(directory, argv[1]);


	int num_formats = 11;
	int file_fmt[] = {
			FIF_IFF,
			TARGA_DEFAULT,
			PSD_DEFAULT,
			JXR_DEFAULT,
			WEBP_DEFAULT,
			TIFF_DEFAULT,
			JPEG_DEFAULT,
			EXR_DEFAULT,
			J2K_DEFAULT,
			JP2_DEFAULT,
			PNM_DEFAULT
	};
	const char* file_ext[] = {
			".iff",
			".tga",
			".psd",
			".jxr",
			".webp",
			".tiff",
			".jpeg",
			".exr",
			".j2k",
			".jp2",
			".pnm"
	};

	int i;
	for(i = 0; i < num_formats; i++){
		// make a unique filename
		char *unique = new char[128];
		snprintf(unique, sizeof(unique), "%d", i);
		strcat(unique, file_ext[i]);

		// open and load the file using the default load option
		dib = GenericLoader(directory, 0);

		if (dib != NULL) {
			// save the file as PNG
			bool bSuccess = GenericWriter(dib, unique, file_fmt[i]);

			// free the dib
			FreeImage_Unload(dib);

			if(bSuccess) {
				fwrite(unique, strlen(unique), 1, log_file);
			} else {
				strcpy(unique, "FAILED");
				fwrite(unique, strlen(unique), 1, log_file);
			}
			fwrite(" >> ", 4, 1, log_file);
			fwrite(directory, strlen(directory), 1, log_file);
			fwrite("\n", 1, 1, log_file);
		}

		delete [] unique;
	}

	delete [] directory;


	fclose(log_file);

	// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif // FREEIMAGE_LIB

	return 0;
}
