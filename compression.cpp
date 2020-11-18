/*
   Copyright(c) 2020 Alain Royer.
   Email: aroyer.qc@gmail.com

   Permission is hereby granted, free of charge, to any person obtaining a copy of this software
   and associated documentation files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all copies or
   substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
   AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "compression.h"
#include "Utility.h"

// ************************************************************************************************

uint8_t Compress(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    eCompression        Compression;                // Compression that will be selected
    int                 BestTotal;                  // Best total count reach from all selection
    QVector<uint8_t>    Data_RLE_4;                 // Compress data for each compression
    QVector<uint8_t>    Data_RLE_8;
    QVector<uint8_t>    Data_RLE_16;
    QVector<uint8_t>    Data_RLE_32;
    QVector<uint8_t>    Data_RLE_16_CLUT;
    QVector<uint8_t>    Data_RLE_32_CLUT;
    QVector<uint8_t>    Data_LZW_12;
    QVector<uint8_t>    Data_LZW_13;
    QVector<uint8_t>    Data_LZW_14;
    QVector<uint8_t>    Data_LZW_15;
    QVector<uint8_t>    Data_LZW_16;
    QVector<uint8_t>*   pData;
    lzw* pLZW_Method;

    // Try each compression method
    int TotalRLE_4       = CRLE_4_Method       (&Data_RLE_4,       pRawData, DataSize, DataOffset);
    int TotalRLE_8       = CRLE_8_Method       (&Data_RLE_8,       pRawData, DataSize, DataOffset);
    int TotalRLE_16      = CRLE_16_Method      (&Data_RLE_16,      pRawData, DataSize, DataOffset);
    int TotalRLE_32      = CRLE_32_Method      (&Data_RLE_32,      pRawData, DataSize, DataOffset);
    int TotalRLE_16_CLUT = CRLE_16_CLUT_Method (&Data_RLE_16_CLUT, pRawData, DataSize, DataOffset);
    int TotalRLE_32_CLUT = CRLE_32_CLUT_Method (&Data_RLE_32_CLUT, pRawData, DataSize, DataOffset);

    pLZW_Method = new lzw(12);
    int TotalLZW_12      = pLZW_Method->Compress(&Data_LZW_12,     pRawData, DataSize, DataOffset);
    delete pLZW_Method;
    pLZW_Method = new lzw(13);
    int TotalLZW_13      = pLZW_Method->Compress(&Data_LZW_13,     pRawData, DataSize, DataOffset);
    delete pLZW_Method;
    pLZW_Method = new lzw(14);
    int TotalLZW_14      = pLZW_Method->Compress(&Data_LZW_14,     pRawData, DataSize, DataOffset);
    delete pLZW_Method;
    pLZW_Method = new lzw(15);
    int TotalLZW_15      = pLZW_Method->Compress(&Data_LZW_15,     pRawData, DataSize, DataOffset);
    delete pLZW_Method;
    pLZW_Method = new lzw(16);
    int TotalLZW_16      = pLZW_Method->Compress(&Data_LZW_16,     pRawData, DataSize, DataOffset);
    delete pLZW_Method;

    // Select the best compression method
    BestTotal = DataSize;
    Compression = COMPRESSION_NONE;

    if((TotalRLE_4 < BestTotal) && (TotalRLE_4 != 0))
    {
        BestTotal = TotalRLE_4;
        Compression = RLE_4;
    }
    if((TotalRLE_8 < BestTotal) && (TotalRLE_8 != 0))
    {
        BestTotal   = TotalRLE_8;
        Compression = RLE_8;
    }
    if((TotalRLE_16 < BestTotal) && (TotalRLE_16 != 0))
    {
        BestTotal   = TotalRLE_16;
        Compression = RLE_16;
    }
    if((TotalRLE_32 < BestTotal) && (TotalRLE_32 != 0))
    {
        BestTotal   = TotalRLE_32;
        Compression = RLE_32;
    }
    if((TotalRLE_16_CLUT < BestTotal) && (TotalRLE_16_CLUT != 0))
    {
        BestTotal   = TotalRLE_16_CLUT;
        Compression = RLE_16_CLUT;
    }
    if((TotalRLE_32_CLUT < BestTotal) && (TotalRLE_32_CLUT != 0))
    {
        BestTotal   = TotalRLE_32_CLUT;
        Compression = RLE_32_CLUT;
    }
/*    if((TotalLZW_12 < BestTotal) && (TotalLZW_12 != 0))
    {
        BestTotal   = TotalLZW_12;
        Compression = LZW_12;
    }
    if((TotalLZW_13 < BestTotal) && (TotalLZW_13 != 0))
    {
        BestTotal   = TotalLZW_13;
        Compression = LZW_13;
    }
    if((TotalLZW_14 < BestTotal) && (TotalLZW_14 != 0))
    {
        BestTotal   = TotalLZW_14;
        Compression = LZW_14;
    }
    if((TotalLZW_15 < BestTotal) && (TotalLZW_15 != 0))
    {
        BestTotal   = TotalLZW_15;
        Compression = LZW_15;
    }
    if((TotalLZW_16 < BestTotal) && (TotalLZW_16 != 0))
    {
        BestTotal   = TotalLZW_16;
        Compression = LZW_16;
    }
    if((pRawData->at(0) == 0xFF) && (pRawData->at(1) == 0xD8))
    {
        BestTotal   = pRawData->size();
        Compression = JPEG;
    }
*/
    switch(Compression)
    {
        case RLE_4:             pData = &Data_RLE_4;        DataSize = pData->size(); DataOffset = 0; break;
        case RLE_8:             pData = &Data_RLE_8;        DataSize = pData->size(); DataOffset = 0; break;
        case RLE_16:            pData = &Data_RLE_16;       DataSize = pData->size(); DataOffset = 0; break;
        case RLE_32:            pData = &Data_RLE_32;       DataSize = pData->size(); DataOffset = 0; break;
        case RLE_16_CLUT:       pData = &Data_RLE_16_CLUT;  DataSize = pData->size(); DataOffset = 0; break;
        case RLE_32_CLUT:       pData = &Data_RLE_32_CLUT;  DataSize = pData->size(); DataOffset = 0; break;
        case LZW_12:            pData = &Data_LZW_12;       DataSize = pData->size(); DataOffset = 0; break;
        case LZW_13:            pData = &Data_LZW_13;       DataSize = pData->size(); DataOffset = 0; break;
        case LZW_14:            pData = &Data_LZW_14;       DataSize = pData->size(); DataOffset = 0; break;
        case LZW_15:            pData = &Data_LZW_15;       DataSize = pData->size(); DataOffset = 0; break;
        case LZW_16:            pData = &Data_LZW_16;       DataSize = pData->size(); DataOffset = 0; break;
        case JPEG:              /* read data from file */ break;
        case COMPRESSION_NONE:  pData = pRawData;                                                     break;
    }

    // Copy newly compressed data to compressed vector
    int DataOffsetSize = DataSize + DataOffset;
    for(int i = DataOffset; i < DataOffsetSize; i++)
    {
        pCompxData->append(pData->at(i));
    }

    return Compression;
}

