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

#ifndef __COMPRESSION_H__
#define __COMPRESSION_H__

#include <QVector>
#include "typedef.h"

// Compression
uint8_t Compress                (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_4_Method           (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_8_Method           (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_16_Method          (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_32_Method          (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_16_CLUT_Method     (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
int     CRLE_32_CLUT_Method     (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);

// Decompression
void    DeCompress              (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset, uint8_t CompressionMethod);
void    DRLE_4_Method           (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);
void    DRLE_8_Method           (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);
void    DRLE_16_Method          (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);
void    DRLE_32_Method          (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);
void    DRLE_16_CLUT_Method     (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);
void    DRLE_32_CLUT_Method     (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataSize, int DataOffset);


class lzw
{
    public:

                            lzw(int Bits = 12);
                            ~lzw();

        int                 Compress                (QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset);
        void                DeCompress              (QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataOffset);
        void                ChangeBits              (int Bits);

    private:

        void                SetVector               ();
        int                 FindMatch               (int HashPrefix, int HashCode);
        void                OutputCode              (QVector<uint8_t>* OutputData, int Code);
        int                 InputCode               (QVector<uint8_t>* InputData, int* pIndex);
        void                DecodeArray             (int Code);

        int                 m_Bits;
        int                 m_HashingShift;
        int                 m_MaxValue;
        int                 m_MaxCode;
        int                 m_TableSize;
        int                 m_BitCount;
        uint32_t            m_BitBuffer;
        QVector<int>        m_CodeValue;
        QVector<int>        m_PrefixCode;
        QVector<int>        m_DecodeStack;
        QVector<int>        m_AppendCode;
};


#endif
