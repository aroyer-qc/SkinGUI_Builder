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

#ifndef __COMBOBOXDELEGATE_H__
#define __COMBOBOXDELEGATE_H__

#include <QString>
#include <QComboBox>
#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;

class ComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT

    public:
        ComboBoxDelegate(QObject *parent = 0);

        QWidget*    createEditor            (QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        void        setEditorData           (QWidget *editor, const QModelIndex &index) const;
        void        setModelData            (QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        void        updateEditorGeometry    (QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        void        setEditorValue          (int value);
        void        addItems                (QString item);
        void        clear                   ();

    private:

        int                 m_Value;
        QStringList         m_Items;
};

#endif
