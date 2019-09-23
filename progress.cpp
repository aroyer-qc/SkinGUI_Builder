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
