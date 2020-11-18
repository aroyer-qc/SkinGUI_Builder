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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "AddingImage.h"
#include "Utility.h"


// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************

void MainWindow::on_ButtonAddImage_clicked()
{
    QString Path;

    Path   = m_currentDir.absolutePath();
    m_pLoadImage = new AddingImage(CALLER_IMAGE, Path, m_DisplaySize);
    connect(m_pLoadImage, SIGNAL(AddImage(sLoadingInfo)), this, SLOT(AddImage(sLoadingInfo)));
    connect(m_pLoadImage, SIGNAL(CloseAddImage(void)), this, SLOT(CloseAddImage(void)));
    m_pLoadImage->show();
}

// ************************************************************************************************

void MainWindow::CloseAddImage()
{
    disconnect(m_pLoadImage, SIGNAL(AddImage(sLoadingInfo)), this, SLOT(AddImage(sLoadingInfo)));
    disconnect(m_pLoadImage, SIGNAL(CloseAddImage(void)), this, SLOT(CloseAddImage(void)));
    delete m_pLoadImage;
}

// ************************************************************************************************

void MainWindow::on_ButtonRemoveImage_clicked()
{
    int row;

    // Get the selected row
    row = ui->TableImage->currentIndex().row();

    //Remove Row
    ui->TableImage->removeRow(row);

    // Remove raw data element
    uint32_t Index = m_ImageInfo[row].RawIndex;
    size_t   Size  = m_ImageInfo[row].DataSize;
    m_SkinSize    -= Size;

    // If skin size is 0, there is nothing to save
    setSkinHasUnsavedData((m_SkinSize != 0) ? true : false);

    m_RawImage.remove(Index, Size);

    // Remove element from info structure
    ID_Code ID(m_ImageInfo[row].ID);
    clearStateID_Code(m_pInUseCode, ID.getCode());
    m_ImageInfo.remove(row);

    // Readjust all index in m_ImageInfo
    int Count = m_ImageInfo.count();
    for(int i = 0; i < Count; i++)
    {
        if(m_ImageInfo[i].RawIndex > Index)
        {
            m_ImageInfo[i].RawIndex -= Size;
        }
    }

    row--;

    // Update Image GUI for all widget
    if(row >= ui->TableImage->rowCount())
    {
        ui->TableImage->selectRow(row - 1);
    }

    UpdateImageGUI(row);
}

// ************************************************************************************************

void MainWindow::on_ButtonUpImage_clicked()
{
    sImageInfo ImageInfoCopy;
    int row;

    // Get the selected row
    row = ui->TableImage->currentIndex().row();

    // Swap info structure
    ImageInfoCopy        = m_ImageInfo[row - 1];
    m_ImageInfo[row - 1] = m_ImageInfo[row];
    m_ImageInfo[row]     = ImageInfoCopy;

    // Swap element in table
    InsertNewRowInTableImage(row - 1,
                        ui->TableImage->item(row, 0)->text(),
                        ui->TableImage->item(row, 1)->text(),
                        ui->TableImage->item(row, 2)->text(),
                        ui->TableImage->item(row, 3)->text());

    ui->TableImage->selectRow(row - 1);
    ui->TableImage->removeRow(row + 1);
}

// ************************************************************************************************

void MainWindow::on_ButtonDownImage_clicked()
{
    sImageInfo ImageInfoCopy;
    int row;

    // Get the selected row
    row = ui->TableImage->currentIndex().row();

    // Swap info structure
    ImageInfoCopy        = m_ImageInfo[row + 1];
    m_ImageInfo[row + 1] = m_ImageInfo[row];
    m_ImageInfo[row]     = ImageInfoCopy;

    // Swap element in table
    InsertNewRowInTableImage(row + 2,
                        ui->TableImage->item(row, 0)->text(),
                        ui->TableImage->item(row, 1)->text(),
                        ui->TableImage->item(row, 2)->text(),
                        ui->TableImage->item(row, 3)->text());

    ui->TableImage->removeRow(row);
    ui->TableImage->selectRow(row + 1);
}

// ************************************************************************************************

void MainWindow::on_TableImage_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    UpdateImageGUI(row);
}

// ************************************************************************************************

void MainWindow::on_TableImage_cellDoubleClicked(int row, int column)
{
    QString Item;
    int     Value;
    UpdateImageGUI(row);

    Item  = ui->TableImage->item(row, column)->text();
    if(column == 1)
    {
        Value = getTypeImageFromText(Item);
        m_pImageComboBoxDelegate->setEditorValue(Value);
    }
}

// ************************************************************************************************

