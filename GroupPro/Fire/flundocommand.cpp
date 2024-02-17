#include "flundocommand.h"

FLUndoCommand::FLUndoCommand( QUndoCommand *parent)
	: QUndoCommand(parent)
{

}

FLUndoCommand::~FLUndoCommand()
{

}

FLAddFlObjectCommand::FLAddFlObjectCommand(QUndoCommand *parent)
	:FLUndoCommand(parent)
{

}

FLAddFlObjectCommand::~FLAddFlObjectCommand()
{

}

FLDeleteFlObjectCommand::FLDeleteFlObjectCommand(QUndoCommand *parent)
	:FLUndoCommand(parent)
{

}

FLDeleteFlObjectCommand::~FLDeleteFlObjectCommand()
{

}

FLAChangeFlObjectCommand::FLAChangeFlObjectCommand(QUndoCommand *parent)
	:FLUndoCommand(parent)
{

}

FLAChangeFlObjectCommand::~FLAChangeFlObjectCommand()
{

}
