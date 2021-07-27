#ifndef HISTORYVIEW_HPP
#define HISTORYVIEW_HPP

#include "dataitem.hpp"
#include <QUrl>
#include <QIcon>
#include <QDebug>
#include <QDialog>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QDesktopWidget>

class HistoryView : public QDialog
{
Q_OBJECT

public:
    explicit HistoryView(QWidget *parent = nullptr, const QList<DataItem> &histList = {},
                         const QString &title = "History");

private:
    QTableWidget *table;
    QPushButton *deleteBtn;
    QList<DataItem> historyList{};

private slots:

    void onDelete();

    void onItemClicked(int x, int y);

signals:

    void historyItemDeleted(int);

    void goToUrl(QUrl url);
};

#endif // HISTORYVIEW_HPP