void MainWindow::on_TableImage_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    UpdateImageGUI(currentRow);
}

// ************************************************************************************************

void MainWindow::on_TableImage_itemChanged(QTableWidgetItem *item)
{
    QString CellData;
    int     Row;
    int     Column;

    Row    = item->row();
    Column = item->column();

    CellData = ui->TableImage->item(Row, Column)->text();

    if(Column == 1)
    {
        // Get type from ComboBox
        eWidgetType Type = getTypeImageFromText(CellData);
        ID_Code ID(Type, 0);
        int FreeID = getNextFreeNumber_Up(m_pInUseCode, ID.getCode());
        ID.setNumber(FreeID);

        if(m_ImageInfo[Row].ID != ID.getCode())
        {
            // Clear old code
            ID_Code OldID(m_ImageInfo[Row].ID);
            clearStateID_Code(m_pInUseCode, OldID);

            // Set new code
            m_ImageInfo[Row].ID = ID.getCode();
            setStateID_Code(m_pInUseCode, ID);

            QTableWidgetItem *CodeID_Item  = new QTableWidgetItem(ID.getCodeText());
            CodeID_Item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
            ui->TableImage->setItem(Row, 2, CodeID_Item);
            setSkinHasUnsavedData(true);
        }
    }

    if(Column == 2)
    {
        ID_Code ID(CellData);

        if(m_ImageInfo[Row].ID != ID.getCode())
        {
            setStateID_Code(m_pInUseCode, ID.getCode());
            m_ImageInfo[Row].ID = ID.getCode();
            setSkinHasUnsavedData(true);
        }
    }
}

// ************************************************************************************************

void MainWindow::on_ImageCheckerBoardSlider_sliderMoved(int position)
{
   ui->graphicsViewImage->setStyleSheet(QString("background-color: #%1;").arg(position + (position << 8) + (position << 16), 6, 16, QChar('0')));
   CheckerPattern(&m_SceneImage);
}

// ************************************************************************************************

void MainWindow::AddImage(sLoadingInfo LoadingInfo)
{
    QRgb        Pixel;
    uint16_t    ItemCount;
    sImageInfo  ImageInfo;
    QImage      Image;
    QImage      ProcessedImage;

    // reload Path, if it was change by LoadImage class
    m_currentDir.setPath(GetPathFromXML());

    ID_Code ID(TYPE_IMAGE, 0);
    int FreeID = getNextFreeNumber_Up(m_pInUseCode, ID.getCode());
    ID.setNumber(FreeID);

    m_SkinSize += LoadingInfo.DataSize;
    setSkinHasUnsavedData(true);

    // Correct to maximum of display
    LoadingInfo.Size.setWidth( (LoadingInfo.Size.width()  > m_DisplaySize.width())  ? m_DisplaySize.width()  : LoadingInfo.Size.width());
    LoadingInfo.Size.setHeight((LoadingInfo.Size.height() > m_DisplaySize.height()) ? m_DisplaySize.height() : LoadingInfo.Size.height());

    // Create image entry
    ImageInfo.ID           = ID.getCode();
    //ImageInfo.Description  = "-";
    ImageInfo.Filename     = LoadingInfo.Filename;
    ImageInfo.Size         = LoadingInfo.Size;
    ImageInfo.DataSize     = LoadingInfo.DataSize;
    ImageInfo.PixelFormat  = LoadingInfo.PixelFormat;
    ImageInfo.RawIndex     = m_RawImage.size();

    Image.load(LoadingInfo.PathAndFilename);                                                        // Load image
    ScaleToRequirement(&Image, &ProcessedImage, &LoadingInfo.Size, LoadingInfo.ScaleType);          // Scale to user selection
    Image = ProcessedImage;
    ProcessedImage = Image.convertToFormat(LoadingInfo.PixelFormat);                                // change pixel format

    // Add this image entry structure in vector
    m_ImageInfo.append(ImageInfo);

    // Fill Table
    ItemCount = ui->TableImage->rowCount();

    InsertNewRowInTableImage(ItemCount,
                             LoadingInfo.Filename,
                             tr("IM - Image"),
                             ID.getCodeText(),
                             tr("%1 kB").arg(int((LoadingInfo.DataSize + 1023) / 1024)));

    for(int y = 0; y < LoadingInfo.Size.height(); y++)
    {
        for(int x = 0; x < LoadingInfo.Size.width(); x++)
        {
            Pixel = ProcessedImage.pixel(x + LoadingInfo.Offset.x() , y + LoadingInfo.Offset.y());

            if(LoadingInfo.PixelFormat == QImage::Format_RGB16)
            {
                uint16_t Color;

                Color  = (uint16_t)((Pixel >> 3) & 0x3F);
                Color |= (uint16_t)(((Pixel >> 19) & 0x1F) << 11);
                Color |= (uint16_t)(((Pixel >> 10)  & 0x3F)  << 5);
                m_RawImage.append((uint8_t)(Color >> 8));
                m_RawImage.append((uint8_t)Color);
            }
            else
            {
                m_RawImage.append((uint8_t)(Pixel >> 24));
                m_RawImage.append((uint8_t)(Pixel >> 16));
                m_RawImage.append((uint8_t)(Pixel >> 8));
                m_RawImage.append((uint8_t)Pixel);
            }
        }
    }

    UpdateImageGUI(m_ImageInfo.size() - 1);
}


