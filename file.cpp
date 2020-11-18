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
