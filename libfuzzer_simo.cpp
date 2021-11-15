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
    int length, height, bpp, y;

    FreeImage_Initialise(true);

    mem = FreeImage_OpenMemory(const_cast<uint8_t *>(data), size);
    if (!mem)
        return 0;

    dib = FreeImage_LoadFromMemory(FIF_TIFF, mem, TIFF_DEFAULT);
    if (!dib)
        return 0;

    bpp = FreeImage_GetBPP(dib);
    length = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);

    for (y = 0; y < height; y++)
        FreeImage_GetScanLine(dib, y);

    FreeImage_Unload(dib);
    FreeImage_CloseMemory(mem);
    FreeImage_DeInitialise();

    return 0;
}
