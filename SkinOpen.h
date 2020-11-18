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

#ifndef __SKIN_OPEN_H__
#define __SKIN_OPEN_H__

#include "mainwindow.h"
#include <QThread>
#include <QTimer>

const int BytesPerPixel[19] =
{
    0,  // 0
    1,  // 1
    1,  // 2
    1,  // 3
    4,  // 4
    4,  // 5
    4,  // 6
    2,  // 7
    3,  // 8
    3,  // 9
    3,  // 10
    2,  // 11
    3,  // 12
    3,  // 13
    2,  // 14
    2,  // 15
    4,  // 16
    4,  // 17
    4,  // 18
};


class SkinOpen : public QThread
{
    Q_OBJECT

    public:
        explicit    SkinOpen                (QString SkinPathAndFileName, QObject* parent = 0);
                    ~SkinOpen               ();

    signals:
        void        OpenProgress            (QString Status, int Value);
        void        OpenDone                ();

    protected:
        void        run                     ();

    private:

        void        DeCompressAllImage      (QVector<uint8_t>* pCompxData);
        void        OpenImageInfo           (QVector<uint8_t>* pCompxData);
        void        ReadXML                 (QString Path);


        eEndianess*             m_pEndian;
        QVector<uint8_t>*       m_pRawData;
        QVector<sImageInfo>*    m_pImageInfo;
        QString                 m_SkinPathAndFileName;

        QVector<QFont>*         m_pFontInfo;
        QVector<uint8_t>*       m_pFontSamplingInfo;

        int                     m_NextBlockOfData;

        uint16_t                m_ImageCount;
        int                     m_OffsetImageCount;
        int                     m_OffsetImageHeader;
};

#endif // SKIN_OPEN_H
