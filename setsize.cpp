#include "setsize.h"
#include "ui_setsize.h"

// ************************************************************************************************

SetSize::SetSize(QSize Size, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetSize)
{
    ui->setupUi(this);

    ui->lineEdit_CustomWidth->setValidator(new QIntValidator(0, 2048, this));
    ui->lineEdit_CustomHeight->setValidator(new QIntValidator(0, 2048, this));

    m_Size = Size;
    CheckButton();
}

// ************************************************************************************************

SetSize::~SetSize()
{
    delete ui;
}

// ************************************************************************************************

void SetSize::on_buttonBox_accepted()
{
    emit SetSizeDisplay(m_Size);
    this->close();
}

// ************************************************************************************************

void SetSize::on_buttonBox_rejected()
{
    this->close();
}

// ************************************************************************************************

void SetSize::on_radioButton_84x48_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_84x48->setChecked(true);
    m_Size = {84, 48};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_96x64_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_96x64->setChecked(true);
    m_Size = {96, 64};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_128x128_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_128x128->setChecked(true);
    m_Size = {128, 128};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_240x320_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_240x320->setChecked(true);
    m_Size = {240, 320};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_320x200_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_320x200->setChecked(true);
    m_Size = {320, 200};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_320x240_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_320x240->setChecked(true);
    m_Size = {320, 240};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_480x272_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_480x272->setChecked(true);
    m_Size = {480, 272};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_640x480_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_640x480->setChecked(true);
    m_Size = {640, 480};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_800x480_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_800x480->setChecked(true);
    m_Size = {800, 480};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_800x600_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_800x600->setChecked(true);
    m_Size = {800, 600};
    UpdateLineEdit();
}

// ************************************************************************************************

void SetSize::on_radioButton_CustomSize_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_CustomSize->setChecked(true);
    ui->lineEdit_CustomWidth->setEnabled(true);
    ui->lineEdit_CustomHeight->setEnabled(true);
}

// ************************************************************************************************

void SetSize::ResetAllRadioButton()
{
    ui->radioButton_84x48->setChecked(false);
    ui->radioButton_96x64->setChecked(false);
    ui->radioButton_128x128->setChecked(false);
    ui->radioButton_240x320->setChecked(false);
    ui->radioButton_320x200->setChecked(false);
    ui->radioButton_320x240->setChecked(false);
    ui->radioButton_480x272->setChecked(false);
    ui->radioButton_640x480->setChecked(false);
    ui->radioButton_800x480->setChecked(false);
    ui->radioButton_800x600->setChecked(false);
    ui->radioButton_CustomSize->setChecked(false);
    ui->lineEdit_CustomWidth->setEnabled(false);
    ui->lineEdit_CustomHeight->setEnabled(false);
}

// ************************************************************************************************

void SetSize::UpdateLineEdit()
{
    ui->lineEdit_CustomWidth->setText(QString("%1").arg(m_Size.width()));
    ui->lineEdit_CustomHeight->setText(QString("%1").arg(m_Size.height()));
}

// ************************************************************************************************

void SetSize::CheckButton()
{
    if     ((m_Size.width() == 84)  && (m_Size.height() == 48))  on_radioButton_84x48_clicked();
    else if((m_Size.width() == 96)  && (m_Size.height() == 64))  on_radioButton_96x64_clicked();
    else if((m_Size.width() == 128) && (m_Size.height() == 128)) on_radioButton_128x128_clicked();
    else if((m_Size.width() == 240) && (m_Size.height() == 320)) on_radioButton_240x320_clicked();
    else if((m_Size.width() == 320) && (m_Size.height() == 200)) on_radioButton_320x200_clicked();
    else if((m_Size.width() == 320) && (m_Size.height() == 240)) on_radioButton_320x240_clicked();
    else if((m_Size.width() == 480) && (m_Size.height() == 272)) on_radioButton_480x272_clicked();
    else if((m_Size.width() == 640) && (m_Size.height() == 480)) on_radioButton_640x480_clicked();
    else if((m_Size.width() == 800) && (m_Size.height() == 480)) on_radioButton_800x480_clicked();
    else if((m_Size.width() == 800) && (m_Size.height() == 600)) on_radioButton_800x600_clicked();
    else                                                         on_radioButton_CustomSize_clicked();
}

// ************************************************************************************************

