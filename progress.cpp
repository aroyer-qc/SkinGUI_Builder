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

#include "progress.h"
#include "ui_progress.h"

Progress::Progress(QString Header, QString Stage1, QString Stage2, QString Status, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Progress)
{
    //parent->

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    //setModa
    ui->setupUi(this);
    ui->LabelHeader->setText(Header);
    ui->progressBar->setValue(0);
    m_Status = Status;
    ui->LabelStage1->setText(Stage1);
    ui->LabelStage2->setText(Stage2);
    ui->progressBar_2->setValue(0);
    show();
}

Progress::~Progress()
{
    delete ui;
}

// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************

void Progress::on_UpdateProgress(QString Status, int Value)
{
    if(Status.isEmpty())
    {
        Status = m_Status;
    }

    ui->LabelStatus->setText(Status);
    if(Value <= 100) ui->progressBar->setValue(Value);
    if(Value >= 100) ui->progressBar_2->setValue(Value - 100);
}

// ************************************************************************************************