// ************************************************************************************************
// *
// *  Function
// *
// ************************************************************************************************

void MainWindow::InitImage()
{
    ui->TableImage->setColumnWidth(0,390);             // Filename
    ui->TableImage->setColumnWidth(1,120);             // Code type
    ui->TableImage->setColumnWidth(2,120);             // Code ID
    ui->TableImage->setColumnWidth(3,49);              // Data size

    ui->TableImage->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);    // Only tab with file is resizable
    ui->TableImage->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->TableImage->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);

    m_pImageComboBoxDelegate->clear();
    m_pImageComboBoxDelegate->addItems("BK - Background");
    m_pImageComboBoxDelegate->addItems("BT - Button");
    m_pImageComboBoxDelegate->addItems("FT - Font");
    m_pImageComboBoxDelegate->addItems("GL - Glyph");
    m_pImageComboBoxDelegate->addItems("IC - Icon");
    m_pImageComboBoxDelegate->addItems("IM - Image");

    ResetImageGUI();
}

// ************************************************************************************************

void MainWindow::ResetImageGUI()
{
    ui->ButtonUpImage->setDisabled(true);
    ui->ButtonDownImage->setDisabled(true);
    ui->ButtonRemoveImage->setDisabled(true);
    ui->LabelFilenameImage->setText("");
    ui->LabelSizeImage->setText("");
    ui->LabelPixelFormatImage->setText("");
    ui->LabelRawDataSizeImage->setText("");
    ClearTable(ui->TableImage);
    ClearSceneImage();
}

// ************************************************************************************************

void MainWindow::ClearSceneImage()
{
    m_SceneImage.clear();
    ui->graphicsViewImage->setSceneRect(0, 0, m_DisplaySize.width(), m_DisplaySize.height());
    CheckerPattern(&m_SceneImage);
    ui->graphicsViewImage->setScene(&m_SceneImage);
}

// ************************************************************************************************

void MainWindow::UpdateImageGUI(int row)
{
    // Update Button
    if((ui->TableImage->rowCount() > 0) && (ui->TableImage->currentIndex().row() >= 0))
    {
        // An image is selected then button remove is active
        ui->ButtonRemoveImage->setEnabled(true);
    }
    else
    {
        ui->ButtonRemoveImage->setDisabled(true);
    }

    if(row > 0)
    {
        ui->ButtonUpImage->setEnabled(true);
    }
    else
    {
        ui->ButtonUpImage->setDisabled(true);
    }

    if((row + 1) < ui->TableImage->rowCount())
    {
        ui->ButtonDownImage->setEnabled(true);
    }
    else
    {
        ui->ButtonDownImage->setDisabled(true);
    }

    if(ui->TableImage->verticalScrollBar()->isVisible() == true)
    {
        ui->TableImage->setColumnWidth(1,110);  // Resize Define name so size appear correctly in window
        ui->TableImage->setColumnWidth(2,110);
    }
    else
    {
        ui->TableImage->setColumnWidth(1,120);
        ui->TableImage->setColumnWidth(2,120);
    }

    // Update view port
    m_SceneImage.clear();

    if(row >= 0)
    {
        // Update Label
        ui->LabelFilenameImage->setText(m_ImageInfo[row].Filename);
        ui->LabelPixelFormatImage->setText(GetFormat(m_ImageInfo[row].PixelFormat));
        ui->LabelSizeImage->setText(PrintSize(m_ImageInfo[row].Size));
        ui->LabelRawDataSizeImage->setText(QString("%1 Bytes").arg(m_ImageInfo[row].DataSize));


        // Loading raw data in QImage (Patch cannot get a pointer from QVector)
        QImage Image( m_ImageInfo[row].Size, m_ImageInfo[row].PixelFormat);

        int Count = m_ImageInfo[row].RawIndex;

        for(int y = 0; y < m_ImageInfo[row].Size.height(); y++)
        {
            for(int x = 0; x < m_ImageInfo[row].Size.width(); x++)
            {
                QRgb Color;

                if(m_ImageInfo[row].PixelFormat == QImage::Format_RGB16)
                {
                    Color  = ((QRgb)(m_RawImage[Count]   & 0xF8) << 16);
                    Color |= ((QRgb)(m_RawImage[Count++] & 0x07) << 13);
                    Color |= ((QRgb)(m_RawImage[Count]   & 0xE0) << 5);
                    Color |= ((QRgb)(m_RawImage[Count++] & 0x1F) << 3);
                    Color |= 0xFF000000;
                }
                else
                {
                    Color  = m_RawImage[Count++] << 24;
                    Color |= m_RawImage[Count++] << 16;
                    Color |= m_RawImage[Count++] << 8;
                    Color |= m_RawImage[Count++];
                }

                Image.setPixel(x, y, Color);
            }
        }

        QGraphicsPixmapItem *PixmapItem = m_SceneImage.addPixmap(QPixmap::fromImage(Image));                // Add the image on top of the checker pattern
        QPoint Point = CenterPoint(m_ImageInfo[row].Size, m_DisplaySize);                                   // Calculate Center position for the image
        PixmapItem->setPos(Point);                                                                          // Then set it
    }
    UpdateStatusBar();
}

