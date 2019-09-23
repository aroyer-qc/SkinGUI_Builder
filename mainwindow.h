#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QDir>
#include <QtWidgets>
#include "AddingImage.h"
#include "AddingAudio.h"
#include "progress.h"
#include "ComboBoxDelegate.h"
#include "SpinBoxDelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        QVector<uint8_t>*       getRawDataPtr()                         { return &m_RawImage; }
        QVector<sImageInfo>*    getImageInfoPtr()                       { return &m_ImageInfo; }
        QVector<QFont>*         getFontInfoPtr()                        { return &m_Font; }
        QVector<sAudioInfo>*    getAudioInfoPtr()                       { return &m_AudioInfo; }
        QVector<uint8_t>*       getFontSamplingInfoPtr()                { return &m_SamplingFont; }
        eEndianess*             getEndianPtr()                          { return &m_Endian; }

    signals:

    public slots:

        void CodeLabel(eCaller Caller, int Item, QString CodeLabel);
        void SetSizeDisplay(QSize Size);
        void SetEndianess(eEndianess Endian);

        // Public slot for file operation
        void on_OpenDone();
        void on_SaveDone();

        // Public slot for Image
        void AddImage(sLoadingInfo LoadingInfo);
        void CloseAddImage();

        // Public slot for Audio
        void AddAudio(sLoadingInfo LoadingInfo);
        void CloseAddAudio();

    private slots:

        // Slot for Image tab
        void on_ButtonDownImage_clicked();
        void on_ButtonUpImage_clicked();
        void on_ButtonAddImage_clicked();
        void on_ButtonRemoveImage_clicked();
        void on_TableImage_cellClicked(int row, int column);
        void on_TableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
        void on_TableImage_cellDoubleClicked(int row, int column);
        void on_TableImage_itemChanged(QTableWidgetItem *item);
        void on_ImageCheckerBoardSlider_sliderMoved(int position);

        // Slot for Font tab
        void on_ButtonAddFont_clicked();
        void on_ButtonRemoveFont_clicked();
        void on_ButtonUpFont_clicked();
        void on_ButtonDownFont_clicked();
        void on_TableFont_cellActivated(int row, int column);
        void on_TableFont_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
        void on_TableFont_cellClicked(int row, int column);
        void on_TableFont_cellDoubleClicked(int row, int column);
        void on_TableFont_itemChanged(QTableWidgetItem *item);
        void on_checkBoxAlphaFont_clicked(bool checked);
        void on_checkBoxNumericFont_clicked(bool checked);
        void on_checkBoxSymbolFont_clicked(bool checked);
        void on_checkBoxExtraSymbolFont_clicked(bool checked);
        void on_checkBoxLatinFont_clicked(bool checked);
        void on_checkBoxBold_clicked(bool checked);
        void on_checkBoxItalic_clicked(bool checked);
        void on_checkBoxFixed_clicked(bool checked);

        // Slot for Audio tab
        void on_ButtonUpAudio_clicked();
        void on_ButtonDownAudio_clicked();
        void on_ButtonAddAudio_clicked();
        void on_ButtonRemoveAudio_clicked();
        //void on_TableAudio_cellClicked(int row, int column);
        //void on_TableAudio_cellDoubleClicked(int row, int column);
        //void on_TableAudio_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
        //void on_TableAudio_itemChanged(QTableWidgetItem *item);
        
        // Slot for Converter
        void on_ButtonBrowse_clicked();
        void on_ButtonConvert_clicked();
        void on_TableFilesFound_cellActivated(int row, int column);
        void on_TableFilesFound_cellClicked(int row, int column);
        void on_TableFilesFound_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
        void on_ComboBoxDirectory_currentIndexChanged(int index);
        void on_horizontalScrollBarConverter_valueChanged(int value);
        void on_verticalScrollBarConverter_valueChanged(int value);

        // Menu action
        void on_actionSave_Skin_triggered();
        void on_actionSave_Skin_As_triggered();
        void on_actionNew_Skin_triggered();
        void on_actionOpen_Skin_triggered();
        void on_actionSet_Display_Size_triggered();
        void on_comboBoxResize_currentIndexChanged(int index);
        void on_comboBoxPixelFormat_currentIndexChanged(int index);

        void on_actionExit_triggered();
        void on_actionSet_Endianess_triggered();

        void on_CheckerBoardSlider_sliderMoved(int position);

   private:

        void     ClearAllData            ();
        void     ResetAllSkinTab         ();
        void     UpdateStatusBar         ();
        void     closeEvent              (QCloseEvent *bar);

        // Function for file operation
        void     Open                    (QString File);
        void     Save                    ();
        bool     SaveSkinAndClearData    ();
        void     setSkinHasUnsavedData   (bool state);

        // Function for Image
        void     InitImage               ();
        void     ResetImageGUI           ();
        void     UpdateImageGUI          (int row);
        void     ClearSceneImage         ();
        void     AdjustTabImage          (QSize Offset, QRect ViewRect);
        void     InsertNewRowInTableImage(int row, QString File, QString CodeType, QString CodeID, QString Size);

        // Function for Font
        void     InitFont                ();
        void     ResetFontGUI            ();
        void     UpdateFontGUI           (int row);
        void     ClearSceneFont          ();
        void     LoadFont                (int row);
        void     SetTableProperties      (int row);
        void     InsertNewRowInTableFont (int row, QString Name, QString Size, QString Properties);
        void     CheckBoxActivation      (bool state);
        void     DisplayExample          (int row);

        // Function for Audio
        void     InitAudio               ();
        void     ResetAudioGUI           ();
        void     UpdateAudioGUI          (int row);
        void     AdjustTabAudio          (QSize Offset, QRect ViewRect);
        void     InsertNewRowInTableAudio(int row, QString File, uint16_t SamplingRate, time_t Duration, QString Size);

        // Function for Converter
        void     InitConverter           ();
        void     ResetConverterGUI       ();
        void     ClearSceneConverter     ();
        void     Extract                 (QTextStream* pStream, int Index);
        void     Find                    ();
        void     AdjustTabConverter      (QSize Offset, QRect ViewRect);
        void     ClearScrollBarValue     ();
        void     ReloadImageConverter    ();
        void     LoadImageConverter      (int row, eResizer Resizer);
        void     BinToFile               (QTextStream* pStream, QString pFileName);

        Ui::MainWindow*         ui;
        eEndianess              m_Endian;

        // Layout info
        QSize                   m_DisplaySize;
        QLabel*                 m_pStatusLabel;
        QRect                   m_ViewRect;
        QRect                   m_TabRect;
        QRect                   m_MainRect;
        QString                 m_SkinName;
        int                     m_SkinSize;

        QVector<uint8_t>*       m_pInUseCode;

        // Data of the Skin
        //QVector<wchar_t>        m_RawLabel;                     // Raw label data
        //QVector<uint32_t>       m_Widget;                       // remove all widget
        //QVector<uint32_t>       m_WidgetIndex;                  // Quick index to recover Widget
        //sLoadingInfo            m_LoadingInfo;

        // File operation
        QDir                    m_currentDir;
        QDir                    m_SkinDir;
        AddingImage*            m_pLoadImage;
        AddingAudio*            m_pLoadAudio;
        QThread*                m_pSkinSave;
        QThread*                m_pSkinOpen;
        bool                    m_IsSkinSaveAs;                 // Save As or Save
        bool                    m_IsSkinHasUnsavedData;         // is there any unsaved data
        bool                    m_IsWarningDisplayed;
        bool                    m_IsNeedCompleteFileOpen;
        bool                    m_IsNeedToClearDataAfterSave;
        bool                    m_IsNeedToCloseAfterSave;
        int                     m_ProgressSave;
        int                     m_ProgressOpen;
        Progress*               m_pProgress;


        // Variable for Image Tab
        QGraphicsScene          m_SceneImage;
        QVector<sImageInfo>     m_ImageInfo;                    // Array of image structure information
        QVector<uint8_t>        m_RawImage;                     // Raw image data
        ComboBoxDelegate*       m_pImageComboBoxDelegate;
        SpinBoxDelegate*        m_pImageSpinBoxDelegate;

        // Variable for Font Tab
        QGraphicsScene          m_SceneExample;
        QGraphicsScene          m_SceneFont;
        QVector<QFont>          m_Font;
        QVector<uint8_t>        m_SamplingFont;
        ComboBoxDelegate*       m_pFontComboBoxDelegate;

        // Variable for Audio Tab
        QVector<sAudioInfo>     m_AudioInfo;                    // Array of audio structure information
        QVector<uint8_t>        m_RawAudio;                     // Raw audio data

        // Variable for Converter Tab
        QGraphicsScene          m_SceneConverter;
        QGraphicsPixmapItem*    m_pPixmapItem;
        QImage::Format          m_PixelFormatConverter;
        int                     m_verticalScrollRange;
        int                     m_horizontalScrollRange;
        int                     m_FileFound;
        QImage*                 m_pImage;
        QImage*                 m_pProcessedImage;
        QSize                   m_Scale;
        size_t                  m_TotalCount;
        size_t                  m_FileSize;
};

#endif // MAINWINDOW_H
