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