// ************************************************************************************************

void MainWindow::AdjustTabImage(QSize Offset, QRect ViewRect)
{
    // Resize Vertical Line
    setWidgetSize(ui->line_V_Image,              -1,                    Offset.height() + 226);

    // Resize Horizontal Line
    setWidgetSize(ui->line_H_Image,              Offset.width() + 20,   -1);

    // Reposition Horizontal Line
    setWidgetXY(ui->line_H_Image,                -1,                    Offset.height() + 40);

    // Reposition Label
    setWidgetXY(ui->LabelStaticImageInfo,        -1,                    Offset.height() + 60);
    setWidgetXY(ui->LabelStaticFilenameImage,    -1,                    Offset.height() + 90);
    setWidgetXY(ui->LabelFilenameImage,          -1,                    Offset.height() + 120);
    setWidgetXY(ui->LabelStaticSizeImage,        -1,                    Offset.height() + 150);
    setWidgetXY(ui->LabelSizeImage,              -1,                    Offset.height() + 150);
    setWidgetXY(ui->LabelStaticPixelFormatImage, -1,                    Offset.height() + 180);
    setWidgetXY(ui->LabelPixelFormatImage,       -1,                    Offset.height() + 180);
    setWidgetXY(ui->LabelStaticRawDataSizeImage, -1,                    Offset.height() + 210);
    setWidgetXY(ui->LabelRawDataSizeImage,       -1,                    Offset.height() + 210);

    // Reposition Button
    setWidgetXY(ui->ButtonAddImage,              -1,                    Offset.height() + 200);
    setWidgetXY(ui->ButtonRemoveImage,           -1,                    Offset.height() + 200);
    setWidgetXY(ui->ButtonUpImage,               -1,                    Offset.height() + 200);
    setWidgetXY(ui->ButtonDownImage,             -1,                    Offset.height() + 200);

    // Resize Table
    setWidgetSize(ui->TableImage,                -1,                    Offset.height() + 170);

    //Resize GraphicsView
    ui->graphicsViewImage->setGeometry(ViewRect);
    ui->graphicsViewImage->setSceneRect(0, 0, ViewRect.width(), ViewRect.height());

    Find();
}

// ************************************************************************************************

void MainWindow::InsertNewRowInTableImage(int row, QString File, QString CodeType, QString CodeID, QString Size)
{
    ui->TableImage->insertRow(row);
    QTableWidgetItem *fileNameItem = new QTableWidgetItem(File);
    QTableWidgetItem *CodeTypeItem = new QTableWidgetItem(CodeType);
    QTableWidgetItem *CodeID_Item  = new QTableWidgetItem(CodeID);
    QTableWidgetItem *SizeItem     = new QTableWidgetItem(Size);
    fileNameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    CodeTypeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    CodeID_Item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    SizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    ui->TableImage->setItem(row, 0, fileNameItem);
    ui->TableImage->setItem(row, 1, CodeTypeItem);
    ui->TableImage->setItem(row, 2, CodeID_Item);
    ui->TableImage->setItem(row, 3, SizeItem);

    ID_Code ID(CodeID);
    setStateID_Code(m_pInUseCode, ID.getCode());
}

// ************************************************************************************************

