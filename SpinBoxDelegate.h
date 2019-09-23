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
