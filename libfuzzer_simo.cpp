#include <assert.h>
#include <stdio.h>
//#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "FreeImage.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    FIMEMORY *mem;
    FIBITMAP *dib;
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    BOOL bSuccess = FALSE;

    FreeImage_Initialise(true);

    mem = FreeImage_OpenMemory(const_cast<uint8_t *>(data), size);
    if (!mem)
        return 0;

    dib = FreeImage_LoadFromMemory(FIF_TIFF, mem, TIFF_DEFAULT);

  	if(dib) {
  		// try to guess the file format from the file extension
  		fif = FreeImage_GetFileTypeFromMemory(mem);
  		if(fif != FIF_UNKNOWN ) {
  			// check that the plugin has sufficient writing and export capabilities ...
  			WORD bpp = FreeImage_GetBPP(dib);
  			if(FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp)) {
  				// ok, we can save the file
  				bSuccess = FreeImage_Save(fif, dib, "testfile", 0);
  				// unless an abnormal bug, we are done !
  			}
  		}
  	}

    FreeImage_Unload(dib);
    FreeImage_CloseMemory(mem);
    FreeImage_DeInitialise();

    return 0;
}
