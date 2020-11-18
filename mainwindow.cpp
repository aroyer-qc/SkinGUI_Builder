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
#include "setsize.h"
#include "setEndianess.h"
#include "Utility.h"

// ************************************************************************************************

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
   // setStyleSheet("background-image: url(:/Images/Marble.jpg);");
    statusBar()->setSizeGripEnabled(false);                                     // Remove the hand grip

    m_pInUseCode = new QVector<uint8_t>;
    m_pInUseCode->resize(6 * 1250);
    m_pInUseCode->fill(0);

    m_pImageComboBoxDelegate = new ComboBoxDelegate;
    ui->TableImage->setItemDelegateForColumn(1, m_pImageComboBoxDelegate);

    m_pImageSpinBoxDelegate = new SpinBoxDelegate(m_pInUseCode);
    ui->TableImage->setItemDelegateForColumn(2, m_pImageSpinBoxDelegate);

    m_pFontComboBoxDelegate = new ComboBoxDelegate;
    ui->TableFont->setItemDelegateForColumn(1, m_pFontComboBoxDelegate);

    m_pImage          = nullptr;
    m_pProcessedImage = nullptr;
    m_pStatusLabel    = nullptr;
    m_pPixmapItem     = nullptr;

    m_IsNeedCompleteFileOpen = false;
    m_IsNeedToClearDataAfterSave = false;
    m_IsNeedToCloseAfterSave = false;

    m_MainRect = this->geometry();
    m_TabRect  = ui->TabFunctionSelect->geometry();
    m_ViewRect = ui->graphicsViewImage->geometry();                             // Get QRect from forms designer, as base value

    m_SkinDir.setPath(GetSkinPathFromXML());
    m_currentDir.setPath(GetPathFromXML());
    m_DisplaySize = GetDisplaySizeFromXML();
    m_Endian = BIG_ENDIAN;

    m_pStatusLabel = new QLabel();
    ui->statusBar->addWidget(m_pStatusLabel);

    ClearAllData();                                                             // Make sure all data are cleared
    SetSizeDisplay(m_DisplaySize);

    InitImage();
    InitFont();
    InitConverter();
}

// ************************************************************************************************

MainWindow::~MainWindow()
{
    if(m_pImage                 != nullptr)       delete m_pImage;
    if(m_pProcessedImage        != nullptr)       delete m_pProcessedImage;
    if(m_pPixmapItem            != nullptr)       delete m_pPixmapItem;

    delete m_pStatusLabel;
    delete m_pImageComboBoxDelegate;
    delete m_pImageSpinBoxDelegate;
    delete m_pFontComboBoxDelegate;
    delete m_pInUseCode;

    delete ui;
}

// ************************************************************************************************

void MainWindow::ClearAllData()
{
    m_SkinSize = 0;
    m_SkinName = "none";
    m_IsSkinSaveAs = true;
    m_IsSkinHasUnsavedData = false;
    m_IsWarningDisplayed = true;
    m_ImageInfo.clear();    // Remove all image info structure
    m_RawImage.clear();     // Remove all raw image data
    m_Font.clear();         // Remove all font info structure
    m_SamplingFont.clear(); // Remove all Sampling info structure

    //m_RawLabel.clear();     // Remove all Label
    //m_WidgetIndex.clear();  // remove all widget index
    //m_Widget.clear();       // remove all widget

    m_pStatusLabel->setText(QString("       Status :    Skin name : none       Skin size : 0 byte "));
}

// ************************************************************************************************

void MainWindow::ResetAllSkinTab()
{
    InitImage();            // Reset tab Image
    InitFont();

}

// ************************************************************************************************

void MainWindow::on_actionSet_Display_Size_triggered()
{
    SetSize* SetSizeDisplay;

    SetSizeDisplay = new SetSize(m_DisplaySize);
    connect(SetSizeDisplay, SIGNAL(SetSizeDisplay(QSize)), this, SLOT(SetSizeDisplay(QSize)));
    SetSizeDisplay->show();
}

// ************************************************************************************************

