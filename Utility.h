#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <QtWidgets>
#include <QImage>
#include "typedef.h"

void                    UpdateComboBox                      (QComboBox* comboBox);
QString                 GetFormat                           (QImage::Format format);
QPoint                  CenterPoint                         (QSize Size, QSize MaxSize);
void                    CheckerPattern                      (QGraphicsScene* pScene);
QString                 PrintSize                           (QSize Size);
QString                 PrintFileSize                       (int Count);
QSize                   GetSizeFromGraphicsView             (QGraphicsView* pGraphView, QSize Size);
void                    setWidgetXY                         (QWidget* pWidget, int X, int Y);
void                    setWidgetSize                       (QWidget* pWidget, int Width, int Height);
QPoint                  SelectScreenAndCenter               (QSize Size);
void                    ClearTable                          (QTableWidget* pTable);
QImage::Format          AutoSelectConversion                (QComboBox* pCombo, QImage::Format Format);


// ---------- Support function for ID_Code ----------

bool                    getStateID_Code                     (QVector<uint8_t>* pData, class ID_Code ID);
void                    setStateID_Code                     (QVector<uint8_t>* pData, class ID_Code ID);
void                    clearStateID_Code                   (QVector<uint8_t>* pData, class ID_Code ID);
int                     getNextFreeNumber_Up                (QVector<uint8_t>* pData, uint32_t ID_Code);
int                     getNextFreeNumber_Down              (QVector<uint8_t>* pData, uint32_t ID_Code);
eWidgetType             getTypeImageFromText                (QString ID_CodeText);
int                     getPointSizeFontFromText            (QString ID_CodeText);

// ---------- Utility to add other type to uint8_t Vector ----------

uint16_t                GetAt_uint16                        (QVector<uint8_t>* pData, int i);
uint32_t                GetAt_uint32                        (QVector<uint8_t>* pData, int i);
void                    Append_uint16                       (QVector<uint8_t>* pData, uint16_t Value);
void                    Append_uint32                       (QVector<uint8_t>* pData, uint32_t Value);
void                    Insert_uint16                       (QVector<uint8_t>* pData, int i, uint16_t Value);
void                    Insert_uint32                       (QVector<uint8_t>* pData, int i, uint32_t Value);
void                    Replace_uint16                      (QVector<uint8_t>* pData, int i, uint16_t Value);
void                    Replace_uint32                      (QVector<uint8_t>* pData, int i, uint32_t Value);
void                    ChangeEndianAt_uint16               (QVector<uint8_t>* pData, int i);
void                    ChangeEndianAt_uint32               (QVector<uint8_t>* pData, int i);

// ---------- Load Image in a QImage ----------

void                    LoadImageFromFile                   (QImage* pImage, QString PathAndFile);

// ---------- Scaling Image Support Function ----------

void                    ScaleToRequirement                  (QImage* pImage, QImage* pProcessedImage, QSize* pSize, eScaler Scale);
void                    ScaleToFit                          (QImage* pImage, QImage* pProcessedImage, QSize* pSize);
void                    ScaleToWidth                        (QImage* pImage, QImage* pProcessedImage, QSize* pSize);
void                    ScaleToHeight                       (QImage* pImage, QImage* pProcessedImage, QSize* pSize);

// ---------- XML Support Function ----------

void                    SavePathToXML                       (QString Path);
QString                 GetPathFromXML                      ();
void                    SaveSkinPathToXML                   (QString Path);
QString                 GetSkinPathFromXML                  ();
void                    SaveDisplaySizeToXML                (QSize Size);
QSize                   GetDisplaySizeFromXML               ();
void                    SaveEndianToXML                     (eEndianess Endian);
eEndianess              GetEndianFromXML                    ();

// ---------- class for ID_Code ----------

class ID_Code
{
    public:

                    ID_Code         (uint32_t Code);
                    ID_Code         (QString CodeText);
                    ID_Code         (eWidgetType Type, int Number);

                    QString     getCodeText     () {return m_CodeText;}
                    QString     getPrefix       () {return m_Prefix;}
                    uint32_t    getCode         () {return m_Code;}
                    int         getNumber       () {return m_Number;}
                    eWidgetType getType         () {return m_Type;}
                    void        setNumber       (int Number);
                    void        setCodeText     (QString CodeText);

                private:

                    void        textToCode      ();
                    void        codeToText      ();
                    void        codeToElement   ();
                    void        elementToText   ();


        QString         m_CodeText;
        QString         m_Prefix;
        uint32_t        m_Code;
        int             m_Number;
        eWidgetType     m_Type;
};


#endif