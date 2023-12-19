#ifndef BITMAP_H
#define BITMAP_H

#include <cstdint>
#include <cstddef>

// reference https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
// reference https://www.setsuki.com/hsp/ext/bmp.htm
// reference https://imagingsolution.net/imaging/imaging-programing/bitmap-file-format/
typedef struct {
    uint8_t bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMP_BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPixPerMeter;
    uint32_t biYPixPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMP_BITMAPINFOHEADER;

typedef struct {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
} BMP_COLOR;

typedef struct {
    BMP_BITMAPFILEHEADER fileHeader;
    BMP_BITMAPINFOHEADER infoHeader;

    size_t nPalette;
    BMP_COLOR *palette;

    uint8_t *data;
    size_t dataSize;
} BitmapFile;

int load_bmp_file(char *path, BitmapFile *file);
void release_bmp_file(BitmapFile *file);
int save_bmp_file(char *path, BitmapFile *file);
void initialize_bmp_file(BitmapFile *file, size_t width, size_t height);

#endif
