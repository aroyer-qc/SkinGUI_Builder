#include "ui_mainwindow.h"
#include "SkinSave.h"
#include "compression.h"
#include "utility.h"
#include "qxmlputget.h"

// ************************************************************************************************

void MainWindow::Save()
{
    m_pProgress = new Progress("Saving skin file", "Compressing", "Saving", m_SkinName);
    m_pSkinSave = new SkinSave(m_SkinName, this);
    connect(m_pSkinSave, SIGNAL(SaveProgress(QString, int)), m_pProgress, SLOT(on_UpdateProgress(QString, int)));
    connect(m_pSkinSave, SIGNAL(SaveDone()),                 this,        SLOT(on_SaveDone()));
    m_pSkinSave->start();
}

// ************************************************************************************************

void MainWindow::on_SaveDone()
{
    disconnect(m_pSkinSave, SIGNAL(SaveProgress(QString, int)), m_pProgress, SLOT(on_UpdateProgress(QString, int)));
    disconnect(m_pSkinSave, SIGNAL(SaveDone()),                 this,        SLOT(on_SaveDone()));
    m_pSkinSave->exit();

    delete m_pProgress;
    delete m_pSkinSave;

    setSkinHasUnsavedData(false);
    UpdateStatusBar();

    if(m_IsNeedToClearDataAfterSave == true)
    {
        ClearAllData();
        ResetAllSkinTab();
        m_IsNeedToClearDataAfterSave = false;
    }

    if(m_IsNeedToCloseAfterSave == true)
    {
        this->close();
    }

    if(m_IsNeedCompleteFileOpen == true)
    {
        on_actionOpen_Skin_triggered();
        m_IsNeedCompleteFileOpen = false;
    }
}

// ************************************************************************************************
//
//  Class SkinSave ( QThread )
//
// ************************************************************************************************

SkinSave::SkinSave(QString SkinPathAndFileName, QObject* parent) : QThread(parent)
{
    m_SkinPathAndFileName = SkinPathAndFileName;

    // Image
    m_pRawData            = ((MainWindow*)parent)->getRawDataPtr();
    m_pImageInfo          = ((MainWindow*)parent)->getImageInfoPtr();

    // Font
    m_pFontSamplingInfo   = ((MainWindow*)parent)->getFontSamplingInfoPtr();
    m_pFontInfo           = ((MainWindow*)parent)->getFontInfoPtr();

    //Endian
    m_pEndian              = ((MainWindow*)parent)->getEndianPtr();
}

// ************************************************************************************************

SkinSave::~SkinSave()
{
}

// ************************************************************************************************

void SkinSave::run(void)
{
    QVector<uint8_t>    CompxData;
    QFile               File(m_SkinPathAndFileName);
    QFileInfo           FileInfo(File.fileName());

    File.open(QIODevice::WriteOnly);

    emit SaveProgress("", 0);

    // Process data
    m_ThisBlockOfData     = 0;
    m_PreviousBlockOfData = 0;

    if(SaveImageInfo(&CompxData) == true)           // Save image info structure
    {
        CompressAllImage(&CompxData);               // Try each compression method, and save best for each image in file
        m_PreviousBlockOfData = m_ThisBlockOfData;
        m_ThisBlockOfData = CompxData.size();
    }

    if(SaveFontInfo(&CompxData) == true)            // Save image info structure
    {
        CompressAllFont(&CompxData);                // Try each compression method, and save best for each image in file
        m_PreviousBlockOfData = m_ThisBlockOfData;
        m_ThisBlockOfData = CompxData.size();
    }

    // Save all data to skin file
    File.write((const char*)CompxData.data(), CompxData.size());

    // Close file and exit
    File.close();

    CreateXML(FileInfo.absolutePath() + "/" + FileInfo.baseName() + ".xml");
    emit SaveProgress("", 100);

    emit SaveDone();
    exec();
}

// ************************************************************************************************

