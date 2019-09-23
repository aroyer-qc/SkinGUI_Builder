#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Utility.h"
#include <QFont>
#include <QPixmap>


// ************************************************************************************************
// *
// *  Slot
// *
// ************************************************************************************************

void MainWindow::on_ButtonAddFont_clicked()
{
    QFont Font;
    int row;
    bool ok;

    Font = QFontDialog::getFont(&ok, QFont("Arial", 12), this);
    if(ok)
    {
        setSkinHasUnsavedData(true);
        m_Font.append(Font);
        m_SamplingFont.append(SAMPLING_DEFAULT_FONT);
        row = m_Font.size() - 1;

        ui->TableFont->insertRow(row);
        QTableWidgetItem *FontNameItem       = new QTableWidgetItem(Font.family());
        QTableWidgetItem *FontSizeItem       = new QTableWidgetItem(QString("%1").arg(Font.pointSize()));
        FontNameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        FontSizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        ui->TableFont->setItem(row, 0, FontNameItem);
        ui->TableFont->setItem(row, 1, FontSizeItem);
        SetTableProperties(row);
        ui->TableFont->setCurrentCell(row, 0);

        LoadFont(row);
    }
}

// ************************************************************************************************

void MainWindow::on_ButtonRemoveFont_clicked()
{
    int row;

    ui->TableFont->blockSignals(true);

    // Get the selected row
    row = ui->TableFont->currentIndex().row();

    // Remove Font from vector
    m_Font.remove(row);
    m_SamplingFont.remove(row);

    // Remove Row
    ui->TableFont->removeRow(row);

    // If skin size is 0, and no more font there is nothing to save
    setSkinHasUnsavedData(((m_SkinSize != 0) || (m_Font.size() != 0)) ? true : false);

    //row--;

    // Update Image GUI for all widget
    if(row >= ui->TableFont->rowCount())
    {
        row =  ui->TableFont->rowCount() - 1;
        ui->TableFont->selectRow(row);
    }

    UpdateFontGUI(row);
    LoadFont(row);

    ui->TableFont->blockSignals(false);
}

// ************************************************************************************************

void MainWindow::on_ButtonUpFont_clicked()
{
    QFont   FontCopy;
    uint8_t SamplingCopy;
    int row;

    ui->TableFont->blockSignals(true);

    // Get the selected row
    row = ui->TableFont->currentIndex().row();

    // Swap info structure
    FontCopy                    = m_Font[row - 1];
    SamplingCopy                = m_SamplingFont[row - 1];

    m_Font[row - 1]             = m_Font[row];
    m_SamplingFont[row - 1]     = m_SamplingFont[row];

    m_Font[row]                 = FontCopy;
    m_SamplingFont[row]         = SamplingCopy;

    // Swap element in table
    InsertNewRowInTableFont(row - 1,
                        ui->TableFont->item(row, 0)->text(),
                        ui->TableFont->item(row, 1)->text(),
                        ui->TableFont->item(row, 2)->text());

    ui->TableFont->selectRow(row - 1);
    ui->TableFont->removeRow(row + 1);

    ui->TableFont->blockSignals(false);

    UpdateFontGUI(row - 1);
}

// ************************************************************************************************

void MainWindow::on_ButtonDownFont_clicked()
{
    QFont   FontCopy;
    uint8_t SamplingCopy;
    int row;

    ui->TableFont->blockSignals(true);

    // Get the selected row
    row = ui->TableFont->currentIndex().row();

    // Swap info structure
    FontCopy                    = m_Font[row + 1];
    SamplingCopy                = m_SamplingFont[row + 1];

    m_Font[row + 1]             = m_Font[row];
    m_SamplingFont[row + 1]     = m_SamplingFont[row];

    m_Font[row]                 = FontCopy;
    m_SamplingFont[row]         = SamplingCopy;

    // Swap element in table
    InsertNewRowInTableFont(row + 2,
                        ui->TableFont->item(row, 0)->text(),
                        ui->TableFont->item(row, 1)->text(),
                        ui->TableFont->item(row, 2)->text());

    ui->TableFont->removeRow(row);
    ui->TableFont->selectRow(row + 1);

    ui->TableFont->blockSignals(false);

    UpdateFontGUI(row + 1);
}

