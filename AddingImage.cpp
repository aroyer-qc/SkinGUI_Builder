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

#include <QDir>
#include "AddingImage.h"
#include "ui_AddingImage.h"
#include "mainwindow.h"
#include "Utility.h"

// ************************************************************************************************

AddingImage::AddingImage(eCaller Caller, QString Path, QSize Size, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddingImage)
{
    this->setModal(true);
    ui->setupUi(this);
    //setStyleSheet("background-image: url(:/Images/Marble.jpg);");
    setSizeGripEnabled(false);
    m_currentDir.setPath(Path);
    m_pPixmapItem     = NULL;
    m_Caller          = Caller;
    m_Size            = Size;
    m_pImage          = NULL;
    m_pProcessedImage = NULL;

    ui->ComboBoxDirectory->blockSignals(true);
    ui->ComboBoxDirectory->setCurrentText(Path);
    UpdateComboBox(ui->ComboBoxDirectory);
    ui->ComboBoxDirectory->blockSignals(false);

    m_PixelFormat = QImage::Format_RGB16;
    ui->comboBoxPixelFormat->blockSignals(true);
    ui->comboBoxPixelFormat->setCurrentIndex(FORMAT_RGB565);
    ui->comboBoxPixelFormat->blockSignals(false);

    ui->comboBoxResize->setCurrentIndex(SCALE_FIT);

    ui->horizontalScrollBar->setRange     (0, 0);
    ui->verticalScrollBar->setRange       (0, 0);
    ui->horizontalScrollBar->setSingleStep(0);
    ui->verticalScrollBar->setSingleStep  (0);

    ui->LabelNote->setVisible(false);

    ResizeForm();
    ResetLoadGUI();

    Find();
}

// ************************************************************************************************

AddingImage::~AddingImage()
{
    if(m_pImage          != NULL)       delete m_pImage;
    if(m_pProcessedImage != NULL)       delete m_pProcessedImage;

    delete ui;
}

// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************
void AddingImage::on_pushButtonAdd_clicked()
{
    ImageSelected();
}

// ************************************************************************************************

void AddingImage::on_pushButtonClose_clicked()
{
    CloseAddImage();
}

// ************************************************************************************************

void AddingImage::on_ButtonBrowse_clicked()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Find Images Files"), m_currentDir.absolutePath());

    if (!directory.isEmpty())
    {
        if(ui->ComboBoxDirectory->findText(directory) == -1)
        {
            ui->ComboBoxDirectory->addItem(directory);
        }
        ui->ComboBoxDirectory->setCurrentIndex(ui->ComboBoxDirectory->findText(directory));
    }
}

// ************************************************************************************************

void AddingImage::on_ComboBoxDirectory_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_currentDir = QDir(ui->ComboBoxDirectory->currentText());
    SavePathToXML(m_currentDir.absolutePath());
    Find();
}

// ************************************************************************************************

void AddingImage::on_TableFilesFound_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    if(currentRow >= 0)
    {
        LoadingImage(currentRow, AUTO_FORMAT);
    }
}

// ************************************************************************************************

void AddingImage::on_TableFilesFound_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    LoadingImage(row, AUTO_FORMAT);
}

// ************************************************************************************************

void AddingImage::on_TableFilesFound_cellActivated(int row, int column)
{
    // Double click on a line will automatically select this file
    Q_UNUSED(row);
    Q_UNUSED(column);
    ImageSelected();
}

// ************************************************************************************************

void AddingImage::on_comboBoxResize_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ReloadingImage();
}

// ************************************************************************************************

void AddingImage::on_comboBoxPixelFormat_currentIndexChanged(int index)
{
    if(index == FORMAT_RGB565) m_PixelFormat = QImage::Format_RGB16;
    else                       m_PixelFormat = QImage::Format_ARGB32;
    ReloadingImage();
}

// ************************************************************************************************

void AddingImage::on_horizontalScrollBar_valueChanged(int value)
{
    int PosX = value / 100;
    int PosY = m_pPixmapItem->y();
    m_pPixmapItem->setPos(-PosX, PosY);
}

// ************************************************************************************************

void AddingImage::on_verticalScrollBar_valueChanged(int value)
{
    int PosX = m_pPixmapItem->x();
    int PosY = value / 100;
    m_pPixmapItem->setPos(PosX, -PosY);
}

// ************************************************************************************************