// ************************************************************************************************

int CRLE_4_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int     Count;
    int     LenghtEncodeCount;
    int16_t ReadValue;
    int16_t LastValue;

    Count             = DataSize;
    LastValue         = -1;
    LenghtEncodeCount = 0;

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue = pRawData->at(DataOffset++);
        Count--;

        if(ReadValue >= 16)                                                 // No value over 15 allowed
        {
            pCompxData->clear();                                            // Clear vector
            return 0;                                                       // No compression possible
        }

        if((ReadValue != LastValue) && (LastValue != -1))                   // We are not at the beginning
        {
            LenghtEncodeCount--;
            LenghtEncodeCount <<= 4;
            pCompxData->append((uint8_t)LenghtEncodeCount | (uint8_t)LastValue);
            LenghtEncodeCount = 0;
        }

        LastValue = ReadValue;

        if(LenghtEncodeCount >= 16)                                         // We do max block 16
        {
            pCompxData->append(0xF0 | (uint8_t)LastValue);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                               // Write what was left when size reach 0
    {
        LenghtEncodeCount--;                                                // (1 - 16) -> (0 - 15)
        LenghtEncodeCount <<= 4;
        pCompxData->append((uint8_t)LenghtEncodeCount | (uint8_t)LastValue);
    }

    return pCompxData->size();
}

// ************************************************************************************************

int CRLE_8_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int     Count;
    int     LenghtEncodeCount;
    int16_t ReadValue;
    int16_t LastValue;

    Count             = DataSize;
    LastValue         = -1;
    LenghtEncodeCount = 0;

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue = pRawData->at(DataOffset++);
        Count--;

        if((ReadValue != LastValue) && (LastValue != -1))                  // We are not at the beginning
        {
            pCompxData->append((uint8_t)LenghtEncodeCount - 1);
            pCompxData->append((uint8_t)LastValue);
            LenghtEncodeCount = 0;
        }

        LastValue = ReadValue;

        if(LenghtEncodeCount >= 256)                                       // We do max block 256
        {
            pCompxData->append(0xFF);
            pCompxData->append((uint8_t)LastValue);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                             // Write what was left when size reach 0
    {
        pCompxData->append((uint8_t)LenghtEncodeCount - 1);
        pCompxData->append((uint8_t)LastValue);
    }

    return pCompxData->size();
}