// ************************************************************************************************

void MainWindow::on_TableFont_cellActivated(int row, int column)
{
    Q_UNUSED(column);
    LoadFont(row);
}

// ************************************************************************************************

void MainWindow::on_TableFont_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn);
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    LoadFont(currentRow);
}

// ************************************************************************************************

void MainWindow::on_TableFont_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    LoadFont(row);
}

// ************************************************************************************************

void MainWindow::on_TableFont_cellDoubleClicked(int row, int column)
{
    QString Item;
    int     Value;

    Item  = ui->TableFont->item(row, column)->text();
    if(column == 1)
    {
        Value = getPointSizeFontFromText(Item);
        m_pFontComboBoxDelegate->setEditorValue(Value);
    }
}

// ************************************************************************************************

void MainWindow::on_TableFont_itemChanged(QTableWidgetItem *item)
{
    QString CellData;
    int     Row;
    int     Column;

    Row    = item->row();
    Column = item->column();

    CellData = ui->TableFont->item(Row, Column)->text();

    if(Column == 1)
    {
        m_Font[Row].setPointSize(CellData.toInt());
        LoadFont(Row);
    }
}

// ************************************************************************************************

// Trig a reload
void MainWindow::on_checkBoxAlphaFont_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true) m_SamplingFont[row] |=  SAMPLING_ALPHA;
    else                m_SamplingFont[row] &= ~SAMPLING_ALPHA;
    LoadFont(row);
}

void MainWindow::on_checkBoxNumericFont_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true)
    {
        m_SamplingFont[row] |=  SAMPLING_NUMERIC;
        ui->checkBoxFixed->setEnabled(true);
    }
    else
    {
        m_SamplingFont[row] &= ~SAMPLING_NUMERIC;
        ui->checkBoxFixed->setChecked(false);
        ui->checkBoxFixed->setEnabled(false);
    }
    LoadFont(row);
}

void MainWindow::on_checkBoxSymbolFont_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true) m_SamplingFont[row] |=  SAMPLING_SYMBOL;
    else                m_SamplingFont[row] &= ~SAMPLING_SYMBOL;
    LoadFont(row);
}

void MainWindow::on_checkBoxExtraSymbolFont_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true) m_SamplingFont[row] |=  SAMPLING_EXTRA_SYMBOL;
    else                m_SamplingFont[row] &= ~SAMPLING_EXTRA_SYMBOL;
    LoadFont(row);
}

void MainWindow::on_checkBoxLatinFont_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true) m_SamplingFont[row] |=  SAMPLING_LATIN;
    else                m_SamplingFont[row] &= ~SAMPLING_LATIN;
    LoadFont(row);
}

// ************************************************************************************************

void MainWindow::on_checkBoxBold_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(row >= 0)
    {
        m_Font[row].setBold(checked);
        LoadFont(row);
    }
}

// ************************************************************************************************

void MainWindow::on_checkBoxItalic_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(row >= 0)
    {
        m_Font[row].setItalic(checked);
        LoadFont(row);
    }
}

// ************************************************************************************************

void MainWindow::on_checkBoxFixed_clicked(bool checked)
{
    int row = ui->TableFont->currentRow();

    if(checked == true) m_SamplingFont[row] |=  SAMPLING_FIXED_NUMERIC;
    else                m_SamplingFont[row] &= ~SAMPLING_FIXED_NUMERIC;

    LoadFont(row);
}

// ************************************************************************************************
// *
// *  Function
// *
// ************************************************************************************************

