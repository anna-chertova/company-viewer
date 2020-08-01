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

bool DataItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > static_cast<int>(childItems.size()))
        return false;

    for (int row = 0; row < count; ++row) {
        std::vector<QVariant> v(columns);
        DataItem *item = new DataItem(v, this);
        childItems.insert(childItems.begin() + position, item);
    }

    return true;
}

DataItem *DataItem::parent()
{
    return parentItem;
}

bool DataItem::removeChildren(int position, int count)
{
    if (position < 0 ||
            (position + count) > static_cast<int>(childItems.size()))
        return false;

    for (int offset = 0; offset < count; ++offset) {
        delete childItems.at(position + offset);
    }
    childItems.erase(
                childItems.begin() + position,
                childItems.begin() + position + count);
    return true;
}

int DataItem::childNumber() const
{
    if (parentItem) {
        auto childOffset = std::find(parentItem->childItems.begin(),
                                     parentItem->childItems.end(),
                                     this);
        return std::distance(parentItem->childItems.begin(), childOffset);
    }
    return 0;
}

bool DataItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= static_cast<int>(valueItems.size()))
        return false;

    valueItems[column] = value;
    return true;
}
