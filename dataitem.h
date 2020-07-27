#ifndef DATAITEM_H
#define DATAITEM_H

#include <vector>
#include <QVariant>

class DataItem {

public:

    explicit DataItem(const std::vector<QVariant>& v, DataItem* p = nullptr);
    ~DataItem();

    DataItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    DataItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:

    DataItem *parentItem;
    std::vector<DataItem*> childItems;
    std::vector<QVariant> valueItems;
};

#endif // DATAITEM_H
