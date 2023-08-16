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

#include "Utility.h"
#include "qxmlputget.h"

// ************************************************************************************************

void UpdateComboBox(QComboBox *comboBox)
{
    if(comboBox->findText(comboBox->currentText()) == -1)
    {
        comboBox->addItem(comboBox->currentText());
    }
}

// ************************************************************************************************

QString GetFormat(QImage::Format format)
{
    QString myFormat;

    switch(format)
    {
        case QImage::Format_Invalid:                myFormat = ("Invalid");                 break;
        case QImage::Format_Mono:                   myFormat = ("Mono");                    break;
        case QImage::Format_MonoLSB:                myFormat = ("Mono LSB");                break;
        case QImage::Format_Indexed8:               myFormat = ("Indexed 8");               break;
        case QImage::Format_RGB32:                  myFormat = ("RGB32");                   break;
        case QImage::Format_ARGB32:                 myFormat = ("ARGB8888");                break;
        case QImage::Format_ARGB32_Premultiplied:   myFormat = ("ARGB32 Premultiplied");    break;
        case QImage::Format_RGB16:                  myFormat = ("RGB16 (RGB565)");          break;
        case QImage::Format_ARGB8565_Premultiplied: myFormat = ("ARGB8565");                break;
        case QImage::Format_RGB666:                 myFormat = ("RGB666");                  break;
        case QImage::Format_ARGB6666_Premultiplied: myFormat = ("ARGB8666");                break;
        case QImage::Format_RGB555:                 myFormat = ("RGB555");                  break;
        case QImage::Format_ARGB8555_Premultiplied: myFormat = ("ARGB8555 Premultiplied");  break;
        case QImage::Format_RGB888:                 myFormat = ("RGB888");                  break;
        case QImage::Format_RGB444:                 myFormat = ("RGB444");                  break;
        case QImage::Format_ARGB4444_Premultiplied: myFormat = ("ARGB4444 Premultiplied");  break;
        case QImage::Format_RGBX8888:               myFormat = ("RGBX8888");                break;
        case QImage::Format_RGBA8888:               myFormat = ("RGBA8888");                break;
        case QImage::Format_RGBA8888_Premultiplied: myFormat = ("RGBA8888 Premultiplied");  break;
        default:                                    myFormat = ("Unknow Format");           break;
    }
    return myFormat;
}

// ************************************************************************************************

extern QPoint CenterPoint(QSize Size, QSize MaxSize)
{
    QPoint Point;

    // This will center the image in the pixmap
    if(Size.width() < MaxSize.width())   Point.setX((MaxSize.width() - Size.width()) / 2);
    else                                 Point.setX(0);

    if(Size.height() < MaxSize.height()) Point.setY((MaxSize.height() - Size.height()) / 2);
    else                                 Point.setY(0);

    return Point;
}

// ************************************************************************************************

void CheckerPattern(QGraphicsScene* pScene)
{
    QBrush Brush;
    QPixmap checker(":/Images/checker.png");

    Brush.setTexture(checker);
    pScene->setBackgroundBrush(Brush);
}

// ************************************************************************************************

QString PrintSize(QSize Size)
{
    return QString("X: %1    Y: %2").arg(Size.width()).arg(Size.height());
}

// ************************************************************************************************

QString PrintFileSize(int Count)
{
    return  QString("%1 kB").arg(int((Count + 1023) / 1024));
}

// ************************************************************************************************

QSize GetSizeFromGraphicsView(QGraphicsView* pGraphView, QSize Size)
{
    QSize SizeCorrected;

    SizeCorrected.setHeight((Size.height() > MINIMUM_PREVIEW_HEIGHT) ? Size.height() : MINIMUM_PREVIEW_HEIGHT); // Add the height of the GraphicView
    SizeCorrected.setHeight(SizeCorrected.height() +
                            pGraphView->style()->pixelMetric(QStyle::PM_ScrollBarExtent));                      // Add the ScrollBar size

    SizeCorrected.setWidth((Size.width() > MINIMUM_PREVIEW_WIDTH) ? Size.width() : MINIMUM_PREVIEW_WIDTH);      // Add the width of the GraphicView
    SizeCorrected.setWidth(SizeCorrected.width() +
                           pGraphView->style()->pixelMetric(QStyle::PM_ScrollBarExtent));                       // Add the ScrollBar size

    return SizeCorrected;
}