void AddingImage::on_loadCheckerBoardSlider_sliderMoved(int position)
{
   ui->graphicsView->setStyleSheet(QString("background-color: #%1;").arg(position + (position << 8) + (position << 16), 6, 16, QChar('0')));
   CheckerPattern(&m_Scene);
}

// ************************************************************************************************
// *
// *  Function
// *
// ************************************************************************************************

void AddingImage::Find()
{
    QStringList files;
    QStringList filters;

    ResetLoadGUI();

    filters << "*.png" << "*.bmp" << "*.jpg";
    m_currentDir.setNameFilters(filters);

    files = m_currentDir.entryList(filters, QDir::Files | QDir::NoSymLinks);

    m_FileFound = files.size();

    ui->TableFilesFound->blockSignals(true);
    for(int i = 0; i < m_FileFound; ++i)
    {
        QFile file(m_currentDir.absoluteFilePath(files[i]));
        qint64 size = QFileInfo(file).size();

        QTableWidgetItem *fileNameItem = new QTableWidgetItem(files[i]);
        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(PrintFileSize(size));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        int row = ui->TableFilesFound->rowCount();
        ui->TableFilesFound->insertRow(row);
        ui->TableFilesFound->setItem(row, 0, fileNameItem);
        ui->TableFilesFound->setItem(row, 1, sizeItem);
    }
    ui->TableFilesFound->blockSignals(false);

    if(m_FileFound)
    {
        ui->LabelFilesFound->setText(tr("%1 file(s) found").arg(m_FileFound));
    }
    else
    {
        ui->LabelFilesFound->setText(tr("0 file found"));
    }
}


// ************************************************************************************************

void AddingImage::ImageSelected()
{
    sLoadingInfo LoadingInfo;

    LoadingInfo.Offset.setX(ui->horizontalScrollBar->value() / 100);
    LoadingInfo.Offset.setY(ui->verticalScrollBar->value()   / 100);
    LoadingInfo.DataSize        = m_TotalCount;                          // Size of the raw data
    LoadingInfo.Size            = m_pProcessedImage->size();             // Dimension of the image (width & height)
    LoadingInfo.PixelFormat     = (ui->comboBoxPixelFormat->currentIndex() == FORMAT_RGB565) ? QImage::Format_RGB16 : QImage::Format_ARGB32;
    LoadingInfo.Filename        = m_Filename;
    LoadingInfo.PathAndFilename = m_PathAndFilename;
    LoadingInfo.ScaleType       = (eScaler)ui->comboBoxResize->currentIndex();

    emit AddImage(LoadingInfo);
}

// ************************************************************************************************

void AddingImage::ClearScene()
{
    if(m_pPixmapItem != NULL)
    {
        m_Scene.removeItem(m_pPixmapItem);
        m_pPixmapItem = NULL;
    }

    m_Scene.clear();
    ui->graphicsView->setScene(&m_Scene);
    CheckerPattern(&m_Scene);
    ui->graphicsView->setSceneRect(0,0,m_Size.width(), m_Size.height());
    ui->graphicsView->setScene(&m_Scene);
}

// ************************************************************************************************

void AddingImage::ResetLoadGUI()
{
    ui->pushButtonAdd->setDisabled(true);
    ui->LabelFilename->setText("");
    ui->LabelDataSize->setText("");
    ui->LabelPixelFormat->setText("");
    ui->LabelSize->setText("");
    ui->LabelScaleSize->setText("");
    ClearTable(ui->TableFilesFound);
    ClearScene();
}

// ************************************************************************************************

void AddingImage::ReloadingImage()
{
    int row = ui->TableFilesFound->currentRow();
    if(row >= 0)
    {
        LoadingImage(row ,RELOAD_FORMAT);
    }
}

// ************************************************************************************************

