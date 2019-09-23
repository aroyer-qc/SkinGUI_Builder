#ifndef __ADDING_IMAGE_H__
#define __ADDING_IMAGE_H__

#include <QDialog>
#include <QtWidgets>
#include "typedef.h"

namespace Ui {
class AddingImage;
}

class AddingImage : public QDialog
{
        Q_OBJECT

    public:
        explicit AddingImage(eCaller Caller, QString Path, QSize Size, QWidget *parent = 0);
        ~AddingImage();

    signals:

        void AddImage(sLoadingInfo LoadingInfo);                // Signal for Image file
        void CloseAddImage();                                   // signal to Main app to close this form

    private slots:

        void on_ButtonBrowse_clicked();
        void on_pushButtonAdd_clicked();
        void on_pushButtonClose_clicked();
        void on_ComboBoxDirectory_currentIndexChanged(int index);
        void on_TableFilesFound_cellClicked(int row, int column);
        void on_TableFilesFound_cellActivated(int row, int column);
        void on_TableFilesFound_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
        void on_comboBoxResize_currentIndexChanged(int index);
        void on_comboBoxPixelFormat_currentIndexChanged(int index);
        void on_verticalScrollBar_valueChanged(int value);
        void on_horizontalScrollBar_valueChanged(int value);

        void on_loadCheckerBoardSlider_sliderMoved(int position);

   private:

        void Find();
        void ImageSelected();
        void ClearScene();
        void ResetLoadGUI();
        void ReloadingImage();
        void LoadingImage(int Row, eResizer Resizer);
        void ResizeForm();
        void ClearScrollBarValue();


        Ui::AddingImage*       ui;
        QGraphicsScene          m_Scene;
        QString                 m_Filename;
        QString                 m_PathAndFilename;
        QDir                    m_currentDir;
        int                     m_FileFound;
        size_t                  m_TotalCount;
        QImage*                 m_pImage;
        QImage*                 m_pProcessedImage;
        QImage::Format          m_PixelFormat;
        QGraphicsPixmapItem*    m_pPixmapItem;
        QSize                   m_Size;
        eCaller                 m_Caller;
        int                     m_verticalScrollRange;
        int                     m_horizontalScrollRange;
};

#endif // ADDING_IMAGE_H