void MainWindow::SetSizeDisplay(QSize Size)
{
    QRect   MainRect;
    QRect   TabRect;
    QRect   ViewRect;
    int     AddVertical;
    int     AddHorizontal;

    if(Size != m_DisplaySize)                       // don't do anything if nothing has change
    {
        SaveSkinAndClearData();
        m_DisplaySize = Size;
        SaveDisplaySizeToXML(Size);
    }

    // All dynamic resize will follow setting from original designer value of 'graphicsViewImage'
    MainRect         = m_MainRect;
    TabRect          = m_TabRect;
    ViewRect         = m_ViewRect;

    // X pos of GraphicsView realigned
    if(m_DisplaySize.width() > MINIMUM_PREVIEW_WIDTH)
    {
        AddHorizontal = (m_DisplaySize.width() - MINIMUM_PREVIEW_WIDTH);
        MainRect.setWidth(MainRect.width() + AddHorizontal);
        TabRect.setWidth(TabRect.width()   + AddHorizontal);
        ViewRect.setWidth(ViewRect.width() + AddHorizontal);
    }
    else
    {
        ViewRect.moveLeft(ViewRect.x() + ((MINIMUM_PREVIEW_WIDTH / 2) - (m_DisplaySize.width() / 2)));
        ViewRect.setWidth((ViewRect.width() - MINIMUM_PREVIEW_WIDTH) +  m_DisplaySize.width());
    }

    // Y pos of GraphicsView realigned
    if(m_DisplaySize.height() > MINIMUM_PREVIEW_HEIGHT)
    {
        AddVertical = (m_DisplaySize.height() - MINIMUM_PREVIEW_HEIGHT);
        MainRect.setHeight(MainRect.height() + AddVertical);
        TabRect.setHeight(TabRect.height()   + AddVertical);
        ViewRect.setHeight(ViewRect.height() + AddVertical);
    }
    else
    {
        ViewRect.moveTop(ViewRect.y() + ((MINIMUM_PREVIEW_HEIGHT / 2) - (m_DisplaySize.height() / 2)));
        ViewRect.setHeight((ViewRect.height() - MINIMUM_PREVIEW_HEIGHT) +  m_DisplaySize.height());
    }

    // Set Size of the main window, QTabWidget and QGraphicsView
    this->hide();
    this->setMinimumSize(0, 0);
    this->setMaximumSize(2048, 2048);
    this->setGeometry(MainRect);
    this->setMinimumSize(MainRect.size().width(), MainRect.size().height());
    this->setMaximumSize(MainRect.size().width(), MainRect.size().height());
    ui->TabFunctionSelect->setGeometry(TabRect);

    // Resize tab
    QSize OffsetWidget = GetSizeFromGraphicsView(ui->graphicsViewImage, m_DisplaySize);
    AdjustTabImage(OffsetWidget, ViewRect);
    AdjustTabConverter(OffsetWidget, ViewRect);
    this->move(SelectScreenAndCenter(this->frameGeometry().size()));
    this->show();
}

// ************************************************************************************************

void MainWindow::on_actionSet_Endianess_triggered()
{
    SetEndian* SetEndianDisplay;

    SetEndianDisplay = new SetEndian(m_Endian);
    connect(SetEndianDisplay, SIGNAL(SetEndianess(eEndianess)), this, SLOT(SetEndianess(eEndianess)));
    SetEndianDisplay->show();
}

// ************************************************************************************************

void MainWindow::SetEndianess(eEndianess Endian)
{
    m_Endian = Endian;
}

// ************************************************************************************************

void MainWindow::UpdateStatusBar()
{
    m_pStatusLabel->setText(QString("       Status :    Skin name : %1       Skin size : %2 byte%3")
                            .arg(m_SkinName)
                            .arg(m_SkinSize)
                            .arg(((m_SkinSize) ? 's' : ' ')));
}

// ************************************************************************************************

void MainWindow::on_actionExit_triggered()
{
    // Check if we have data to save before exiting
    // If we do, than exit will be delayed until it is done, then it will close

    m_IsNeedToCloseAfterSave = true;
    if(SaveSkinAndClearData() == false)
    {
        this->close();
    }
}

// ************************************************************************************************

void MainWindow::closeEvent(QCloseEvent *bar)
{
    // Check if we have data to save before exiting
    // If we do, than exit will be delayed until it is done, then it will close

    m_IsNeedToCloseAfterSave = true;
    if(SaveSkinAndClearData() == true)
    {
        bar->ignore();                          // Ignore request to close for now, done after saving
    }
}

// ************************************************************************************************