// ************************************************************************************************

void setWidgetXY(QWidget* pWidget, int X, int Y)
{
    QRect Rect = pWidget->geometry();
    if(X >= 0) Rect.moveLeft(X);
    if(Y >= 0) Rect.moveTop(Y);
    pWidget->setGeometry(Rect);
}

// ************************************************************************************************

void setWidgetSize(QWidget* pWidget, int Width, int Height)
{
    QRect Rect = pWidget->geometry();
    if(Width >= 0)  Rect.setWidth(Width);
    if(Height >= 0) Rect.setHeight(Height);
    pWidget->setGeometry(Rect);
}

// ************************************************************************************************

QPoint SelectScreenAndCenter(QSize Size)
{
    QGuiApplication Desktop;

    QScreen* primary = Desktop.primaryScreen();
    int screenWidth  = primary->geometry().width();
    int screenHeight = primary->geometry().height();
    QRect Resolution = primary->geometry();
    Resolution.setX(Resolution.x() + ((screenWidth / 2)  - (Size.width()  / 2)));
    Resolution.setY(Resolution.y() + ((screenHeight / 2) - (Size.height() / 2)));
    return(QPoint(Resolution.x(), Resolution.y()));
}

// ************************************************************************************************

void ClearTable(QTableWidget* pTable)
{
    pTable->blockSignals(true);
    while(pTable->rowCount() > 0)
    {
        pTable->removeRow(0);
    }
    pTable->blockSignals(false);
}

// ************************************************************************************************

bool getStateID_Code(QVector<uint8_t>* pData, ID_Code ID)
{
    int     BitOffset;
    char    Value;
    int     Offset;

    Offset     = (SIZE_OF_ID_CODE_SPACE / 8) * (int)ID.getType();
    Offset    += ID.getNumber() / 8;
    BitOffset  = ID.getNumber() % 8;
    Value      = pData->at(Offset);
    Value     &= (char)1 << BitOffset;
    return (Value) ? true : false;
}

// ************************************************************************************************

void setStateID_Code(QVector<uint8_t>* pData, ID_Code ID)
{
    int     BitOffset;
    char    Value;
    int     Offset;

    Offset           = (SIZE_OF_ID_CODE_SPACE / 8) * (int)ID.getType();
    Offset          += (ID.getNumber() / 8);
    BitOffset        = ID.getNumber() % 8;
    Value            = pData->at(Offset);
    Value           |= (1 << BitOffset);
    pData->replace(Offset, Value);
}

// ************************************************************************************************

void clearStateID_Code(QVector<uint8_t>* pData, ID_Code ID)
{
    int     BitOffset;
    char    Value;
    int     Offset;

    Offset           = (SIZE_OF_ID_CODE_SPACE / 8) * (int)ID.getType();
    Offset          += (ID.getNumber() / 8);
    BitOffset        = ID.getNumber() % 8;
    Value            = pData->at(Offset);
    Value           &= ~(1 << BitOffset);
    pData->replace(Offset, Value);
}

// ************************************************************************************************

int getNextFreeNumber_Up(QVector<uint8_t>* pData, uint32_t Code)
{
    ID_Code ID(Code);
    int     Number = ID.getNumber();

    while(getStateID_Code(pData, ID.getCode()) == true)
    {
        Number++;                // We ask to increment
        if(Number >= SIZE_OF_ID_CODE_SPACE)
        {
            Number = 0;
        }
        ID.setNumber(Number);
    }
    return Number;
}

// ************************************************************************************************

int getNextFreeNumber_Down(QVector<uint8_t>* pData, uint32_t Code)
{
    ID_Code ID(Code);
    int     Number = ID.getNumber();

    while(getStateID_Code(pData, ID.getCode()) == true)
    {
        Number--;                // We ask to decrement
        if(Number < 0)
        {
            Number = SIZE_OF_ID_CODE_SPACE - 1;
        }
        ID.setNumber(Number);
    }
    return Number;
}

