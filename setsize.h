#ifndef __SETSIZE_H__
#define __SETSIZE_H__

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class SetSize;
}

class SetSize : public QDialog
{
        Q_OBJECT

    signals:

     void   SetSizeDisplay        (QSize Size);

    public:
        explicit SetSize(QSize Size, QWidget *parent = 0);
        ~SetSize();

    private slots:

        void on_buttonBox_accepted();
        void on_radioButton_84x48_clicked();
        void on_radioButton_96x64_clicked();
        void on_radioButton_128x128_clicked();
        void on_radioButton_240x320_clicked();
        void on_radioButton_320x200_clicked();
        void on_radioButton_320x240_clicked();
        void on_radioButton_640x480_clicked();
        void on_radioButton_480x272_clicked();
        void on_radioButton_800x480_clicked();
        void on_radioButton_800x600_clicked();
        void on_radioButton_CustomSize_clicked();

        void on_buttonBox_rejected();

    private:

        void ResetAllRadioButton();
        void UpdateLineEdit();
        void CheckButton();

     Ui::SetSize *ui;

        QSize m_Size;
};

#endif // SETSIZE_H
