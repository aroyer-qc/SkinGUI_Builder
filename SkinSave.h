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

#ifndef __SKIN_SAVE_H__
#define __SKIN_SAVE_H__

#include "mainwindow.h"
#include <QThread>
#include <QTimer>


//                  Skin file description
//
//                   ______________________________________________
//  Image block     |                                              |
//                  | Next block of data                (uint32_t) |
//                  |______________________________________________|
//                  |                                              |
//                  | Block type 0x0000                 (uint16_t) |
//                  |______________________________________________|
//                  |                                              |
//                  | Image Count                       (uint16_t) |
//                  |______________________________________________|__
//                  |                                              |  |
//                  | Structure image information                  |  |
//                  |                                              |  |
//                  |   0-3   Image ID                  (uint32_t) |  |
//                  |   4-7   Datasize                  (uint32_t) |  |___
//                  |   8     Pixel format              (uint8_t)  |  |   |
//                  |   9-10  Width                     (uint16_t) |  |   |
//                  |   11-12 Height                    (uint16_t) |  |   |
//                  |   13    Compression               (uint8_t)  |  |   |
//                  |   14-17 Offset in data            (uint32_t) |  |  /_\
//                  |______________________________________________|__|   |
//                  |                                              |      |
//                  |   ... Repeat last structure                  |\_____|
//                  |       According to image count               |/
//                  |______________________________________________|__
//                  |                                              |  |
//                  |   Image compressed raw data                  |  |___
//                  |______________________________________________|__|   |
//                  |                                              |     /_\
//                  |   ... Repeat compressed data                 |\_____|
//                  |       According to image count               |/
//                  |______________________________________________|
//
//                   ______________________________________________
//  Font block      |                                              |
//                  | Next block of data                (uint32_t) |
//                  |______________________________________________|
//                  |                                              |
//                  | Block type 0x0001                 (uint16_t) |
//                  |______________________________________________|
//                  |                                              |
//                  | Font Count                        (uint16_t) |
//                  |______________________________________________|__
//                  |                                              |  |
//                  |   0   Height                      (uint8_t)  |  |___
//                  |   1   Interline                   (uint8_t)  |  |   |
//                  |______________________________________________|__|   |
//                  |                                              |     /_\
//                  |   ... Repeat last structure                  |\_____|
//                  |       According to font                      |/
//                  |______________________________________________|______________
//                  |                                              |              |
//                  |   0-3 Font ID                     (uint32_t) |              |
//                  |   4   Character count in font     (uint8_t)  |              |
//                  |______________________________________________|__            |
//                  |                                              |  |           |
//                  | Structure font information                   |  |           |
//                  |                                              |  |           |
//                  |   0     Char number               (uint8_t)  |  |           |
//                  |   1-2   Datasize                  (uint16_t) |  |           |
//                  |   3     Left Bearing              (int8_t)   |  |___        |____
//                  |   4     right Bearing             (int8_t)   |  |   |       |    |
//                  |   5     Width Pixel Zone          (uint8_t)  |  |   |       |    |
//                  |   6     Height Pixel Zone         (uint8_t)  |  |   |       |    |
//                  |   7     Width                     (uint8_t)  |  |   |       |    |
//                  |   8     Offset Y                  (uint8_t)  |  |  /_\      |   /_\
//                  |   9     Compression               (uint8_t)  |  |   |       |    |
//                  |   10-13 Offset in data            (uint32_t) |  |   |       |    |
//                  |______________________________________________|__|   |       |    |
//                  |                                              |      |       |    |
//                  |   ... Repeat last structure                  |\_____|       |    |
//                  |       According to char count in this font   |/             |    |
//                  |______________________________________________|______________|    |
//                  |                                              |                   |
//                  |   ... Repeat last section                    |\__________________|
//                  |       According to font count                |/
//                  |______________________________________________|__
//                  |                                              |  |
//                  |   Image compressed raw data                  |  |___
//                  |______________________________________________|__|   |
//                  |                                              |     /_\
//                  |   ... Repeat compressed data                 |\_____|
//                  |       According to total character count for |/
//                  |       each font                              |
//                  |______________________________________________|
//
//

class SkinSave : public QThread
{
    Q_OBJECT

    public:
        explicit    SkinSave                (QString SkinPathAndFileName, QObject* parent = 0);
                    ~SkinSave               ();

    signals:
        void        SaveProgress            (QString Status, int Value);
        void        SaveDone                ();

    protected:
        void        run                     ();

    private:

        void        CompressAllFont         (QVector<uint8_t>* pCompxData);
        void        CompressAllImage        (QVector<uint8_t>* pCompxData);
        bool        SaveFontInfo            (QVector<uint8_t>* pCompxData);
        bool        SaveImageInfo           (QVector<uint8_t>* pCompxData);
        void        CreateXML               (QString Path);
        void        ExtractFontInfo         (QVector<uint8_t>* pCompxData, char Char);
        void        CompressFont            (QVector<uint8_t>* pCompxData, char Char);

        eEndianess*             m_pEndian;
        QVector<uint8_t>*       m_pRawData;
        QString                 m_SkinPathAndFileName;
        int                     m_PreviousBlockOfData;
        int                     m_ThisBlockOfData;

        QVector<sImageInfo>*    m_pImageInfo;
        int                     m_ImageCount;
        uint32_t                m_OffsetImageHeader;

        QVector<QFont>*         m_pFontInfo;
        QVector<uint8_t>*       m_pFontSamplingInfo;
        QVector<uint32_t>       m_OffsetFontHeader;
        QVector<uint32_t>       m_OffsetFontHeight;
        int                     m_FontCount;
        uint32_t                m_TotalCharCount;
        uint32_t                m_InFontCharCount;
        uint32_t                m_OffsetFontCharCountHeader;
        uint8_t                 m_MaxX_FixedFont;
        QVector<uint8_t>        m_MinX;
        QVector<uint8_t>        m_MaxX;
        QVector<uint8_t>        m_MinY;
        QVector<uint8_t>        m_MaxY;
        QVector<uint8_t>        m_Width;
        QVector<int8_t>         m_LeftBearing;
        QVector<int8_t>         m_RightBearing;



        const QFont*            m_pFont;
        QFontMetrics*           m_pFontMetric;


};

#endif // SKIN_SAVE_H