bool SkinSave::SaveImageInfo(QVector<uint8_t>* pCompxData)
{
    GFX_ePixelFormat PixelFormat;

    m_ImageCount = m_pImageInfo->size();

    if(m_ImageCount != 0)
    {
        if(m_ThisBlockOfData != 0)
        {
            Replace_uint32(pCompxData, m_PreviousBlockOfData, pCompxData->size());
        }

        // Reserve space in compressed data for the next block of data
        Append_uint32(pCompxData, (uint32_t)0x00000000);

        // Put the type of data block (here Image)
        Append_uint16(pCompxData, (uint16_t)0x0000);

        // Put image count in compressed data
        Append_uint16(pCompxData, (uint16_t)m_ImageCount);

        m_OffsetImageHeader = pCompxData->size();

        for(int Count = 0; Count < m_ImageCount; Count++)
        {
            Append_uint32(pCompxData, (uint32_t)m_pImageInfo->at(Count).ID);                // 0  - ID
            Append_uint32(pCompxData, (uint32_t)m_pImageInfo->at(Count).DataSize);          // 4  - size compressed data

            if(m_pImageInfo->at(Count).PixelFormat == QImage::Format_ARGB32) PixelFormat = ARGB8888;
            if(m_pImageInfo->at(Count).PixelFormat == QImage::Format_RGB16)  PixelFormat = RGB565;

            pCompxData->append((uint8_t)PixelFormat);                                       // 8  - Pixel format                                                            // Compression
            Append_uint16(pCompxData, (uint16_t)m_pImageInfo->at(Count).Size.width());      // 9  - Width image
            Append_uint16(pCompxData, (uint16_t)m_pImageInfo->at(Count).Size.height());     // 11 - Height image
            pCompxData->append(0);                                                          // 13 - Compression
            Append_uint32(pCompxData, (uint32_t)0);                                         // 14 - Offset in data
        }
        return true;
    }
    return false;
}

// ************************************************************************************************

void SkinSave::CompressAllImage(QVector<uint8_t>* pCompxData)
{
    int     Progress;
    uint8_t CompressionMethod;
    int     Size;

    // Scan all image and compress all image in memory
    for(int Count = 0; Count < m_ImageCount; Count++)
    {
        Size = pCompxData->size();

        // Write offset for this image data
        Replace_uint32(pCompxData, (Count * IMAGE_HEADER_SIZE) + 14 + m_OffsetImageHeader, Size);

        // Applied change in endianess to raw data
        if(*m_pEndian == LITTLE_ENDIAN)
        {
            uint32_t EndIndex = m_pImageInfo->at(Count).RawIndex + m_pImageInfo->at(Count).DataSize;

            if(m_pImageInfo->at(Count).PixelFormat == QImage::Format_RGB16)
            {
                for(uint32_t Index = m_pImageInfo->at(Count).RawIndex; Index < EndIndex; Index += 2)
                {
                    ChangeEndianAt_uint16(m_pRawData,Index);
                }
            }
            else
            {
                for(uint32_t Index = m_pImageInfo->at(Count).RawIndex; Index < EndIndex; Index += 4)
                {
                    ChangeEndianAt_uint32(m_pRawData,Index);
                }
            }
        }

        // Write data for this image
        CompressionMethod = Compress(pCompxData,
                            m_pRawData,
                            m_pImageInfo->at(Count).DataSize,
                            m_pImageInfo->at(Count).RawIndex);

        // Applied change in endianess to raw data
        if(*m_pEndian == LITTLE_ENDIAN)
        {
            uint32_t EndIndex = m_pImageInfo->at(Count).RawIndex + m_pImageInfo->at(Count).DataSize;

            if(m_pImageInfo->at(Count).PixelFormat == QImage::Format_RGB16)
            {
                for(uint32_t Index = m_pImageInfo->at(Count).RawIndex; Index < EndIndex; Index += 2)
                {
                    ChangeEndianAt_uint16(m_pRawData,Index);
                }
            }
            else
            {
                for(uint32_t Index = m_pImageInfo->at(Count).RawIndex; Index < EndIndex; Index += 4)
                {
                    ChangeEndianAt_uint32(m_pRawData,Index);
                }
            }
        }

        // Write compression method for this image
        pCompxData->replace((Count * IMAGE_HEADER_SIZE) + 13 + m_OffsetImageHeader, CompressionMethod);

        Size = pCompxData->size() - Size;

        // Write raw compress datasize for this image
        Replace_uint32(pCompxData, (Count * IMAGE_HEADER_SIZE) + 4 + m_OffsetImageHeader, uint32_t(Size));

        Progress = (Count * 98) / m_ImageCount;
        emit SaveProgress("", Progress);
    }
}

// ************************************************************************************************

