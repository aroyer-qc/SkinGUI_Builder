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

#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#include <QStyledItemDelegate>
#include "Utility.h"

class SpinBoxDelegate : public QStyledItemDelegate
{
        Q_OBJECT

    public:

                    SpinBoxDelegate         (QVector<uint8_t>* Data, QObject *parent = 0);

        QWidget*    createEditor            (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void        setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void        updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    public slots:

        void        valueChanged            (int Value);

    signals:
        void        setValue                (int Value);

    private:

        QVector<uint8_t>* m_pData;
        ID_Code*          m_pID;
};

#endif
