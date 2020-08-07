/*
 * (c) Anna Chertova 2020
 * Commands for undo/redo functionality
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

#include "dataitem.h"

class AddCommand : public QUndoCommand
{
public:
    AddCommand(DataItem *item, QUndoCommand *parent = nullptr);
    ~AddCommand();

    void undo() override;
    void redo() override;
};

class DeleteCommand : public QUndoCommand
{
public:
    explicit DeleteCommand(QUndoCommand *parent = nullptr);

    void undo() override;
    void redo() override;
};

#endif // COMMANDS_H
