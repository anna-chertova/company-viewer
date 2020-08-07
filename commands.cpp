/*
 * (c) Anna Chertova 2020
 * Commands for undo/redo functionality
 */

#include "commands.h"

AddCommand::AddCommand(DataItem *item, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    setText("Add item");
}
AddCommand::~AddCommand()
{

}

void AddCommand::undo()
{

}

void AddCommand::redo()
{

}

DeleteCommand::DeleteCommand(QUndoCommand *parent)
    : QUndoCommand(parent)
{
    setText("Delete item");
}

void DeleteCommand::undo()
{

}
void DeleteCommand::redo()
{

}

