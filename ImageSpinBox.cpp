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
