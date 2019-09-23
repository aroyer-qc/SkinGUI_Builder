#include <QDir>
#include "AddingAudio.h"
#include "ui_AddingAudio.h"
#include "mainwindow.h"
#include "Utility.h"

// ************************************************************************************************

AddingAudio::AddingAudio(eCaller Caller, QString Path, QSize Size, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AddingAudio)
{
    this->setModal(true);
    ui->setupUi(this);
   // setStyleSheet("background-image: url(:/Images/Marble.jpg);");
    setSizeGripEnabled(false);
    m_currentDir.setPath(Path);
    m_Caller          = Caller;
//    m_Size            = Size;
//    m_pImage          = NULL;
//    m_pProcessedImage = NULL;

    ui->ComboBoxDirectory->blockSignals(true);
    ui->ComboBoxDirectory->setCurrentText(Path);
    UpdateComboBox(ui->ComboBoxDirectory);
    ui->ComboBoxDirectory->blockSignals(false);

    ui->comboBoxAudioMode->blockSignals(true);
    ui->comboBoxAudioMode->setCurrentIndex(FORMAT_RGB565);
    ui->comboBoxAudioMode->blockSignals(false);

    ui->comboBoxSamplingRate->setCurrentIndex(SCALE_FIT);

    //ui->LabelNote->setVisible(false);

    ResetLoadGUI();

    Find();
}

// ************************************************************************************************

AddingAudio::~AddingAudio()
{
//    if(m_pImage          != NULL)       delete m_pImage;
//    if(m_pProcessedImage != NULL)       delete m_pProcessedImage;

    delete ui;
}

// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************
void AddingAudio::on_pushButtonAdd_clicked()
{
    AudioSelected();
}

// ************************************************************************************************

void AddingAudio::on_pushButtonClose_clicked()
{
    CloseAddAudio();
}

// ************************************************************************************************

void AddingAudio::on_ButtonBrowse_clicked()
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

void AddingAudio::on_ComboBoxDirectory_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    m_currentDir = QDir(ui->ComboBoxDirectory->currentText());
    SavePathToXML(m_currentDir.absolutePath());
    Find();
}

// ************************************************************************************************

void AddingAudio::on_TableFilesFound_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    if(currentRow >= 0)
    {
        LoadingAudio(currentRow, AUTO_FORMAT);
    }
}

// ************************************************************************************************

void AddingAudio::on_TableFilesFound_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    LoadingAudio(row, AUTO_FORMAT);
}

// ************************************************************************************************

void AddingAudio::on_TableFilesFound_cellActivated(int row, int column)
{
    // Double click on a line will automatically select this file
    Q_UNUSED(row);
    Q_UNUSED(column);
    AudioSelected();
}

// ************************************************************************************************
// *
// *  Function
// *
// ************************************************************************************************

void AddingAudio::Find()
{
    QStringList files;
    QStringList filters;

    ResetLoadGUI();

    filters << "*.mp3" << "*.wav" << "*.flac";
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

void AddingAudio::AudioSelected()
{
    sLoadingInfo LoadingInfo;

    LoadingInfo.DataSize        = m_TotalCount;                          // Size of the raw data
    //LoadingInfo.Size            = m_pProcessedImage->size();             // Dimension of the image (width & height)
    //LoadingInfo.PixelFormat     = (ui->comboBoxPixelFormat->currentIndex() == FORMAT_RGB565) ? QImage::Format_RGB16 : QImage::Format_ARGB32;
    LoadingInfo.Filename        = m_Filename;
    LoadingInfo.PathAndFilename = m_PathAndFilename;
    LoadingInfo.ScaleType       = (eScaler)ui->comboBoxSamplingRate->currentIndex();

    emit AddAudio(LoadingInfo);
}

// ************************************************************************************************

void AddingAudio::ResetLoadGUI()
{
    ui->pushButtonAdd->setDisabled(true);
    ui->LabelFilename->setText("");
    ui->LabelDataSize->setText("");
    ui->LabelSamplingRate->setText("");
    ui->LabelDuration->setText("");
    ui->LabelSize->setText("");
    //ui->ClearTable(ui->TableFilesFound);
}

// ************************************************************************************************

void AddingAudio::ReloadingAudio()
{
    int row = ui->TableFilesFound->currentRow();
    if(row >= 0)
    {
    //    LoadingImage(row, RELOAD_FORMAT);
    }
}

// ************************************************************************************************

void AddingAudio::LoadingAudio(int row, eResizer Resizer)
{
    QTableWidgetItem *item;

    ui->pushButtonAdd->setEnabled(true);
    item = ui->TableFilesFound->item(row, 0);
    m_Filename        = item->text();
    m_PathAndFilename = m_currentDir.absoluteFilePath(m_Filename);

  //  if(m_pImage != NULL)
  //  {
  //      delete m_pImage;
  //      m_pImage = NULL;
  //  }

  //  m_pImage = new QImage();
   // m_pImage->load(m_PathAndFilename);     // loasd ausio instead

    // Information that do not change if setting change
    ui->LabelFilename->setText(item->text());
 //   ui->LabelSize->setText(PrintSize(m_pAudio->size()));

    if(Resizer == AUTO_FORMAT)
    {
    //    m_PixelFormat = AutoSelectConversion(ui->comboBoxPixelFormat, m_pImage->format());
    }

    //pResizedImage = new QImage();

    //if(m_pProcessedImage != NULL)
   // {
    //    delete m_pProcessedImage;
     //   m_pProcessedImage = NULL;
   // }
   // m_pProcessedImage = new QImage();
   // *m_pProcessedImage = pResizedImage->convertToFormat(m_PixelFormat);
   // delete pResizedImage;

    //m_TotalCount  = m_pProcessedImage->byteCount() / (m_pProcessedImage->width() * m_pProcessedImage->height());    // Adjust byte count to image size in viewport if it is the case
    //m_TotalCount *= (Size.width() * Size.height());

//    ui->LabelPixelFormat->setText(GetFormat(m_pImage->format()));
//    ui->LabelDataSize->setText(QString("%1 Bytes").arg(m_TotalCount));                                              // Print updated value in information
  //  ui->LabelScaleSize->setText(PrintSize(Size));                                                                   // Display viewport size
    }

// ************************************************************************************************

