#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"

// ************************************************************************************************

// ************************************************************************************************

void MainWindow::CodeLabel(eCaller Caller, int Item, QString Label)
{
    QTableWidgetItem *LabelItem = new QTableWidgetItem(Label);

    if(Caller == CALLER_IMAGE)
    {
        ui->TableImage->setItem(Item, 2, LabelItem);

    }
    else if(Caller == CALLER_BUTTON)
    {
    }
    else if(Caller == CALLER_ICON)
    {
    }
}