void AddingImage::LoadingImage(int row, eResizer Resizer)
{
    QTableWidgetItem *item;

    ui->pushButtonAdd->setEnabled(true);
    item = ui->TableFilesFound->item(row, 0);
    m_Filename        = item->text();
    m_PathAndFilename = m_currentDir.absoluteFilePath(m_Filename);

    if(m_pImage != NULL)
    {
        delete m_pImage;
        m_pImage = NULL;
    }

    m_pImage = new QImage();
    m_pImage->load(m_PathAndFilename);

    // Information that do not change if setting change
    ui->LabelFilename->setText(item->text());
    ui->LabelSize->setText(PrintSize(m_pImage->size()));

    if(Resizer == AUTO_FORMAT)
    {
        m_PixelFormat = AutoSelectConversion(ui->comboBoxPixelFormat, m_pImage->format());
    }

    //     Redraw Image
    QPoint  Point;
    QSize   Size;
    QImage* pResizedImage;

    ClearScene();

    pResizedImage = new QImage();
    ScaleToRequirement(m_pImage, pResizedImage, &m_Size, (eScaler)ui->comboBoxResize->currentIndex());

    if(m_pProcessedImage != NULL)
    {
        delete m_pProcessedImage;
        m_pProcessedImage = NULL;
    }
    m_pProcessedImage = new QImage();
    *m_pProcessedImage = pResizedImage->convertToFormat(m_PixelFormat);
    delete pResizedImage;

    // Set the size for the image info
    Size.setWidth ((m_pProcessedImage->width()  > m_Size.width())  ? m_Size.width()  : m_pProcessedImage->width());
    Size.setHeight((m_pProcessedImage->height() > m_Size.height()) ? m_Size.height() : m_pProcessedImage->height());

    m_TotalCount  = m_pProcessedImage->byteCount() / (m_pProcessedImage->width() * m_pProcessedImage->height());    // Adjust byte count to image size in viewport if it is the case
    m_TotalCount *= (Size.width() * Size.height());

    ui->LabelPixelFormat->setText(GetFormat(m_pImage->format()));
    ui->LabelDataSize->setText(QString("%1 Bytes").arg(m_TotalCount));                                              // Print updated value in information
    ui->LabelScaleSize->setText(PrintSize(Size));                                                                   // Display viewport size

    // Set the size for the scene (minimum size is viewport)
    Size.setWidth ((m_pProcessedImage->width()  < m_Size.width())  ? m_Size.width()  : m_pProcessedImage->width());
    Size.setHeight((m_pProcessedImage->height() < m_Size.height()) ? m_Size.height() : m_pProcessedImage->height());

    if((Size.width()  > m_Size.width()) ||                                                                          // Display special note if image is bigger than viewport
       (Size.height() > m_Size.height()))
    {
        ui->LabelNote->setVisible(true);
    }
    else
    {
        ui->LabelNote->setVisible(false);
    }

    ui->horizontalScrollBar->blockSignals(true);
    ui->verticalScrollBar->blockSignals(true);
    ui->horizontalScrollBar->setValue(0);
    ui->verticalScrollBar->setValue(0);
    m_horizontalScrollRange = (m_pProcessedImage->size().width()  - m_Size.width())  * 100;
    m_verticalScrollRange   = (m_pProcessedImage->size().height() - m_Size.height()) * 100;
    ui->horizontalScrollBar->setRange(0, m_horizontalScrollRange);
    ui->verticalScrollBar->setRange  (0, m_verticalScrollRange);
    ui->horizontalScrollBar->setSingleStep(m_horizontalScrollRange / 10);
    ui->verticalScrollBar->setSingleStep  (m_verticalScrollRange / 10);
    ui->horizontalScrollBar->blockSignals(false);
    ui->verticalScrollBar->blockSignals(false);

    m_pPixmapItem = m_Scene.addPixmap(QPixmap::fromImage(*m_pProcessedImage));                                          // Add the image on top of the checker pattern
    Point = CenterPoint(m_pProcessedImage->size(), m_Size);
    m_pPixmapItem->setPos(Point);
}

// ************************************************************************************************

