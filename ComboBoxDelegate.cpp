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


#include "ComboBoxDelegate.h"
#include <QComboBox>
#include <QWidget>
#include <QModelIndex>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QComboBox* Editor = new QComboBox(parent);
    Editor->setEditable(false);
    if(Editor->count() == 0)
    {
        Editor->addItems(m_Items);
    }
    Editor->setCurrentIndex(m_Value);

    return Editor;
}

void ComboBoxDelegate::setEditorData(QWidget *Editor, const QModelIndex &index) const
{
   Q_UNUSED(index);
   QComboBox *comboBox = static_cast<QComboBox*>(Editor);
    comboBox->setCurrentIndex(m_Value);
}

void ComboBoxDelegate::setModelData(QWidget *Editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(Editor);
    model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *Editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    Editor->setGeometry(option.rect);
}

void ComboBoxDelegate::setEditorValue(int value)
{
    m_Value = value;
}

void ComboBoxDelegate::addItems(QString item)
{
    m_Items.append(item);
}

void ComboBoxDelegate::clear()
{
    m_Items.clear();
}
