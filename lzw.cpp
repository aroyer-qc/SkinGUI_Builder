//********************************************************************
//**
//** Copyright (c) 1989 Mark R. Nelson
//** April 13, 1989
//** LZW data compression/expansion.
//**
//** Alain Royer modified for C++ and using QVector instead of FILE
//** Put into class
//** Number of bit can be selected dynamically
//**
//*********************************************************************
#include "compression.h"


lzw::lzw(int Bits)
{
    ChangeBits(Bits);
}


lzw::~lzw()
{
    m_CodeValue.clear();
    m_AppendCode.clear();
    m_PrefixCode.clear();
    m_DecodeStack.clear();
}


void lzw::ChangeBits(int Bits)
{
    m_Bits = Bits;

    switch(Bits)
    {
        case 16:              m_TableSize = 74521; break;
        case 15:              m_TableSize = 36251; break;
        case 14:              m_TableSize = 18041; break;
        case 13:              m_TableSize = 9029;  break;
        default: m_Bits = 12; m_TableSize = 5021;  break;    // we override bit definition if unsupported number
    }

    m_HashingShift = Bits - 8;
    m_MaxValue     = (1 << Bits) - 1;
    m_MaxCode      = m_MaxValue - 1;

    SetVector();
}

void lzw::SetVector()
{
    m_CodeValue.resize(m_TableSize);
    m_PrefixCode.resize(m_TableSize);
    m_AppendCode.resize(m_TableSize);

    for(int i = 0; i < m_TableSize; i++)        // Clear out the code table before starting
    {
        m_CodeValue[i] = -1;
    }
}

int lzw::Compress(QVector<uint8_t>* pCompxData, QVector<uint8_t>* pRawData, int DataSize, int DataOffset)
{
    int  NextCode;
    int  Code;
    int  CodeArray;
    int  Index;

    m_BitCount  = 0;
    m_BitBuffer = 0L;
    NextCode    = 256;                                      // Next code is the next available array code

    CodeArray = pRawData->at(DataOffset++);                 // Get the first code
    DataSize--;

    // This is the main loop where it all happens.  This loop runs until all of
    // the input has been exhausted.  Note that it stops adding codes to the
    // table after all of the possible codes have been defined.
    while(DataSize >= 1)
    {
        DataSize--;
        Code  = pRawData->at(DataOffset++);
        Index = FindMatch(CodeArray, Code);                 // See if the array is in the table.

        if(m_CodeValue[Index] != -1)                        // If it is,
        {
            CodeArray = m_CodeValue[Index];                 // get the code value.
        }
        else                                                // If the array is not in the table, try to add it.
        {
            if(NextCode <= m_MaxCode)
            {
                m_CodeValue[Index]  = NextCode++;
                m_PrefixCode[Index] = CodeArray;
                m_AppendCode[Index] = Code;
            }

            OutputCode(pCompxData, CodeArray);              // When a array is found that is not in the table,
            CodeArray = Code;                               // output the last array after adding the new one
        }
    }

    // End of the main loop.
    OutputCode(pCompxData, CodeArray);                      // Output the last code
    OutputCode(pCompxData, m_MaxValue);                     // Output the end of buffer code
    OutputCode(pCompxData, (uint)0);                        // This code flushes the output buffer
    OutputCode(pCompxData, (uint)0);

    return pCompxData->size();
}

// This is the hashing routine.  It tries to find a match for the prefix+code
// array in the array table.  If it finds it, the index is returned.  If
// the array is not found, the first available index in the array table is
// returned instead.
int lzw::FindMatch( int HashPrefix, int HashCode)
{
    int Index;
    int Offset;

    Index = (HashCode << m_HashingShift) ^ HashPrefix;
    if(Index == 0)
    {
        Offset = 1;
    }
    else
    {
        Offset = m_TableSize - Index;
    }

    while(1)
    {
        if(m_CodeValue[Index] == -1)
        {
            return Index;
        }

        if((m_PrefixCode[Index] == HashPrefix) && (m_AppendCode[Index] == HashCode))
        {
            return Index;
        }

        Index -= Offset;

        if(Index < 0)
        {
            Index += m_TableSize;
        }
    }
}

void lzw::DeCompress(QVector<uint8_t>* pRawData, QVector<uint8_t>* pCompxData, int DataOffset)
{
    int         NextCode;
    int         NewCode;
    int         OldCode;
    int         Code;

    NextCode     = 256;                                             // This is the next available code to define
    m_BitCount   = 0;
    m_BitBuffer  = 0L;

    OldCode = InputCode(pCompxData, &DataOffset);                   // Read in the first code, initialize the
    Code    = OldCode;                                              // character variable, and send the first
    pRawData->append(OldCode);                                      // code to the output vector

    // This is the main expansion loop.  It reads in characters from the LZW file
    // until it sees the special code used to inidicate the end of the data.

    while((NewCode = InputCode(pCompxData, &DataOffset)) != (m_MaxValue))
    {
        // This code checks for the special ARRAY+CODE+ARRAY+CODE+ARRAY
        // case which generates an undefined code.  It handles it by decoding
        // the last code, and adding a single character to the end of the decode string.
        if(NewCode >= NextCode)
        {
            m_DecodeStack.append(Code);
            DecodeArray(OldCode);
        }
        // Otherwise we do a straight decode of the new code.
        else
        {
            DecodeArray(NewCode);
        }

        // Now we output the decoded string in reverse order.
        Code = m_DecodeStack.last();
        while(m_DecodeStack.size() != 0)
        {
            uint8_t Value = m_DecodeStack.takeLast();
            pRawData->append(Value);
        }

        // Finally, if possible, add a new code to the string table.
        if(NextCode <= m_MaxCode)
        {
            m_PrefixCode[NextCode] = OldCode;
            m_AppendCode[NextCode] = Code;
            NextCode++;
        }
        OldCode = NewCode;
    }
}

void lzw::DecodeArray(int Code)
{
    int i = 0;

    while(Code > 255)
    {
        m_DecodeStack.append(m_AppendCode[Code]);
        Code = m_PrefixCode[Code];

        if(i++ >= m_MaxCode)
        {
            //ERROR  //exit(-3);
        }
    }
    m_DecodeStack.append(Code);
}

int lzw::InputCode(QVector<uint8_t>* pCompxData, int* pIndex)
{
    int Value;

    while(m_BitCount <= 24)
    {
         m_BitBuffer |= ((uint32_t)pCompxData->at(*pIndex) << (24 -  m_BitCount));
        (*pIndex)++;
         m_BitCount += 8;
    }

    Value = (int)( m_BitBuffer >> (32 - m_Bits));
     m_BitBuffer <<= m_Bits;
     m_BitCount   -= m_Bits;

    return Value;
}

void lzw::OutputCode(QVector<uint8_t>* pCompxData, int Code)
{
    m_BitBuffer |= (uint32_t)Code << (32 - m_Bits -  m_BitCount);
    m_BitCount  += m_Bits;

    while( m_BitCount >= 8)
    {
        pCompxData->append(m_BitBuffer >> 24);
         m_BitBuffer <<= 8;
         m_BitCount   -= 8;
    }
}