void AddingImage::ResizeForm()
{
    QRect   MainRect = this->geometry();;
    QRect   ViewRect = ui->graphicsView->geometry();
    int     AddVertical;
    int     AddHorizontal;

    // X pos of GraphicsView realigned
    if(m_Size.width() > MINIMUM_PREVIEW_WIDTH)
    {
        AddHorizontal = (m_Size.width() - MINIMUM_PREVIEW_WIDTH);
        MainRect.setWidth(MainRect.width() + AddHorizontal);
        ViewRect.setWidth(ViewRect.width() + AddHorizontal);
    }
    else
    {
        ViewRect.moveLeft(ViewRect.x() + ((MINIMUM_PREVIEW_WIDTH / 2) - (m_Size.width() / 2)));
        ViewRect.setWidth((ViewRect.width() - MINIMUM_PREVIEW_WIDTH) +  m_Size.width());
    }

    // Y pos of GraphicsView realigned
    if(m_Size.height() > MINIMUM_PREVIEW_HEIGHT)
    {
        AddVertical = (m_Size.height() - MINIMUM_PREVIEW_HEIGHT);
        MainRect.setHeight(MainRect.height() + AddVertical);
        ViewRect.setHeight(ViewRect.height() + AddVertical);
    }
    else
    {
        ViewRect.moveTop(ViewRect.y() + ((MINIMUM_PREVIEW_HEIGHT / 2) - (m_Size.height() / 2)));
        ViewRect.setHeight((ViewRect.height() - MINIMUM_PREVIEW_HEIGHT) +  m_Size.height());
    }

    // Set Size of the main window and QGraphicsView

    this->setMinimumSize(0, 0);
    this->setMaximumSize(2048, 2048);
    this->setGeometry(MainRect);
    this->setMinimumSize(MainRect.size().width(), MainRect.size().height());
    this->setMaximumSize(MainRect.size().width(), MainRect.size().height());
    ui->graphicsView->setGeometry(ViewRect);

    // Resize tab
    QSize Offset = GetSizeFromGraphicsView(ui->graphicsView, m_Size);

    // Resize Vertical line
    setWidgetSize(ui->line_V_Load,              -1,                                     Offset.height() + 196);

    // Resize Horizontal Line
    setWidgetSize(ui->line_H_Load,              Offset.width() + 10,                    -1);

    // Reposition Horizontal Line
    setWidgetXY(ui->line_H_Load,                -1,                                     Offset.height() + 75);

    // Reposition Label
    setWidgetXY(ui->LabelStaticFilename,        -1,                                     Offset.height() + 90);
    setWidgetXY(ui->LabelFilename,              -1,                                     Offset.height() + 90);
    setWidgetXY(ui->LabelStaticDataSize,        -1,                                     Offset.height() + 120);
    setWidgetXY(ui->LabelDataSize,              -1,                                     Offset.height() + 120);
    setWidgetXY(ui->LabelStaticPixelFormat,     -1,                                     Offset.height() + 150);
    setWidgetXY(ui->LabelPixelFormat,           -1,                                     Offset.height() + 150);
    setWidgetXY(ui->LabelStaticSize,            -1,                                     Offset.height() + 180);
    setWidgetXY(ui->LabelSize,                  -1,                                     Offset.height() + 180);
    setWidgetXY(ui->LabelStaticScaleSize,       -1,                                     Offset.height() + 210);
    setWidgetXY(ui->LabelScaleSize,             -1,                                     Offset.height() + 210);
    setWidgetXY(ui->LabelFilesFound,            -1,                                     Offset.height() + 247);
    setWidgetXY(ui->LabelNote,                  -1,                                     Offset.height() + 245);

    // Reposition Button
    setWidgetXY(ui->pushButtonAdd,              -1,                                     Offset.height() + 245);
    setWidgetXY(ui->pushButtonClose,            -1,                                     Offset.height() + 245);

    // Reposition ComboBox and label
    setWidgetXY(ui->LabelStaticConvertTo,       Offset.width() + 360,                   Offset.height() + 175);
    setWidgetXY(ui->comboBoxPixelFormat,        Offset.width() + 460,                   Offset.height() + 173);
    setWidgetXY(ui->LabelStaticResize,          Offset.width() + 360,                   Offset.height() + 220);
    setWidgetXY(ui->comboBoxResize,             Offset.width() + 460,                   Offset.height() + 218);

    // Resize Table
    setWidgetSize(ui->TableFilesFound,          -1,                                     Offset.height() + 130);

    // Resize ScrollBar
    setWidgetSize(ui->horizontalScrollBar,      ViewRect.width(),                       -1);
    setWidgetSize(ui->verticalScrollBar,        -1,                                     ViewRect.height());

    // Reposition VerticalScrollBar
    setWidgetXY(ui->horizontalScrollBar,        ViewRect.x(),                           ViewRect.y() + ViewRect.height() + 5);
    setWidgetXY(ui->verticalScrollBar,          ViewRect.x() + ViewRect.width() + 5,    ViewRect.y());

    // Reset ScrollBar cursor
    ClearScrollBarValue();

    this->move(SelectScreenAndCenter(this->frameGeometry().size()));
}

// ************************************************************************************************

void AddingImage::ClearScrollBarValue()
{
    ui->horizontalScrollBar->blockSignals(true);
    ui->horizontalScrollBar->setValue(0);
    ui->horizontalScrollBar->blockSignals(false);
    ui->verticalScrollBar->blockSignals(true);
    ui->verticalScrollBar->setValue(0);
    ui->verticalScrollBar->blockSignals(false);
}