// ************************************************************************************************

int CRLE_16_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int     Count;
    int     LenghtEncodeCount;
    int32_t ReadValue;
    int32_t LastValue;

    if((DataSize % 2) != 0)                                                 // We do RLE 16 on array of 2 bytes boundary
    {
        return 0;                                                           // No compression possible
    }

    Count             = DataSize;
    LastValue         = -1;
    LenghtEncodeCount = 0;

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue   = GetAt_uint16(pRawData, DataOffset);
        DataOffset += 2;
        Count      -= 2;

        if((ReadValue != LastValue) && (LastValue != -1))                   // We are not at the beginning
        {
            pCompxData->append((uint8_t)LenghtEncodeCount - 1);
            Append_uint16(pCompxData, LastValue);
            LenghtEncodeCount = 0;
        }

        LastValue = ReadValue;

        if(LenghtEncodeCount >= 256)                                        // We do max block 256
        {
            pCompxData->append(0xFF);
            Append_uint16(pCompxData, LastValue);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                               // Write what was left when size reach 0
    {
        pCompxData->append((uint8_t)LenghtEncodeCount - 1);
        Append_uint16(pCompxData, LastValue);
    }

    return pCompxData->size();
}

// ************************************************************************************************

int CRLE_32_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int     Count;
    int     LenghtEncodeCount;
    int64_t ReadValue;
    int64_t LastValue;

    if((DataSize % 4) != 0)                                                 // We do RLE 32 on array of 4 bytes boundary
    {
        return 0;                                                           // No compression possible
    }

    Count             = DataSize;
    LastValue         = -1;
    LenghtEncodeCount = 0;

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue   = GetAt_uint32(pRawData, DataOffset);
        DataOffset += 4;
        Count      -= 4;

        if((ReadValue != LastValue) && (LastValue != -1))                   // We are not at the beginning
        {
            pCompxData->append((uint8_t)LenghtEncodeCount - 1);
            Append_uint32(pCompxData, LastValue);
            LenghtEncodeCount = 0;
        }

        LastValue = ReadValue;

        if(LenghtEncodeCount >= 256)                                       // We do max block 256
        {
            pCompxData->append(0xFF);
            Append_uint32(pCompxData, LastValue);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                             // Write what was left when size reach 0
    {
        pCompxData->append((uint8_t)LenghtEncodeCount - 1);
        Append_uint32(pCompxData, LastValue);
    }

    return pCompxData->size();
}

// ************************************************************************************************

int CRLE_16_CLUT_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int      Count;
    int      NextFreeTableIndex;                                               // Next free location in table
    int      NewTableIndex;                                                    // Table location of the new read value
    int      LastTableIndex;                                                   // Table location of the last read value
    int      LenghtEncodeCount;
    int      OffsetClut;
    uint16_t TableValue;
    int32_t  ReadValue;
    int32_t  LastValue;

    if((DataSize % 2) != 0)                                                     // We do RLE 16 on array of 2 bytes boundary
    {
        return 0;                                                               // No compression possible
    }

    Count              = DataSize;
    NextFreeTableIndex = 0;
    LastTableIndex     = 0;
    LenghtEncodeCount  = 0;
    LastValue          = -1;

    OffsetClut = pCompxData->size();
    pCompxData->append(0);                                                      // reserve space for CLUT size

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue   = GetAt_uint16(pRawData, DataOffset);
        DataOffset += 2;
        Count      -= 2;

        if((LastTableIndex == -1) || (LastValue != ReadValue))                    // We do not scan for nothing
        {
            // Scan to find entry already in table
            for(NewTableIndex = 0; NewTableIndex < NextFreeTableIndex; NewTableIndex++)
            {
                int Index  = (NewTableIndex << 1) + 1;
                TableValue = GetAt_uint16(pCompxData, Index);

                if(ReadValue == TableValue)
                {
                    break;
                }
            }

            // We did not found any entry in lookup table, then add one
            if(NewTableIndex == NextFreeTableIndex)
            {
                if(NextFreeTableIndex > 255)                                    // We reach maximum element in table, so exit
                {
                    pCompxData->clear();                                        // Clear vector
                    return(0);                                                  // No compression possible
                }

                pCompxData->replace(OffsetClut, NextFreeTableIndex);
                int Index = (NextFreeTableIndex << 1) + 1;
                Insert_uint16(pCompxData, Index, ReadValue);
                NextFreeTableIndex++;
            }

            if(NewTableIndex != LastTableIndex)                                 // Insert sequence if index has change
            {
                pCompxData->append((uint8_t)LenghtEncodeCount - 1);
				pCompxData->append(LastTableIndex);
				LenghtEncodeCount = 0;
				LastTableIndex    = NewTableIndex;
            }
            LastValue = ReadValue;
        }

        if(LenghtEncodeCount >= 256)                                            // We do max block 256
        {
            pCompxData->append(0xFF);
            pCompxData->append(LastTableIndex);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                                  // Write what was left when size reach 0
    {
        pCompxData->append((uint8_t)LenghtEncodeCount - 1);
        pCompxData->append(LastTableIndex);
    }

    pCompxData->replace(0, NextFreeTableIndex - 1);                             // insert size for CLUT

    return pCompxData->size();
}

