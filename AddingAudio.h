#ifndef __ADDING_AUDIO_H__
#define __ADDING_AUDIO_H__

#include <QDialog>
#include <QtWidgets>
#include "typedef.h"

namespace Ui {
class AddingAudio;
}

class AddingAudio : public QDialog
{
        Q_OBJECT

    public:
        explicit AddingAudio(eCaller Caller, QString Path, QSize Size, QWidget *parent = 0);
        ~AddingAudio();

    signals:

        void AddAudio(sLoadingInfo LoadingInfo);                // Signal for Audio file
        void CloseAddAudio();                                   // signal to Main app to close this form

    private slots:

        void on_ButtonBrowse_clicked();
        void on_pushButtonAdd_clicked();
        void on_pushButtonClose_clicked();
        void on_ComboBoxDirectory_currentIndexChanged(int index);
        void on_TableFilesFound_cellClicked(int row, int column);
        void on_TableFilesFound_cellActivated(int row, int column);
        void on_TableFilesFound_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

   private:

        void Find();
        void AudioSelected();
        void ResetLoadGUI();
        void ReloadingAudio();
        void LoadingAudio(int Row, eResizer Resizer);


        Ui::AddingAudio*        ui;
        QString                 m_Filename;
        QString                 m_PathAndFilename;
        QDir                    m_currentDir;
        int                     m_FileFound;
        size_t                  m_TotalCount;
        eCaller                 m_Caller;

};

#endif // ADDING_AUDIO_H
