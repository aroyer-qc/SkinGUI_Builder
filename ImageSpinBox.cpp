#include "ImageSpinBox.h"
#include <QRegExp>

ImageSpinBox::ImageSpinBox(QWidget*  parent) : QSpinBox(parent)
{
}

int ImageSpinBox::valueFromText(const QString &text) const
{
    QRegExp Regex("[^A-Z]+");

    Regex.setMinimal(false);
    Regex.setPatternSyntax(QRegExp::RegExp);

    if(Regex.indexIn(text, 0) >= 0)
    {
        return Regex.cap(0).toInt();
    }
    return 0;
}

QString ImageSpinBox::textFromValue(int value) const
{
    return QString("%1").arg(value, 4, 10, QChar('0'));
}
