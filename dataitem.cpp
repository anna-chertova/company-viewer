#include "dataitem.h"

DataItem::DataItem(const std::vector<QVariant>& v, DataItem* p): parentItem(p), valueItems(v)
{

}

DataItem::~DataItem()
{
    qDeleteAll(childItems);
}

DataItem *DataItem::child(int number)
{
    if (number < 0 || static_cast<size_t>(number) >= childItems.size())
        return nullptr;
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
    if (column < 0 || static_cast<size_t>(column) >= valueItems.size())
        return QVariant();
    return valueItems.at(column);
}

bool DataItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || static_cast<size_t>(position) > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        std::vector<QVariant> v(columns);
        DataItem *item = new DataItem(v, this);
        childItems.insert(childItems.begin() + position, item);
    }

    return true;
}

/*
bool DataItem::insertColumns(int position, int columns)
{
    if (position < 0 || static_cast<size_t>(position) > valueItems.size())
        return false;

    for (int column = 0; column < columns; ++column)
        valueItems.insert(valueItems.begin() + position, QVariant());

    for (DataItem *child : childItems)
        child->insertColumns(position, columns);

    return true;
}*/

DataItem *DataItem::parent()
{
    return parentItem;
}

bool DataItem::removeChildren(int position, int count)
{
    if (position < 0 || static_cast<size_t>(position + count) > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        DataItem *item = childItems.at(position);
        delete item;
        childItems.erase(childItems.begin() + position);
    }
    return true;
}

int DataItem::childNumber() const
{
    if (parentItem) {
        auto childOffset = std::find(parentItem->childItems.begin(), parentItem->childItems.end(), this);
        return std::distance(parentItem->childItems.begin(), childOffset);
    }
    return 0;
}

bool DataItem::setData(int column, const QVariant &value)
{
    if (column < 0 || static_cast<size_t>(column) >= valueItems.size())
        return false;

    valueItems[column] = value;
    return true;
}
