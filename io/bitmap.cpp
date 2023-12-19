#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "bitmap.hpp"


int load_file_header(FILE *fp, BitmapFile *file) 
{
    size_t n = fread(&file->fileHeader.bfType[0], sizeof(uint8_t), 2, fp);
    if (n < 2) {
        return -1;
    }

    if (strcmp((const char *) file->fileHeader.bfType, "BM") != 0) {
        return -1;
    }

    n = fread(&file->fileHeader.bfSize, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&file->fileHeader.bfReserved1, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&file->fileHeader.bfReserved2, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&file->fileHeader.bfOffBits, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    return 0;
}

int load_info_header(FILE *fp, BitmapFile *file) 
{
    BMP_BITMAPINFOHEADER *info_header = &file->infoHeader;

    size_t n = fread(&info_header->biSize, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }
    if (info_header->biSize != 40) {
        // OS/2 形式は対応しない
        return -1;
    }

    n = fread(&info_header->biWidth, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biHeight, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biPlanes, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biBitCount, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biCompression, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }
    if (info_header->biCompression != 0) {
        // 圧縮は対応しない
        return -1;
    }

    n = fread(&info_header->biSizeImage, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biXPixPerMeter, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biYPixPerMeter, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biClrUsed, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fread(&info_header->biClrImportant, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    return 0;
}

int load_color_palette(FILE *fp, BitmapFile *file) 
{
    uint16_t bit_count = file->infoHeader.biBitCount;
    if (24 <= bit_count && bit_count <= 32) {
        // 未圧縮を仮定している
        file->palette = NULL;
        file->nPalette = 0;
        return 0;
    } else if (32 <= bit_count) {
        return -1;
    }

    uint32_t n_palette = file->infoHeader.biClrUsed;
    if (n_palette == 0) {
        n_palette = 1u << bit_count;
    }

    file->palette = (BMP_COLOR *)malloc(sizeof(BMP_COLOR) * n_palette);
    if (file->palette == NULL) {
        return -1;
    }
    file->nPalette = n_palette;

    for(int i = 0; i < n_palette; i++) {
        size_t n = fread(&file->palette[i].rgbBlue, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fread(&file->palette[i].rgbGreen, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fread(&file->palette[i].rgbRed, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fread(&file->palette[i].rgbReserved, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
    }
    return 0;
}

typedef struct {
    size_t stride;
    size_t padding;
    size_t original_size;
} OriginalSize;

OriginalSize calc_original_size(BMP_BITMAPINFOHEADER *info)
{
    // reference: https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader#-------------
    uint16_t bit_per_pixel = info->biBitCount;

    size_t stride = (((info->biWidth * info->biBitCount) + 31) & ~31) >> 3;

    size_t padding = (stride * 8 - info->biWidth * bit_per_pixel) / 8;

    size_t original_size = stride * info->biHeight;
    OriginalSize result = {stride, padding, original_size};
    return result;
}

void initialize_bmp_file(BitmapFile *file, size_t width, size_t height) 
{
    BMP_BITMAPFILEHEADER *fileHeader = &file->fileHeader;
    fileHeader->bfType[0] = 'B';
    fileHeader->bfType[1] = 'M';
    fileHeader->bfReserved1 = 0;
    fileHeader->bfReserved2 = 0;
    fileHeader->bfOffBits = 54;
    BMP_BITMAPINFOHEADER *infoHeader = &file->infoHeader;
    infoHeader->biSize = 40;
    infoHeader->biWidth = width;
    infoHeader->biHeight = height;
    infoHeader->biPlanes = 1;
    infoHeader->biBitCount = 24;
    infoHeader->biCompression = 0;
    infoHeader->biSizeImage = 0;
    infoHeader->biXPixPerMeter = 0;
    infoHeader->biYPixPerMeter = 0;
    infoHeader->biClrUsed = 0;
    infoHeader->biClrImportant = 0;

    file->nPalette = 0;
    file->palette = 0;

    file->dataSize = width * height * infoHeader->biBitCount / 8;
    file->data = (uint8_t *) malloc(sizeof(uint8_t) * file->dataSize);
    memset(file->data, 255, file->dataSize);

    OriginalSize osize = calc_original_size(infoHeader);
    fileHeader->bfSize = 54 + osize.original_size;
}

void release_bmp_file(BitmapFile *file) 
{
    if(file->data != NULL) {
        free(file->data);
    }

    if(file->palette != NULL) {
        free(file->data);
    }
}

int load_bmp_data(FILE *fp, BitmapFile *file)
{
    BMP_BITMAPINFOHEADER *info = &file->infoHeader;
    size_t width = info->biWidth;
    size_t height = info->biHeight;

    uint16_t bit_per_pixel = info->biBitCount;
    if (bit_per_pixel % 8 != 0) {
        // 1ピクセルが1byteより小さい形式は対応しない。
        return -1;
    }
    if (bit_per_pixel == 16) {
        // 1ピクセルあたり2bytes使う場合のファイルフォーマットが不明なので対応しない。
        return -1;
    }
    assert(bit_per_pixel == 8 || bit_per_pixel == 24 || bit_per_pixel == 32);

    OriginalSize osize = calc_original_size(info);
    uint8_t *data = (uint8_t *) malloc(sizeof(uint8_t) * osize.original_size);
    if (data == NULL) {
        return -1;
    }
    size_t n = fread(data, 1, osize.original_size, fp);
    if (n < osize.original_size) {
        return -1;
    }

    size_t size = osize.original_size - osize.padding * height;
    uint8_t *image = (uint8_t *) malloc(sizeof(uint8_t) * size);
    size_t bytes_per_width = width * bit_per_pixel / 8;
    size_t j = 0;
    for (size_t i = 0; i < osize.original_size; i+=osize.stride) {
        for(size_t k = 0; k < bytes_per_width; k++) {
            image[j + k] = data[i + k];
        }
        j += bytes_per_width;
    }

    file->data = image;
    file->dataSize = size;

    return 0;
}

int load_bmp_file(char *path, BitmapFile *file)
{
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }

    // preset for detection to free memory on error
    file->data = NULL;
    file->palette = NULL;

    if (load_file_header(fp, file) < 0) {
        fclose(fp);
        release_bmp_file(file);
        return -2;
    }

    if (load_info_header(fp, file) < 0) {
        fclose(fp);
        release_bmp_file(file);
        return -3;
    }

    if (load_color_palette(fp, file) < 0) {
        fclose(fp);
        release_bmp_file(file);
        return -4;
    }

    if (load_bmp_data(fp, file) < 0) {
        fclose(fp);
        release_bmp_file(file);
        return -5;
    }

    fclose(fp);
    return 0;
}


int save_file_header(FILE *fp, BitmapFile *file) 
{
    size_t n = fwrite(&file->fileHeader.bfType[0], sizeof(uint8_t), 2, fp);
    if (n < 2) {
        return -1;
    }

    n = fwrite(&file->fileHeader.bfSize, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&file->fileHeader.bfReserved1, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&file->fileHeader.bfReserved2, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&file->fileHeader.bfOffBits, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    return 0;
}

int save_info_header(FILE *fp, BitmapFile *file) 
{
    BMP_BITMAPINFOHEADER *info_header = &file->infoHeader;

    size_t n = fwrite(&info_header->biSize, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biWidth, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biHeight, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biPlanes, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biBitCount, sizeof(uint16_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biCompression, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biSizeImage, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biXPixPerMeter, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biYPixPerMeter, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biClrUsed, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    n = fwrite(&info_header->biClrImportant, sizeof(uint32_t), 1, fp);
    if (n < 1) {
        return -1;
    }

    return 0;
}

int save_color_palette(FILE *fp, BitmapFile *file) 
{
    uint16_t bit_count = file->infoHeader.biBitCount;
    if (24 <= bit_count && bit_count <= 32) {
        // 未圧縮を仮定している
        return 0;
    }
    if (32 < bit_count) {
        return -1;
    }

    uint32_t n_palette = file->infoHeader.biClrUsed;
    if (n_palette == 0) {
        n_palette = 1u << bit_count;
    }

    for(int i = 0; i < n_palette; i++) {
        size_t n = fwrite(&file->palette[i].rgbBlue, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fwrite(&file->palette[i].rgbGreen, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fwrite(&file->palette[i].rgbRed, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
        n = fwrite(&file->palette[i].rgbReserved, sizeof(uint8_t), 1, fp);
        if (n < 1) {
            return -1;
        }
    }
    return 0;
}

int save_bmp_data(FILE *fp, BitmapFile *file)
{
    BMP_BITMAPINFOHEADER *info = &file->infoHeader;
    size_t width = info->biWidth;
    size_t height = info->biHeight;

    uint16_t bit_per_pixel = info->biBitCount;
    // reference: https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader#-------------
    size_t stride = (((info->biWidth * info->biBitCount) + 31) & ~31) >> 3;
    if (bit_per_pixel % 8 != 0) {
        // 1ピクセル1byte以下の形式は対応しない。
        return -1;
    }

    size_t padding = (stride * 8 - width * bit_per_pixel) / 8;
    size_t bytes_per_width = width * bit_per_pixel / 8;
    assert(file->dataSize % bytes_per_width == 0);

    uint8_t *pad_zeros = NULL;
    if(padding > 0) {
        pad_zeros = (uint8_t *) malloc(sizeof(uint8_t) * padding);
        memset(&pad_zeros[0], 0, sizeof(uint8_t) * padding);
    }

    for (size_t i = 0; i < file->dataSize; i+=bytes_per_width) {
        size_t n = fwrite(&file->data[i], sizeof(uint8_t), bytes_per_width, fp);
        if (n < bytes_per_width) {
            return -1;
        }
        if (pad_zeros == NULL) {
            continue;
        }

        n = fwrite(&pad_zeros[0], sizeof(uint8_t), padding, fp);
        if (n < padding) {
            return -1;
        }
    }

    if (pad_zeros != 0) {
        free(pad_zeros);
    }

    return 0;
}

int save_bmp_file(char *path, BitmapFile *file)
{
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }

    if (save_file_header(fp, file) < 0) {
        fclose(fp);
        return -2;
    }

    if (save_info_header(fp, file) < 0) {
        fclose(fp);
        return -3;
    }

    if (save_color_palette(fp, file) < 0) {
        fclose(fp);
        return -4;
    }

    if (save_bmp_data(fp, file) < 0) {
        fclose(fp);
        return -5;
    }

    fclose(fp);
    return 0;
}
