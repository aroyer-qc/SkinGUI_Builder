#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include <QWidget>
#include <QTimer>

namespace Ui {
class Progress;
}

class Progress : public QWidget
{
        Q_OBJECT

    public:
        explicit Progress(QString Header, QString Stage1, QString Stage2, QString Status, QWidget *parent = 0);
        ~Progress();

    public slots:

        void on_UpdateProgress(QString Status, int Value);

    private:
        Ui::Progress*   ui;
        QString         m_Status;
};

#endif // PROGRESS_H
