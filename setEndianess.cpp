#include "setEndianess.h"
#include "ui_setEndianess.h"

// ************************************************************************************************

SetEndian::SetEndian(eEndianess Endian, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetEndian)
{
    ui->setupUi(this);
    m_Endian = Endian;
    if (m_Endian == LITTLE_ENDIAN) on_radioButton_Little_clicked();
    else                           on_radioButton_Big_clicked();
}

// ************************************************************************************************

SetEndian::~SetEndian()
{
    delete ui;
}

// ************************************************************************************************

void SetEndian::on_buttonBox_accepted()
{
    emit SetEndianess(m_Endian);
    this->close();
    delete(this);
}

// ************************************************************************************************


void SetEndian::on_radioButton_Little_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_Little->setChecked(true);
    m_Endian = LITTLE_ENDIAN;
}

// ************************************************************************************************

void SetEndian::on_radioButton_Big_clicked()
{
    ResetAllRadioButton();
    ui->radioButton_Big->setChecked(true);
    m_Endian = BIG_ENDIAN;
}

// ************************************************************************************************

void SetEndian::ResetAllRadioButton()
{
    ui->radioButton_Little->setChecked(false);
    ui->radioButton_Big->setChecked(false);
}

// ************************************************************************************************

void SetEndian::on_buttonBox_rejected()
{
    this->close();
}

// ************************************************************************************************

