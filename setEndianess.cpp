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

