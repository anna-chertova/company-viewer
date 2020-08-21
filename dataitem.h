/*
 * (c) Anna Chertova 2020
 * Data item for hierarchical company data model
 */

#ifndef DATAITEM_H
#define DATAITEM_H

#include <vector>
#include <QVariant>

class DataItem {

public:

    explicit DataItem(const std::vector<QVariant>& v, DataItem* p = nullptr);
    ~DataItem();

    // returns child with the specified number
    DataItem *child(int number);
    // returns number of children of the current item
    int childCount() const;
    // returns number of data columns in the current item
    int columnCount() const;
    // returns data in the specified column
    QVariant data(int column) const;
    // adds children items in the specified position
    // number of children = count,
    // number of data columns added = columns
    void insertChildren(int position, int count, int columns);
    // returns parent of the current item
    DataItem *parent();
    // removes children from the position
    // number of children to be removed = count
    void removeChildren(int position, int count);
    // modifies item's data int specified column with specified value
    void setData(int column, const QVariant &value);

private:

    DataItem *parentItem;
    std::vector<DataItem*> childItems;
    std::vector<QVariant> valueItems;
};

#endif // DATAITEM_H
