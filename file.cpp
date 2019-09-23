#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SkinSave.h"


// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************

void MainWindow::on_actionNew_Skin_triggered()
{
    SaveSkinAndClearData();
}

// ************************************************************************************************

void MainWindow::on_actionOpen_Skin_triggered()
{
    if(SaveSkinAndClearData() == false)
    {
        QString File = QFileDialog::getOpenFileName(this,
                                                   tr("Open file"),
                                                   m_SkinDir.absolutePath(),
                                                   tr("Skin file (*.skn)"));

        if(!File.isEmpty())
        {
            Open(File);
        }
    }
    else
    {
        m_IsNeedCompleteFileOpen = true;            // Will trigger file open at the end saving
    }
}

// ************************************************************************************************

void MainWindow::on_actionSave_Skin_triggered()
{
    if(m_SkinSize != 0)
    {
        if(m_IsSkinHasUnsavedData == true)
        {
            if(m_IsSkinSaveAs == true)
            {
                on_actionSave_Skin_As_triggered();
            }
            else
            {
                Save();
            }
        }
        else if(m_IsWarningDisplayed == true)
        {
            QMessageBox msgBox;
            msgBox.setText("<font size=\"5\">" + tr("File already up to date</font>"));
            msgBox.setIconPixmap(QPixmap(":/Images/Warning64.png"));
            msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
            msgBox.exec();
        }
    }
    else if(m_IsWarningDisplayed == true)
    {
        QMessageBox msgBox;
        msgBox.setText("<font size=\"5\">" + tr("Nothing to save</font>"));
        msgBox.setIconPixmap(QPixmap(":/Images/Warning64.png"));
        msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
        msgBox.exec();
    }
}

// ************************************************************************************************

void MainWindow::on_actionSave_Skin_As_triggered()
{
    QString File = QFileDialog::getSaveFileName(this,
                                               tr("Save file as"),
                                               m_SkinDir.absolutePath() + "/untitled.skn",
                                               tr("Skin file (*.skn)"));

    if(!File.isEmpty())
    {
        m_SkinName     = File;
        m_IsSkinSaveAs = false;
        m_IsSkinHasUnsavedData = true;             // To trigger data to save
        on_actionSave_Skin_triggered();
        m_IsSkinHasUnsavedData = false;
    }
}

// ************************************************************************************************
// *
// *  Function
// *
// ************************************************************************************************

bool MainWindow::SaveSkinAndClearData()
{
    bool FileSaveInProgress = false;

    if(m_IsSkinHasUnsavedData == true)
    {
        QMessageBox msgBox;
        msgBox.setText("<font size=\"5\">" + tr("Current skin not saved<br>Do you want to save?</font>"));
        msgBox.setIconPixmap(QPixmap(":/Images/question_mark64.png"));
        msgBox.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        if(msgBox.exec() == QMessageBox::Yes)
        {
            m_IsWarningDisplayed = false;
            on_actionSave_Skin_triggered();
            m_IsWarningDisplayed = true;
            FileSaveInProgress = true;
            m_IsNeedToClearDataAfterSave = true;
        }
        else
        {
            ClearAllData();
            ResetAllSkinTab();
        }
    }
    else
    {
        ClearAllData();
        ResetAllSkinTab();
    }

    return FileSaveInProgress;
}

// ************************************************************************************************

void MainWindow::setSkinHasUnsavedData(bool State)
{
    m_IsSkinHasUnsavedData = State;
    if(State)   this->setWindowTitle("* Skin Builder");
    else        this->setWindowTitle("Skin Builder");
}