// ************************************************************************************************

eWidgetType getTypeImageFromText(QString ID_CodeText)
{
    eWidgetType Type;

    if((ID_CodeText.at(0) == QChar('B')) && (ID_CodeText.at(1) == QChar('K'))) Type = TYPE_BACKGROUND;
    if((ID_CodeText.at(0) == QChar('B')) && (ID_CodeText.at(1) == QChar('T'))) Type = TYPE_BUTTON;
    if((ID_CodeText.at(0) == QChar('F')) && (ID_CodeText.at(1) == QChar('T'))) Type = TYPE_FONT;
    if((ID_CodeText.at(0) == QChar('G')) && (ID_CodeText.at(1) == QChar('L'))) Type = TYPE_GLYPH;
    if((ID_CodeText.at(0) == QChar('I')) && (ID_CodeText.at(1) == QChar('C'))) Type = TYPE_ICON;
    if((ID_CodeText.at(0) == QChar('I')) && (ID_CodeText.at(1) == QChar('M'))) Type = TYPE_IMAGE;

    return Type;
}

// ************************************************************************************************

int getPointSizeFontFromText(QString ID_CodeText)
{
    int Type;

    if(ID_CodeText.compare("6") == 0) Type = 0;
    if(ID_CodeText.compare("7") == 0) Type = 1;
    if(ID_CodeText.compare("8") == 0) Type = 2;
    if(ID_CodeText.compare("9") == 0) Type = 3;
    if(ID_CodeText.compare("10") == 0) Type = 4;
    if(ID_CodeText.compare("11") == 0) Type = 5;
    if(ID_CodeText.compare("12") == 0) Type = 6;
    if(ID_CodeText.compare("14") == 0) Type = 7;
    if(ID_CodeText.compare("16") == 0) Type = 8;
    if(ID_CodeText.compare("18") == 0) Type = 9;
    if(ID_CodeText.compare("20") == 0) Type = 10;
    if(ID_CodeText.compare("22") == 0) Type = 11;
    if(ID_CodeText.compare("24") == 0) Type = 12;
    if(ID_CodeText.compare("26") == 0) Type = 13;
    if(ID_CodeText.compare("28") == 0) Type = 14;
    if(ID_CodeText.compare("32") == 0) Type = 15;
    if(ID_CodeText.compare("36") == 0) Type = 16;
    if(ID_CodeText.compare("42") == 0) Type = 17;
    if(ID_CodeText.compare("48") == 0) Type = 18;
    if(ID_CodeText.compare("56") == 0) Type = 19;
    if(ID_CodeText.compare("72") == 0) Type = 20;

    return Type;
}

// ************************************************************************************************

QImage::Format AutoSelectConversion(QComboBox* pCombo, QImage::Format Format)
{
    QImage::Format NewFormat;

    // Temporary block signal
    pCombo->blockSignals(true);

    // Autoselect best conversion
    switch(Format)
    {
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        case QImage::Format_ARGB8565_Premultiplied:
        case QImage::Format_ARGB6666_Premultiplied:
        case QImage::Format_ARGB8555_Premultiplied:
        case QImage::Format_ARGB4444_Premultiplied:
        case QImage::Format_RGBX8888:
        case QImage::Format_RGBA8888:
        case QImage::Format_RGBA8888_Premultiplied:
        {
            pCombo->setCurrentIndex(FORMAT_ARGB8888);
            NewFormat = QImage::Format_ARGB32;
            break;
        }

        //case QImage::Format_Mono:
        //case QImage::Format_MonoLSB:
        //case QImage::Format_Indexed8:
        //case QImage::Format_RGB444:
        //case QImage::Format_RGB555:
        //case QImage::Format_RGB16:
        //case QImage::Format_RGB666:
        //case QImage::Format_RGB888:
        //case QImage::Format_RGB32:
        // etc...
        default:
        {
            pCombo->setCurrentIndex(FORMAT_RGB565);
            NewFormat = QImage::Format_RGB16;
            break;
        }

    }

    // Release block signal
    pCombo->blockSignals(false);

    return NewFormat;
}

