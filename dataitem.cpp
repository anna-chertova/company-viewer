/*
 * (c) Anna Chertova 2020
 * Data item for hierarchical company data model (implementation)
 */

#include "dataitem.h"

DataItem::DataItem(const std::vector<QVariant>& v, DataItem* p):
    parentItem(p), valueItems(v)
{

}

DataItem::~DataItem()
{
    qDeleteAll(childItems);
    childItems.clear();
}

DataItem *DataItem::child(int number)
{
    Q_ASSERT(number >= 0);
    Q_ASSERT(number < static_cast<int>(childItems.size()));
    return childItems.at(number);
}

int DataItem::childCount() const
{
    return childItems.size();
}

int DataItem::columnCount() const
{
    return valueItems.size();
}

QVariant DataItem::data(int column) const
{
    Q_ASSERT(column >= 0);
    Q_ASSERT(column < static_cast<int>(valueItems.size()));
    return valueItems.at(column);
}

void DataItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > static_cast<int>(childItems.size()))
        return;

    for (int row = 0; row < count; ++row) {
        std::vector<QVariant> v(columns);
        DataItem *item = new DataItem(v, this);
        childItems.insert(childItems.begin() + position, item);
    }
}

DataItem *DataItem::parent()
{
    return parentItem;
}

void DataItem::removeChildren(int position, int count)
{
    if (position < 0 ||
            (position + count) > static_cast<int>(childItems.size()))
        return;

    for (int offset = 0; offset < count; ++offset) {
        delete childItems.at(position + offset);
    }
    childItems.erase(
                childItems.begin() + position,
                childItems.begin() + position + count);
}

void DataItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= static_cast<int>(valueItems.size()))
        return;

    valueItems[column] = value;    
}