void MainWindow::InitFont()
{
    ui->TableFont->setColumnWidth(0,140);            // Font Name
    ui->TableFont->setColumnWidth(1,80);             // Font Size
    ui->TableFont->setColumnWidth(2,200);            // Font Style

    ui->TableFont->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);    // Only tab with file is resizable
    ui->TableFont->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->TableFont->horizontalHeader()->setStretchLastSection(true);

    m_pFontComboBoxDelegate->clear();
    m_pFontComboBoxDelegate->addItems("6");
    m_pFontComboBoxDelegate->addItems("7");
    m_pFontComboBoxDelegate->addItems("8");
    m_pFontComboBoxDelegate->addItems("9");
    m_pFontComboBoxDelegate->addItems("10");
    m_pFontComboBoxDelegate->addItems("11");
    m_pFontComboBoxDelegate->addItems("12");
    m_pFontComboBoxDelegate->addItems("14");
    m_pFontComboBoxDelegate->addItems("16");
    m_pFontComboBoxDelegate->addItems("18");
    m_pFontComboBoxDelegate->addItems("20");
    m_pFontComboBoxDelegate->addItems("22");
    m_pFontComboBoxDelegate->addItems("24");
    m_pFontComboBoxDelegate->addItems("26");
    m_pFontComboBoxDelegate->addItems("28");
    m_pFontComboBoxDelegate->addItems("32");
    m_pFontComboBoxDelegate->addItems("36");
    m_pFontComboBoxDelegate->addItems("42");
    m_pFontComboBoxDelegate->addItems("48");
    m_pFontComboBoxDelegate->addItems("56");
    m_pFontComboBoxDelegate->addItems("72");

    ResetFontGUI();
}

// ************************************************************************************************

void MainWindow::ResetFontGUI()
{
    ui->ButtonAddFont->setEnabled(true);
    ui->ButtonRemoveFont->setDisabled(true);
    ui->ButtonUpFont->setDisabled(true);
    ui->ButtonDownFont->setDisabled(true);
    ui->LabelFontName->setText("");
    ui->LabelFontSize->setText("");
    ui->LabelFontProperties ->setText("");
    ui->checkBoxAlphaFont->setChecked(true);
    ui->checkBoxNumericFont->setChecked(true);
    ui->checkBoxSymbolFont->setChecked(true);
    ui->checkBoxFixed->setChecked(true);
    //ui->checkBoxExtraSymbolFont->setChecked(true);
    //ui->checkBoxLatinFont->setChecked(true);
    ClearTable(ui->TableFont);
    ClearSceneFont();
    CheckBoxActivation(false);
}

// ************************************************************************************************

void MainWindow::ClearSceneFont()
{
    m_SceneFont.clear();
    ui->graphicsViewFont->setScene(&m_SceneFont);
    m_SceneExample.clear();
    ui->graphicsViewExample->setScene(&m_SceneExample);
}

// ************************************************************************************************