// ************************************************************************************************

void Append_uint16(QVector<uint8_t>* pData, uint16_t Value)
{
    pData->append((uint8_t)(Value >> 8));
    pData->append((uint8_t)Value);
}

// ************************************************************************************************

void Append_uint32(QVector<uint8_t>* pData, uint32_t Value)
{
    pData->append((uint8_t)(Value >> 24));
    pData->append((uint8_t)(Value >> 16));
    pData->append((uint8_t)(Value >> 8));
    pData->append((uint8_t)Value);
}

// ************************************************************************************************

void Insert_uint16(QVector<uint8_t>* pData, int i, uint16_t Value)
{
    pData->insert(i++, (uint8_t)(Value >> 8));
    pData->insert(i, (uint8_t)Value);
}

// ************************************************************************************************

void Insert_uint32(QVector<uint8_t>* pData, int i, uint32_t Value)
{
    pData->insert(i++, (uint8_t)(Value >> 24));
    pData->insert(i++, (uint8_t)(Value >> 16));
    pData->insert(i++, (uint8_t)(Value >> 8));
    pData->insert(i, (uint8_t)Value);
}

// ************************************************************************************************

uint16_t GetAt_uint16(QVector<uint8_t>* pData, int i)
{
    uint16_t Value;

    Value  = (uint16_t)pData->at(i++) << 8;
    Value |= (uint16_t)pData->at(i);

    return Value;
}

// ************************************************************************************************

uint32_t GetAt_uint32(QVector<uint8_t>* pData, int i)
{
    uint32_t Value;

    Value  = ((uint32_t)pData->at(i++) << 24);
    Value |= ((uint32_t)pData->at(i++) << 16);
    Value |= ((uint32_t)pData->at(i++) << 8);
    Value |=  (uint32_t)pData->at(i);

    return Value;
}

// ************************************************************************************************

void Replace_uint16(QVector<uint8_t>* pData, int i, uint16_t Value)
{
    pData->replace(i++, (uint8_t)(Value >> 8));
    pData->replace(i, (uint8_t)Value);
}

// ************************************************************************************************

void Replace_uint32(QVector<uint8_t>* pData, int i, uint32_t Value)
{
    pData->replace(i++, (uint8_t)(Value >> 24));
    pData->replace(i++, (uint8_t)(Value >> 16));
    pData->replace(i++, (uint8_t)(Value >> 8));
    pData->replace(i, (uint8_t)Value);
}

// ************************************************************************************************

void ChangeEndianAt_uint16(QVector<uint8_t>* pData, int i)
{
    uint16_t Value;

    Value  = (uint16_t)pData->at(i++) << 8;
    Value |= (uint16_t)pData->at(i);
    pData->replace(i--, uint8_t(Value >> 8));
    pData->replace(i,   uint8_t(Value));
}

// ************************************************************************************************

void ChangeEndianAt_uint32(QVector<uint8_t>* pData, int i)
{
    uint32_t Value;

    Value  = (uint32_t)pData->at(i++) << 24;
    Value |= (uint32_t)pData->at(i++) << 16;
    Value |= (uint32_t)pData->at(i++) << 8;
    Value |= (uint32_t)pData->at(i);
    pData->replace(i--, uint8_t(Value >> 24));
    pData->replace(i--, uint8_t(Value >> 16));
    pData->replace(i--, uint8_t(Value >> 8));
    pData->replace(i,   uint8_t(Value));
}


// ************************************************************************************************
// *
// *  Scale image
// *
// ************************************************************************************************

void ScaleToRequirement(QImage* pImage, QImage* pProcessedImage, QSize* pSize, eScaler Scale)
{
    *pProcessedImage = *pImage;

    switch(Scale)
    {
        case (int)SCALE_NONE:                                                    break;
        case (int)SCALE_FIT:    ScaleToFit(pImage,    pProcessedImage, pSize);   break;
        case (int)SCALE_WIDTH:  ScaleToWidth(pImage,  pProcessedImage, pSize);   break;
        case (int)SCALE_HEIGHT: ScaleToHeight(pImage, pProcessedImage, pSize);   break;
        default:                                                                 break;            // no Scaling
    }
}

