#ifndef __SETENDIANESS_H__
#define __SETENDIANESS_H__

#include <QDialog>
#include <QtWidgets>
#include "typedef.h"

namespace Ui {
class SetEndian;
}

class SetEndian : public QDialog
{
        Q_OBJECT

    signals:

     void   SetEndianess(eEndianess Endian);

    public:
        explicit SetEndian(eEndianess Endian, QWidget *parent = 0);
        ~SetEndian();

    private slots:

        void on_buttonBox_accepted();
        void on_radioButton_Little_clicked();
        void on_radioButton_Big_clicked();

        void on_buttonBox_rejected();

    private:

        void ResetAllRadioButton();

        Ui::SetEndian* ui;
        eEndianess     m_Endian;
};

#endif // __SETENDIANESS_H__