// ************************************************************************************************

int CRLE_32_CLUT_Method(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int      Count;
    int      NextFreeTableIndex;                                                // Next free location in table
    int      NewTableIndex;                                                     // Table location of the new read value
    int      LastTableIndex;                                                    // Table location of the last read value
    int      LenghtEncodeCount;
    int      OffsetClut;
    uint32_t TableValue;
    int64_t  ReadValue;
    int64_t  LastValue;

    if((DataSize % 4) != 0)                                                     // We do RLE 32 on array of 4 bytes boundary
    {
        return 0;                                                               // No compression possible
    }

    Count              = DataSize;
    NextFreeTableIndex = 0;
    LastTableIndex     = 0;
    LenghtEncodeCount  = 0;
    LastValue          = -1;

    OffsetClut = pCompxData->size();
    pCompxData->append(0);                                                      // reserve space for CLUT size

    while(Count != 0)
    {
        // Read Value and adjust offset and count
        ReadValue   = GetAt_uint32(pRawData, DataOffset);
        DataOffset += 4;
        Count      -= 4;

        if((LastTableIndex == -1) || (LastValue != ReadValue))                    // We do not scan for nothing
        {
            // Scan to find entry already in table
            for(NewTableIndex = 0; NewTableIndex < NextFreeTableIndex; NewTableIndex++)
            {
                int Index  = (NewTableIndex << 2) + 1;
                TableValue = GetAt_uint32(pCompxData, Index);

                if(ReadValue == TableValue)
                {
                    break;
                }
            }

            // We did not found any entry in lookup table, then add one
            if(NewTableIndex == NextFreeTableIndex)
            {
                if(NextFreeTableIndex > 255)                                    // We reach maximum element in table, so exit
                {
                    pCompxData->clear();                                        // Clear vector
                    return(0);                                                  // No compression possible
                }

                pCompxData->replace(OffsetClut, NextFreeTableIndex);
                int Index = (NextFreeTableIndex << 2) + 1;
                Insert_uint32(pCompxData, Index, ReadValue);
                NextFreeTableIndex++;
            }

            if(NewTableIndex != LastTableIndex)
            {
                pCompxData->append((uint8_t)LenghtEncodeCount - 1);
                pCompxData->append(LastTableIndex);
                LenghtEncodeCount = 0;
                LastTableIndex    = NewTableIndex;
            }
            LastValue = ReadValue;
        }

        if(LenghtEncodeCount >= 256)                                            // We do max block 256
        {
            pCompxData->append(0xFF);
            pCompxData->append(LastTableIndex);
            LenghtEncodeCount = 0;
        }

        LenghtEncodeCount++;
    }

    if(LenghtEncodeCount > 0)                                                   // Write what was left when size reach 0
    {
        pCompxData->append((uint8_t)LenghtEncodeCount - 1);
        pCompxData->append(LastTableIndex);
    }

    pCompxData->replace(0, NextFreeTableIndex - 1);                             // insert size for CLUT

    return pCompxData->size();
}

// ************************************************************************************************