void MainWindow::LoadFont(int row)
{
    QGraphicsTextItem* pItem;
    int Height = 0;
    int HeightOff;

    Q_UNUSED(row);

    ClearSceneFont();
    UpdateFontGUI(row);

    if(m_Font.size() != 0)
    {
        if(row == -1)
        {
            row = ui->TableFont->currentRow();
        }

        QFontMetrics FontMetric(m_Font[row]);
        HeightOff = FontMetric.height();

        // Added image to the scene
        if(ui->checkBoxAlphaFont->isChecked() == true)
        {
            pItem = new QGraphicsTextItem();
            pItem->setFont(m_Font[row]);
            pItem->setDefaultTextColor(Qt::black);
            pItem->setPos(0,Height);
            pItem->setPlainText("The quick brown fox jumps over the lazy dog");
            m_SceneFont.addItem(pItem);
            Height += HeightOff;
        }

        if(ui->checkBoxNumericFont->isChecked() == true)
        {
            if(ui->checkBoxFixed->isChecked() == true)                  // Print with fixed spacing
            {
                QFontMetrics FontMetric(m_Font[row]);
                uint8_t MaxSizeChar = 0;
                uint8_t thisSizeChar;
                int OffsetChar = 0;

                for(uint8_t c = '0'; c <= '9'; c++)
                {
                    thisSizeChar = FontMetric.width(c);
                    if(thisSizeChar > MaxSizeChar)
                    {
                        MaxSizeChar = thisSizeChar;
                    }
                }
                for(uint8_t c = 0; c <= 9; c++)
                {
                    pItem = new QGraphicsTextItem();
                    pItem->setFont(m_Font[row]);
                    pItem->setDefaultTextColor(Qt::black);
                    pItem->setPos(OffsetChar, Height);
                    pItem->setPlainText(QString("%1").arg(c));
                    OffsetChar += MaxSizeChar;
                    m_SceneFont.addItem(pItem);
                }
            }
            else                                                        // Print in normal spacing
            {
                pItem = new QGraphicsTextItem();
                pItem->setFont(m_Font[row]);
                pItem->setDefaultTextColor(Qt::black);
                pItem->setPos(0,Height);
                pItem->setPlainText("0123456789");
                m_SceneFont.addItem(pItem);
            }
            Height += HeightOff;
        }

        if(ui->checkBoxSymbolFont->isChecked() == true)
        {
            pItem = new QGraphicsTextItem();
            pItem->setFont(m_Font[row]);
            pItem->setDefaultTextColor(Qt::black);
            pItem->setPos(0,Height);
            pItem->setPlainText("!\"#$%'()*+,-./:;<=>?@[\\]^_`{|}");
            m_SceneFont.addItem(pItem);
            Height += HeightOff;
        }

        if(ui->checkBoxExtraSymbolFont->isChecked() == true)
        {
            pItem = new QGraphicsTextItem();
            pItem->setFont(m_Font[row]);
            pItem->setDefaultTextColor(Qt::black);
            pItem->setPos(0,Height);
            pItem->setPlainText("°©®±");
            m_SceneFont.addItem(pItem);
            Height += HeightOff;
        }

        if(ui->checkBoxLatinFont->isChecked() == true)
        {
            pItem = new QGraphicsTextItem();
            pItem->setFont(m_Font[row]);
            pItem->setDefaultTextColor(Qt::black);
            pItem->setPos(0,Height);
            pItem->setPlainText("ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝ\nàáâãäåæèéêëìíîïðñòóôõöøùúûüýÞÿ");
            m_SceneFont.addItem(pItem);
            Height += HeightOff;
        }

        ui->LabelFontName->setText(m_Font[row].family());
        ui->LabelFontSize->setText(QString("%1").arg(m_Font[row].pointSize()));
        QString Properties = "";
        if(m_Font[row].bold()       == false)               Properties.append("Normal");
        else                                                Properties.append("Bold");
        if(m_Font[row].italic()     == true)                Properties.append(", Italic");
        if(m_SamplingFont[row] & SAMPLING_FIXED_NUMERIC)    Properties.append("\nNumeric Fixed Width");
        ui->LabelFontProperties->setText(Properties);

        if(m_Font[row].bold()       == false) ui->checkBoxBold->setChecked(false);
        else                                  ui->checkBoxBold->setChecked(true);
        if(m_Font[row].italic()     == false) ui->checkBoxItalic->setChecked(false);
        else                                  ui->checkBoxItalic->setChecked(true);

        SetTableProperties(row);
        DisplayExample(row);
    }
}


// ************************************************************************************************

void MainWindow::SetTableProperties(int row)
{
    QString Properties = "";
    if(m_Font[row].bold()       == false)               Properties.append("Normal");
    else                                                Properties.append("Bold");
    if(m_Font[row].italic()     == true)                Properties.append(", Italic");
    if(m_SamplingFont[row] & SAMPLING_FIXED_NUMERIC)    Properties.append(", Numeric Fixed Width");
    QTableWidgetItem *FontPropertiesItem = new QTableWidgetItem(Properties);
    FontPropertiesItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->TableFont->setItem(row, 2, FontPropertiesItem);
}

// ************************************************************************************************

