#include "historyview.hpp"

HistoryView::HistoryView(QWidget *parent, const QList<DataItem> &histList, const QString &title)
        : QDialog(parent), historyList(histList)
{
    resize(QDesktopWidget().availableGeometry(this).size() * 0.5);
    setWindowTitle(title);
    setWindowIcon(QIcon(":assets/history.ico"));
    // Table layout management
    table     = new QTableWidget(historyList.size(), 3, this);
    deleteBtn = new QPushButton(QIcon(":assets/delete.ico"), "", this);
    table->setFixedSize(width(), height());
    const auto columnWidth{table->width() / 3};
    table->setColumnWidth(0, columnWidth);
    table->setColumnWidth(1, columnWidth);
    table->setColumnWidth(2, columnWidth);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Date and Time"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Url"));
    int i{0};
    for (auto const &temp : qAsConst(historyList))
    {
        table->setItem(i, 0, new QTableWidgetItem(temp.date, 0));
        table->setItem(i, 1, new QTableWidgetItem(temp.title, 0));
        table->setItem(i, 2, new QTableWidgetItem(temp.page, 0));
        i++;
    }
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    auto lay = new QVBoxLayout(this);
    lay->addWidget(table, 9);
    lay->addWidget(deleteBtn, 1);
    connect(table, &QTableWidget::cellDoubleClicked, this, &HistoryView::onItemClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &HistoryView::onDelete);
}

/// Deletion of a row
void HistoryView::onDelete()
{
    const int pos{table->currentRow()};
    table->removeRow(pos);
    table->setCurrentCell(0, 0);
    if ((pos < historyList.size()) && (!historyList.empty()) && (pos >= 0))
    {
        historyList.takeAt(pos);
        emit historyItemDeleted(pos);
    }
}

/// Click on an item of the view
/// \param x
/// \param y
void HistoryView::onItemClicked(int x, int y)
{
    Q_UNUSED(y)
    emit goToUrl(QUrl(table->item(x, 2)->text()));
}