void DeCompress(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset, uint8_t CompressionMethod)
{
    lzw* pLZW_Method;

    switch(CompressionMethod)
    {
        case RLE_4:       DRLE_4_Method       (pRawData, pCompxData, DataSize, DataOffset); break;
        case RLE_8:       DRLE_8_Method       (pRawData, pCompxData, DataSize, DataOffset); break;
        case RLE_16:      DRLE_16_Method      (pRawData, pCompxData, DataSize, DataOffset); break;
        case RLE_32:      DRLE_32_Method      (pRawData, pCompxData, DataSize, DataOffset); break;
        case RLE_16_CLUT: DRLE_16_CLUT_Method (pRawData, pCompxData, DataSize, DataOffset); break;
        case RLE_32_CLUT: DRLE_32_CLUT_Method (pRawData, pCompxData, DataSize, DataOffset); break;
        case LZW_12:
        case LZW_13:
        case LZW_14:
        case LZW_15:
        case LZW_16:
        {
            pLZW_Method = new lzw((CompressionMethod - LZW_12) + 12);
            pLZW_Method->DeCompress(pRawData, pCompxData, DataOffset);
            delete pLZW_Method;
            break;
        }

        case COMPRESSION_NONE:
        {
            int DataOffsetSize = (DataSize + DataOffset);
            for(int i = DataOffset; i < DataOffsetSize; i++)
            {
                pRawData->append(pCompxData->at(i));
            }
            break;
        }
    }
}

// ************************************************************************************************

void DRLE_4_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int     LenghtEncodeCount;
    uint8_t ReadValue;

    while(DataSize > 0)
    {
        ReadValue = pCompxData->at(DataOffset++);

        LenghtEncodeCount = (ReadValue >> 4) + 1;
        ReadValue        &= 0x0F;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            pRawData->append(ReadValue);
        }
        DataSize--;
    }
}

// ************************************************************************************************

void DRLE_8_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int     LenghtEncodeCount;
    uint8_t ReadValue;

    DataSize >>= 1;

    while(DataSize > 0)
    {
        LenghtEncodeCount = pCompxData->at(DataOffset++) + 1;
        ReadValue         = pCompxData->at(DataOffset++);

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            pRawData->append(ReadValue);
        }
        DataSize--;

    }
}

// ************************************************************************************************

void DRLE_16_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int LenghtEncodeCount;
    uint16_t ReadValue;

    DataSize /= 3;

    while(DataSize > 0)
    {
        LenghtEncodeCount = pCompxData->at(DataOffset++) + 1;
        ReadValue         = GetAt_uint16(pCompxData, DataOffset);
        DataOffset       += 2;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            Append_uint16(pRawData, ReadValue);
        }
        DataSize--;
    }
}

// ************************************************************************************************

void DRLE_32_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int     LenghtEncodeCount;
    int32_t ReadValue;

    DataSize /= 5;

    while(DataSize > 0)
    {
        LenghtEncodeCount = pCompxData->at(DataOffset++) + 1;
        ReadValue         = GetAt_uint32(pCompxData, DataOffset);
        DataOffset       += 4;

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            Append_uint32(pRawData, ReadValue);
        }
        DataSize--;
    }
}

// ************************************************************************************************

void DRLE_16_CLUT_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int      ReadIndex;
    int      LenghtEncodeCount;
    int      CLUT_Offset;
    uint16_t ReadValue;

    CLUT_Offset = DataOffset + 1;                                       // Beginning of the CLUT
    DataOffset += ((((int)pCompxData->at(DataOffset) + 1) * 2) + 1);    // Beginning of the data
    DataSize--;
    DataSize   -= (DataOffset - CLUT_Offset);
    DataSize  >>= 1;

    while(DataSize > 0)
    {
        LenghtEncodeCount = pCompxData->at(DataOffset++) + 1;
        ReadIndex         = pCompxData->at(DataOffset++);

        ReadValue = GetAt_uint16(pCompxData, CLUT_Offset + (ReadIndex * 2));

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            Append_uint16(pRawData, ReadValue);
        }
        DataSize--;
    }
}

// ************************************************************************************************

void DRLE_32_CLUT_Method(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset)
{
    int      ReadIndex;
    int      LenghtEncodeCount;
    int      CLUT_Offset;
    uint32_t ReadValue;

    CLUT_Offset = DataOffset + 1;                                       // Beginning of the CLUT
    DataOffset += ((((int)pCompxData->at(DataOffset) + 1) * 4) + 1);    // Beginning of the data
    DataSize--;
    DataSize   -= (DataOffset - CLUT_Offset);
    DataSize  >>= 1;

    while(DataSize > 0)
    {
        LenghtEncodeCount = pCompxData->at(DataOffset++) + 1;
        ReadIndex         = pCompxData->at(DataOffset++);

        ReadValue = GetAt_uint32(pCompxData, CLUT_Offset + (ReadIndex * 4));

        for(int i = 0; i < LenghtEncodeCount; i++)
        {
            Append_uint32(pRawData, ReadValue);
        }
        DataSize--;
    }
}
