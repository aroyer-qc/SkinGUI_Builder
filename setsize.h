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
