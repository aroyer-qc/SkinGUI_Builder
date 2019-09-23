#ifndef __IMAGE_SPIN_BOX_H__
#define __IMAGE_SPIN_BOX_H__

#include <QSpinBox>

class ImageSpinBox : public QSpinBox
{
    Q_OBJECT

    public:

                    ImageSpinBox    (QWidget *parent = 0);

        int         valueFromText   (const QString &text) const;
        QString     textFromValue   (int value) const;
};

#endif
