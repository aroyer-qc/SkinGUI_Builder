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
