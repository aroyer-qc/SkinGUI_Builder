//    SpinBoxDelegate.cpp
//
//    A delegate that allows the user to change integer values from the model
//    using a spin box widget.


#include "SpinBoxDelegate.h"
#include "ImageSpinBox.h"

// ************************************************************************************************

SpinBoxDelegate::SpinBoxDelegate(QVector<uint8_t>* Data, QObject *parent) : QStyledItemDelegate(parent)
{
    m_pData = Data;
    m_pID = new ID_Code(TYPE_IMAGE, 0);
}

// ************************************************************************************************

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
{
    QString Code;
    ImageSpinBox *editor = new ImageSpinBox(parent);

    Code = index.model()->data(index, Qt::EditRole).toString();
    m_pID->setCodeText(Code);
    clearStateID_Code(m_pData, m_pID->getCode());
    editor->setPrefix(m_pID->getPrefix());
    editor->setValue(m_pID->getNumber());
    editor->setFrame(false);
    editor->setWrapping(true);
    editor->setMinimum(0);
    editor->setMaximum(9999);
    connect(editor, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    connect(this, SIGNAL(setValue(int)), editor, SLOT(setValue(int)));
    return editor;
}

// ************************************************************************************************

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ImageSpinBox *spinBox = static_cast<ImageSpinBox*>(editor);

    disconnect(editor, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    disconnect(this, SIGNAL(setValue(int)), editor, SLOT(setValue(int)));
    spinBox->interpretText();
    int value = spinBox->value();
    m_pID->setNumber(value);
    setStateID_Code(m_pData, m_pID->getCode());
    model->setData(index, QString("%1%2").arg(m_pID->getPrefix()).arg(value, 4, 10, QChar('0')), Qt::EditRole);

}

// ************************************************************************************************

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

// ************************************************************************************************

void SpinBoxDelegate::valueChanged(int Value)
{
    int OldValue = m_pID->getNumber();
    m_pID->setNumber(Value);

    if((Value > OldValue) ||
       ((Value == 0) && (OldValue == (SIZE_OF_ID_CODE_SPACE - 1))))
    {
        Value = getNextFreeNumber_Up(m_pData, m_pID->getCode());
    }
    else
    {
        Value = getNextFreeNumber_Down(m_pData, m_pID->getCode());
    }

    emit setValue(Value);
}

// ************************************************************************************************

