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
