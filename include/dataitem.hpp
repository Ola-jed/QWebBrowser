#ifndef DATAITEM_H
#define DATAITEM_H

#include <QString>

struct DataItem
{
    QString date;
    QString title;
    QString page;

    DataItem(const QString &dte, const QString &title, const QString &page)
            : date(dte), title(title), page(page)
    {
    }

    DataItem(const DataItem &toCopy)
            : date(toCopy.date), title(toCopy.title), page(toCopy.page)
    {
    }
};

#endif // DATAITEM_H