bool SkinSave::SaveFontInfo(QVector<uint8_t>* pCompxData)
{
    m_FontCount = m_pFontInfo->count();

    if(m_FontCount != 0)
    {
        if(m_ThisBlockOfData != 0)
        {
            Replace_uint32(pCompxData, m_PreviousBlockOfData, pCompxData->size());
        }

        Append_uint32(pCompxData, (uint32_t)0x00000000);        // Reserve space in compressed data for the next block of data
        Append_uint16(pCompxData, (uint16_t)0x0001);            // Put type of data block (here Font)
        Append_uint16(pCompxData, (uint16_t)m_FontCount);       // Put font count in compressed data

        m_TotalCharCount = 0;                                   // Reset value of the character count

        for(int Count = 0; Count < m_FontCount; Count++)
        {
            m_OffsetFontHeight.append(pCompxData->size());      // Kept offset so we can modifed then later
            pCompxData->append(0x00);                           // Reserve space for height for this font
            pCompxData->append(0x00);                           // Reserve space for interline for this font
        }

        for(int Count = 0; Count < m_FontCount; Count++)
        {
            m_InFontCharCount = 0;

            m_pFont       = &m_pFontInfo->at(Count);
            m_pFontMetric = new QFontMetrics(*m_pFont);

            // Put font count in compressed data
            Append_uint32(pCompxData, Count);                   // For now we use count number as ID

            // Reserve space in compressed data for character count
            m_OffsetFontCharCountHeader = pCompxData->size();   // Keep offset for later
            pCompxData->append(0x00);                           // Value rewritten when count is knowned

            // Sample all alpha character
            if((m_pFontSamplingInfo->at(Count) & SAMPLING_ALPHA) != 0)
            {
                for(uint32_t CharCount = 'a'; CharCount <= 'z'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = 'A'; CharCount <= 'Z'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
            }

            // Sample all numeric character
            if((m_pFontSamplingInfo->at(Count) & SAMPLING_NUMERIC) != 0)
            {
                for(uint32_t CharCount = '0'; CharCount <= '9'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
            }

            // Sample all symbol character
            if((m_pFontSamplingInfo->at(Count) & SAMPLING_SYMBOL) != 0)
            {
                for(uint32_t CharCount = ' '; CharCount <= '/'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = ':'; CharCount <= '@'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = '['; CharCount <= '`'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = '{'; CharCount <= '~'; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
            }

            // Sample all extra symbol character
            if((m_pFontSamplingInfo->at(Count) & SAMPLING_EXTRA_SYMBOL) != 0)
            {
                ExtractFontInfo(pCompxData, 153); //'tm'
                ExtractFontInfo(pCompxData, 169); //'©'
                ExtractFontInfo(pCompxData, 174); //'®'
                ExtractFontInfo(pCompxData, 176); //'°'
                ExtractFontInfo(pCompxData, 185); //'±'
            }

            // Sample all latin character
            if((m_pFontSamplingInfo->at(Count) & SAMPLING_LATIN) != 0)
            {
                for(uint32_t CharCount = 192/*'À'*/; CharCount <= 214/*'Ö'*/; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = 216/*'Ø'*/; CharCount <= 246/*'ö'*/; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
                for(uint32_t CharCount = 248/*'ø'*/; CharCount <= 254/*'þ'*/; CharCount++) ExtractFontInfo(pCompxData, char(CharCount));
            }

            pCompxData->replace(m_OffsetFontCharCountHeader, m_InFontCharCount); // Write character count
        }
        return true;
    }
    return false;
}

// ************************************************************************************************

void SkinSave::CompressAllFont(QVector<uint8_t>* pCompxData)
{
    m_TotalCharCount = 0;                                      // Reset value of the character count

    for(int Count = 0; Count < m_FontCount; Count++)
    {
        uint32_t StartOffsetFont = m_TotalCharCount;
        uint8_t MinY = 0xFF;
        uint8_t MaxY = 0x00;

        m_pFont       = &m_pFontInfo->at(Count);
        m_pFontMetric = new QFontMetrics(*m_pFont);

        // Sample all alpha character
        if((m_pFontSamplingInfo->at(Count) & SAMPLING_ALPHA) != 0)
        {
            for(uint32_t CharCount = 'a'; CharCount <= 'z'; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = 'A'; CharCount <= 'Z'; CharCount++) CompressFont(pCompxData, char(CharCount));
        }

        // Sample all numeric character
        if((m_pFontSamplingInfo->at(Count) & SAMPLING_NUMERIC) != 0)
        {
            for(uint32_t CharCount = '0'; CharCount <= '9'; CharCount++) CompressFont(pCompxData, char(CharCount));
        }

        // Sample all symbol character
        if((m_pFontSamplingInfo->at(Count) & SAMPLING_SYMBOL) != 0)
        {
            for(uint32_t CharCount = ' '; CharCount <= '/'; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = ':'; CharCount <= '@'; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = '['; CharCount <= '`'; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = '{'; CharCount <= '~'; CharCount++) CompressFont(pCompxData, char(CharCount));
        }

        // Sample all extra symbol character
        if((m_pFontSamplingInfo->at(Count) & SAMPLING_EXTRA_SYMBOL) != 0)
        {
            CompressFont(pCompxData, 153); //'tm'
            CompressFont(pCompxData, 169); //'©'
            CompressFont(pCompxData, 174); //'®'
            CompressFont(pCompxData, 176); //'°'
            CompressFont(pCompxData, 185); //'±'
        }

        // Sample all latin character
        if((m_pFontSamplingInfo->at(Count) & SAMPLING_LATIN) != 0)
        {
            for(uint32_t CharCount = 192/*'À'*/; CharCount <= 214/*'Ö'*/; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = 216/*'Ø'*/; CharCount <= 246/*'ö'*/; CharCount++) CompressFont(pCompxData, char(CharCount));
            for(uint32_t CharCount = 248/*'ø'*/; CharCount <= 254/*'þ'*/; CharCount++) CompressFont(pCompxData, char(CharCount));
        }

        // Rescan for lowest Y minimum and
        for(uint32_t CharCount = StartOffsetFont; CharCount < m_TotalCharCount; CharCount++)
        {
            if(MinY > m_MinY[CharCount]) MinY = m_MinY[CharCount];
            if(MaxY < m_MaxY[CharCount]) MaxY = m_MaxY[CharCount];
        }

        // Write the new height for the font
        pCompxData->replace(m_OffsetFontHeight[Count], (MaxY - MinY) + 1);

        // We subtract this absolute minimum from height, and all other minimum and maximum
        for(uint32_t CharCount = StartOffsetFont; CharCount < m_TotalCharCount; CharCount++)
        {
             pCompxData->replace(m_OffsetFontHeader[CharCount] + 8,  m_MinY[CharCount] -= MinY);
        }
    }
}

// ************************************************************************************************

void SkinSave::ExtractFontInfo(QVector<uint8_t>* pCompxData, char Char)
{
    m_OffsetFontHeader.append(pCompxData->size());          // Kept the offset for this character header

    pCompxData->append(Char);                               // 0     - Put the character value
    Append_uint16(pCompxData, (uint16_t)0x0000);            // 1-2   - Reserve space for data size
    pCompxData->append(0x00);                               // 3     - Reserve space for left bearing
    pCompxData->append(0x00);                               // 4     - Reserve space for right bearing
    pCompxData->append(0x00);                               // 5     - Reserve space for width Pixel
    pCompxData->append(0x00);                               // 6     - Reserve space for height Pixel
    pCompxData->append(0x00);                               // 7     - Reserve space for width
    pCompxData->append(0x00);                               // 8     - Reserve space for Offset Y
    pCompxData->append(0x00);                               // 9     - Reserve space for compression
    Append_uint32(pCompxData, (uint32_t)0x00000000);        // 10-13 - Reserve space for offset in raw data

    m_TotalCharCount++;
    m_InFontCharCount++;
}

// ************************************************************************************************

void SkinSave::CompressFont(QVector<uint8_t>* pCompxData, char Char)
{
    QPixmap* pPix = new QPixmap(SAMPLING_BOX_X_SIZE, SAMPLING_BOX_Y_SIZE);
    QPainter* pPainter = new QPainter(pPix);
    QImage Image;
    uint16_t Count = 0;
    uint8_t CompressionMethod;
    QVector<uint8_t> InputData;
    uint32_t OffsetFontHeader = m_OffsetFontHeader[m_TotalCharCount];
    int     Size;
    bool FoundPixel = false;

    // Add new min/max X
    m_MinX.append(SAMPLING_BOX_X_SIZE);
    m_MaxX.append(0);
    m_MinY.append(SAMPLING_BOX_Y_SIZE);
    m_MaxY.append(0);
    m_Width.append(m_pFontMetric->width(Char));
    m_LeftBearing.append(m_pFontMetric->leftBearing(Char));
    m_RightBearing.append(m_pFontMetric->leftBearing(Char));

    // Prepare Pix map for character drawing white in black
    pPainter->setPen(Qt::white);
    pPainter->fillRect(0, 0, SAMPLING_BOX_X_SIZE, SAMPLING_BOX_Y_SIZE, Qt::black);
    pPainter->setFont(*m_pFont);
    pPainter->drawText(QPoint(20, m_pFontMetric->height()), QString("%1").arg(Char));
    Image = pPix->toImage();

    // Found the sampling rectangle size
    for(uint8_t y = 0; y < SAMPLING_BOX_Y_SIZE; y++)
    {
        for(uint8_t x = 0; x < SAMPLING_BOX_X_SIZE; x++)
        {
            QRgb ColorPixel;

            ColorPixel = Image.pixel(x, y);
            if((ColorPixel & 0x00FFFFFF) != 0x00000000)
            {
                FoundPixel = true;
                if(x < m_MinX[m_TotalCharCount]) m_MinX[m_TotalCharCount] = x;
                if(x > m_MaxX[m_TotalCharCount]) m_MaxX[m_TotalCharCount] = x;
                if(y < m_MinY[m_TotalCharCount]) m_MinY[m_TotalCharCount] = y;
                if(y > m_MaxY[m_TotalCharCount]) m_MaxY[m_TotalCharCount] = y;
            }
        }
    }

    // Write the raw data offset address in character structure only if character has data (example 'SPACE' has no data)
    if(FoundPixel == true)
    {
        Replace_uint32(pCompxData, OffsetFontHeader + 10, pCompxData->size());              // Write raw data offset for this character
    }

    // Copy rectangle found data in linear data for compression
    for(uint8_t y = m_MinY[m_TotalCharCount]; y <= m_MaxY[m_TotalCharCount]; y++)
    {
        for(uint8_t x = m_MinX[m_TotalCharCount]; x <= m_MaxX[m_TotalCharCount]; x++)
        {
            InputData.append(qGray(Image.pixel(x, y)));
            Count++;
        }
    }

    if(Count)
    {
        Size = pCompxData->size();
        CompressionMethod = Compress(pCompxData, &InputData, Count, 0);
        Size = pCompxData->size() - Size;
        // Write raw compress datasize for this image
        Replace_uint16(pCompxData, OffsetFontHeader + 1, uint16_t(Size));
        int8_t lb = m_pFontMetric->leftBearing(Char);


        if(lb < 0)
        {
            lb--;
            lb++;
        }


        pCompxData->replace(OffsetFontHeader + 3, uint8_t(lb));
        int8_t rb = m_pFontMetric->rightBearing(Char);
        pCompxData->replace(OffsetFontHeader + 4, uint8_t(rb));
        pCompxData->replace(OffsetFontHeader + 5,  (m_MaxX[m_TotalCharCount] - m_MinX[m_TotalCharCount]) + 1);
        pCompxData->replace(OffsetFontHeader + 6,  (m_MaxY[m_TotalCharCount] - m_MinY[m_TotalCharCount]) + 1);
        pCompxData->replace(OffsetFontHeader + 9, CompressionMethod);
    }
    pCompxData->replace(OffsetFontHeader + 7,  m_pFontMetric->width(Char));     // outside because space need width but has no data

    m_TotalCharCount++;
}

// ************************************************************************************************

void SkinSave::CreateXML(QString Path)
{
    QXmlPut xmlPut("Skin");

    // Little or Big endian
    xmlPut.descend("Endian");
    xmlPut.putString("State", (*m_pEndian == LITTLE_ENDIAN) ? "Little" : "Big");
    xmlPut.rise();

    // Image information
    xmlPut.descend("Image");
    for(int i = 0; i < m_ImageCount; i++)
    {
        xmlPut.putString("File", m_pImageInfo->at(i).Filename);
    }
    xmlPut.rise();

    // Font Information
    xmlPut.descend("Font");

    for(int i = 0; i < m_FontCount; i++)
    {
        xmlPut.putFont("Family", m_pFontInfo->at(i));
        xmlPut.putInt("Option", m_pFontSamplingInfo->at(i));
    }
    xmlPut.rise();

    xmlPut.save(Path);
}

// ************************************************************************************************

