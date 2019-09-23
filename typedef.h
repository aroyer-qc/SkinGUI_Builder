#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <QImage>
#include <stdint.h>

#define MINIMUM_PREVIEW_WIDTH   480
#define MINIMUM_PREVIEW_HEIGHT  272

#define FORMAT_RGB565           0
#define FORMAT_ARGB8888         1
#define FORMAT_FILE_DATA        2

#define IMAGE_HEADER_SIZE       18

#define SIZE_OF_ID_CODE_SPACE   10000


#define SAMPLING_ALPHA          uint8_t(0x01)
#define SAMPLING_NUMERIC        uint8_t(0x02)
#define SAMPLING_SYMBOL         uint8_t(0x04)
#define SAMPLING_EXTRA_SYMBOL   uint8_t(0x08)
#define SAMPLING_LATIN          uint8_t(0x10)
#define SAMPLING_FIXED_NUMERIC  uint8_t(0x20)
#define SAMPLING_DEFAULT_FONT   uint8_t(0x27)


#define SAMPLING_BOX_X_SIZE     230
#define SAMPLING_BOX_Y_SIZE     200



typedef QSize QOffset;

typedef enum Endianess_e
{
    LITTLE_ENDIAN = 0,
    BIG_ENDIAN,
} eEndianess;

typedef enum Resizer_e
{
    RELOAD_FORMAT = 0,
    AUTO_FORMAT,
} eResizer;

typedef enum Caller_e
{
    CALLER_IMAGE = 0,
    CALLER_BUTTON,
    CALLER_ICON,
} eCaller;

typedef enum Scale_e
{
    SCALE_NONE = 0,
    SCALE_FIT,
    SCALE_WIDTH,
    SCALE_HEIGHT,
} eScaler;

typedef enum GFX_PixelFormat_e
{
   ARGB8888,
   RGB888,
   RGB565,
   ARGB1555,
   ARGB4444,
   L8,
   AL44,
   AL88,
   L4,
   A8,
   A4,
   DUMMY_PIXEL_FORMAT,
} GFX_ePixelFormat;

typedef enum Compression_e
{
    COMPRESSION_NONE,
    RLE_4,                                  // 4 bits per pixel  (font, or grayscale)
    RLE_8,                                  // 8 bits per pixel  (font, or grayscale)
    RLE_16,                                 // 16 bits per pixel (RGB565)
    RLE_32,                                 // 32 bits per pixel (ARGB8888)
    RLE_16_CLUT,                            // 16 bits per pixel (RGB565, with color table)
    RLE_32_CLUT,                            // 32 bits per pixel (ARGB8888, with color table);
    LZW_12,                                 // Good for all
    LZW_13,                                 // Good for all
    LZW_14,                                 // Good for all
    LZW_15,                                 // Good for all
    LZW_16,                                 // Good for all
    JPEG,                                   // If the file is already in JPEG and smaller than other method
} eCompression;

typedef enum WidgetType_e
{
    TYPE_BACKGROUND,
    TYPE_BUTTON,
    TYPE_FONT,
    TYPE_GLYPH,
    TYPE_ICON,
   TYPE_IMAGE,
   TYPE_AUDIO,
} eWidgetType;

#pragma pack (1)
typedef struct ImageInfo_s
{
    uint32_t        ID;
    uint32_t        RawIndex;
    size_t          CompressDataSize;
    size_t          DataSize;
    QSize           Size;
    QImage::Format  PixelFormat;
    QString         Filename;
    QString         Description;
} sImageInfo;
#pragma pack (4)

#pragma pack (1)
typedef struct AudioInfo_s
{
    uint32_t        ID;
    uint32_t        RawIndex;
    size_t          DataSize;
    QSize           Size;
    time_t          Duration;
    QString         Filename;
    QString         Description;
} sAudioInfo;
#pragma pack (4)

// Struct for data in skin file.
typedef struct SkinInfoData_s
{
        uint32_t        ID;
        uint32_t        DataSize;
        uint8_t         Compression;
    #pragma pack (1)
    union
    {
        struct
        {
            uint16_t        width;
            uint16_t        height;
            uint8_t         PixelFormat;
        } Image;
        struct
        {
            uint8_t         Code;
            uint8_t         Page;
        } Font;
        struct
        {
            uint8_t         LabelCount;
        };
    }u;
    #pragma pack (4)
} sSkinInfoData;


typedef struct LoadingInfo_s
{
    QString         PathAndFilename;
    QString         Filename;
    size_t          DataSize;
    QSize           Size;
    QPoint          Offset;
    eScaler         ScaleType;
    QImage::Format  PixelFormat;
} sLoadingInfo;

typedef struct Encoding_s
{
    eCompression Type;
    int          Size;


} sEncoding;

#endif