// ************************************************************************************************

void ScaleToFit(QImage* pImage, QImage* pProcessedImage, QSize* pSize)
{
    //ScaleToWidth(pImage,    pProcessedImage, pSize);
    //ScaleToHeight(pImage,    pProcessedImage, pSize);

    if((pImage->width() >  pSize->width()) ||
       (pImage->height() >  pSize->height()))
    {
        *pProcessedImage = pImage->scaled(*pSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
}

// ************************************************************************************************

void ScaleToWidth(QImage* pImage, QImage* pProcessedImage, QSize* pSize)
{
    if(pImage->width() >  pSize->width())
    {
        *pProcessedImage = pImage->scaledToWidth(pSize->width(), Qt::SmoothTransformation);
    }
}

// ************************************************************************************************

void ScaleToHeight(QImage* pImage, QImage* pProcessedImage, QSize* pSize)
{
    if(pImage->height() > pSize->height())
    {
        *pProcessedImage = pImage->scaledToHeight(pSize->height(), Qt::SmoothTransformation);
    }
}


// ************************************************************************************************
// *
// *  XML support function
// *
// ************************************************************************************************

void SavePathToXML(QString Path)
{
    if(GetPathFromXML().compare(Path) != 0)
    {
        QXmlPut xmlPut("SkinAssembler");

        xmlPut.putString("PATH", Path);
        xmlPut.save("SA_Path.xml");
    }
}

// ************************************************************************************************

QString GetPathFromXML()
{
    QString Path;
    QXmlGet xmlGet;

    xmlGet.load("SA_Path.xml");

    if(xmlGet.find("PATH"))
    {
        Path = xmlGet.getString();
    }

    return Path;
}

// ************************************************************************************************

void SaveSkinPathToXML(QString Path)
{
    if(GetPathFromXML().compare(Path) != 0)
    {
        QXmlPut xmlPut("SkinAssembler");

        xmlPut.putString("SkinPath", Path);
        xmlPut.save("SKN_Path.xml");
    }
}

// ************************************************************************************************

QString GetSkinPathFromXML()
{
    QString Path;
    QXmlGet xmlGet;

    xmlGet.load("SKN_Path.xml");

    if(xmlGet.find("SkinPath"))
    {
        Path = xmlGet.getString();
    }

    return Path;
}

// ************************************************************************************************

void SaveDisplaySizeToXML(QSize Size)
{
    QXmlPut xmlPut("SkinAssembler");

    xmlPut.descend("SizeDisplay");
    xmlPut.putInt("width", Size.width());
    xmlPut.putInt("height", Size.height());
    xmlPut.rise();

    xmlPut.save("SA_DisplaySize.xml");
}

// ************************************************************************************************

QSize GetDisplaySizeFromXML()
{
    QSize Size;
    QXmlGet xmlGet;

    xmlGet.load("SA_DisplaySize.xml");

    if(xmlGet.findNext("SizeDisplay"))
    {
        xmlGet.descend();
        if (xmlGet.find("width"))   Size.setWidth(xmlGet.getInt());
        if (xmlGet.find("height"))  Size.setHeight(xmlGet.getInt());
    }

    return Size;
}

// ************************************************************************************************

void SaveEndianToXML(eEndianess Endian)
{
    QXmlPut xmlPut("SkinAssembler");

    xmlPut.putString("Endian", (Endian == LITTLE_ENDIAN) ? "LITTLE" :"BIG");

    xmlPut.save("SA_Endian.xml");
}

// ************************************************************************************************

eEndianess GetEndianFromXML()
{
    eEndianess Endian;
    QXmlGet xmlGet;
    QString EndianName;

    xmlGet.load("SA_Endian.xml");

    if(xmlGet.find("Endian"))
    {
        EndianName = xmlGet.getString();
    }

    if(EndianName == "BIG") Endian = BIG_ENDIAN;
    else                    Endian = LITTLE_ENDIAN;

    return Endian;
}

// ************************************************************************************************

// Class ID_Code

ID_Code::ID_Code(uint32_t Code)
{
    m_Code = Code;
    codeToText();
    codeToElement();
}

// ************************************************************************************************

ID_Code::ID_Code(QString CodeText)
{
    setCodeText(CodeText);
}

// ************************************************************************************************

ID_Code::ID_Code(eWidgetType Type, int Number)
{
    m_Type   = Type;
    m_Number = Number;
    elementToText();
    textToCode();
}

// ************************************************************************************************

void ID_Code::setNumber(int Number)
{
    m_Number = Number;
    elementToText();
    textToCode();
}

// ************************************************************************************************

void ID_Code::setCodeText(QString CodeText)
{
    m_CodeText = CodeText;
    textToCode();
    codeToElement();
}

// ************************************************************************************************

void ID_Code::textToCode()
{
    if(m_CodeText.length() >= 6)
    {
        m_Code  =  (m_CodeText.at(0).toLatin1() - 'A') << 22;
        m_Code |= ((m_CodeText.at(1).toLatin1() - 'A') << 16);
        m_Code |= ((m_CodeText.at(2).toLatin1() - '0') << 12);
        m_Code |= ((m_CodeText.at(3).toLatin1() - '0') << 8);
        m_Code |= ((m_CodeText.at(4).toLatin1() - '0') << 4);
        m_Code |= ((m_CodeText.at(5).toLatin1() - '0'));
    }
    else
    {
        m_Code = 0;
    }
}

// ************************************************************************************************

void ID_Code::codeToText()
{
    m_CodeText.append(QChar( (m_Code >> 22  & 0x3F) + 'A'));
    m_CodeText.append(QChar(((m_Code >> 16) & 0x3F) + 'A'));
    m_CodeText.append(QChar(((m_Code >> 12) & 0x0F) + '0'));
    m_CodeText.append(QChar(((m_Code >> 8)  & 0x0F) + '0'));
    m_CodeText.append(QChar(((m_Code >> 4)  & 0x0F) + '0'));
    m_CodeText.append(QChar(((m_Code) & 0x0F) + '0'));
}

// ************************************************************************************************

void ID_Code::codeToElement()
{
    // Find type
    if((m_CodeText.at(0) == QChar('B')) && (m_CodeText.at(1) == QChar('K'))) m_Type = TYPE_BACKGROUND;
    if((m_CodeText.at(0) == QChar('B')) && (m_CodeText.at(1) == QChar('T'))) m_Type = TYPE_BUTTON;
    if((m_CodeText.at(0) == QChar('F')) && (m_CodeText.at(1) == QChar('T'))) m_Type = TYPE_FONT;
    if((m_CodeText.at(0) == QChar('G')) && (m_CodeText.at(1) == QChar('L'))) m_Type = TYPE_GLYPH;
    if((m_CodeText.at(0) == QChar('I')) && (m_CodeText.at(1) == QChar('C'))) m_Type = TYPE_ICON;
    if((m_CodeText.at(0) == QChar('I')) && (m_CodeText.at(1) == QChar('M'))) m_Type = TYPE_IMAGE;

    // Find Number
    m_Number  = ((m_Code >> 12) & 0x0F) * 1000;
    m_Number += ((m_Code >> 8) & 0x0F)  * 100;
    m_Number += ((m_Code >> 4) & 0x0F)  * 10;
    m_Number += ((m_Code >> 0) & 0x0F);

    // Find prefix
    m_Prefix.clear();
    m_Prefix.append(m_CodeText.at(0));
    m_Prefix.append(m_CodeText.at(1));
}

// ************************************************************************************************

void ID_Code::elementToText()
{
    if(m_Type == TYPE_BACKGROUND) m_Prefix = "BK";
    if(m_Type == TYPE_BUTTON)     m_Prefix = "BT";
    if(m_Type == TYPE_FONT)       m_Prefix = "FT";
    if(m_Type == TYPE_GLYPH)      m_Prefix = "GL";
    if(m_Type == TYPE_ICON)       m_Prefix = "IC";
    if(m_Type == TYPE_IMAGE)      m_Prefix = "IM";

    m_CodeText = m_Prefix;
    m_CodeText.append(QString("%1").arg(m_Number, 4, 10, QChar('0')));
}

// ************************************************************************************************