void MainWindow::InsertNewRowInTableFont(int row, QString Name, QString Size, QString Properties)
{
    ui->TableFont->insertRow(row);
    QTableWidgetItem *FontNameItem = new QTableWidgetItem(Name);
    QTableWidgetItem *FontSizeItem = new QTableWidgetItem(Size);
    QTableWidgetItem *FontPropertiesItem  = new QTableWidgetItem(Properties);
    FontNameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    FontSizeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
    FontPropertiesItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->TableFont->setItem(row, 0, FontNameItem);
    ui->TableFont->setItem(row, 1, FontSizeItem);
    ui->TableFont->setItem(row, 2, FontPropertiesItem);
}

// ************************************************************************************************

void MainWindow::UpdateFontGUI(int row)
{
    // Update CheckBox

    ui->checkBoxAlphaFont->setChecked       ((m_SamplingFont[row] & SAMPLING_ALPHA)         ? true : false);
    ui->checkBoxNumericFont->setChecked     ((m_SamplingFont[row] & SAMPLING_NUMERIC)       ? true : false);
    ui->checkBoxSymbolFont->setChecked      ((m_SamplingFont[row] & SAMPLING_SYMBOL)        ? true : false);
    ui->checkBoxExtraSymbolFont->setChecked ((m_SamplingFont[row] & SAMPLING_EXTRA_SYMBOL)  ? true : false);
    ui->checkBoxLatinFont->setChecked       ((m_SamplingFont[row] & SAMPLING_LATIN)         ? true : false);

    if(ui->checkBoxNumericFont->isChecked() == true)
    {
        ui->checkBoxFixed->setChecked       ((m_SamplingFont[row] & SAMPLING_FIXED_NUMERIC) ? true : false);
    }
    else
    {
        ui->checkBoxFixed->setChecked(false);
        ui->checkBoxFixed->setEnabled(false);
    }

    // Update Button

    if((ui->TableFont->rowCount() > 0) && (ui->TableFont->currentIndex().row() >= 0))
    {
        // An image is selected then button remove is active
        ui->ButtonRemoveFont->setEnabled(true);

        // Also radio button are activated
        CheckBoxActivation(true);
    }
    else
    {
        // No image selected then button remove is deactivated
        ui->ButtonRemoveFont->setDisabled(true);

        // Also radio button are activated
        CheckBoxActivation(false);

        // Clear all label
        ui->LabelFontName->clear();
        ui->LabelFontSize->clear();
        ui->LabelFontProperties->clear();

        // Clear viewport
    }

    if(row > 0)
    {
        ui->ButtonUpFont->setEnabled(true);
    }
    else
    {
        ui->ButtonUpFont->setDisabled(true);
    }

    if((row + 1) < ui->TableFont->rowCount())
    {
        ui->ButtonDownFont->setEnabled(true);
    }
    else
    {
        ui->ButtonDownFont->setDisabled(true);
    }

}

// ************************************************************************************************

void MainWindow::CheckBoxActivation(bool state)
{
    ui->checkBoxAlphaFont->setEnabled(state);
    ui->checkBoxNumericFont->setEnabled(state);
    ui->checkBoxSymbolFont->setEnabled(state);
    ui->checkBoxExtraSymbolFont->setEnabled(state);
    ui->checkBoxLatinFont->setEnabled(state);
    ui->checkBoxBold->setEnabled(state);
    ui->checkBoxItalic->setEnabled(state);
    if(ui->checkBoxNumericFont->isChecked()) ui->checkBoxFixed->setEnabled(state);
}

// ************************************************************************************************

void MainWindow::DisplayExample(int row)
{
    // helpful to debug font save...

    QPixmap* pPix = new QPixmap(SAMPLING_BOX_X_SIZE, SAMPLING_BOX_Y_SIZE);
    QPainter* pPainter = new QPainter(pPix);
    QFontMetrics FontMetric(m_Font[row]);
    QImage Image;

    pPainter->fillRect(0, 0, SAMPLING_BOX_X_SIZE, SAMPLING_BOX_Y_SIZE, Qt::black);
    pPainter->setFont(m_Font[row]);
    pPainter->setPen(Qt::white);
    pPainter->drawText(QPoint(20, FontMetric.height()), QString("%1").arg('a'));
    Image = pPix->toImage();
    m_SceneExample.clear();
    m_SceneExample.addPixmap(*pPix);
}

