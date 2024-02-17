#ifndef FLUNDOCOMMAND_H
#define FLUNDOCOMMAND_H

#include <QUndoCommand>

class FLUndoCommand : public QUndoCommand
{
public:
	FLUndoCommand( QUndoCommand *parent);
	~FLUndoCommand();

private:
	
};


class FLAddFlObjectCommand : public FLUndoCommand
{
public:
	FLAddFlObjectCommand( QUndoCommand *parent);
	~FLAddFlObjectCommand();

private:

};


class FLDeleteFlObjectCommand : public FLUndoCommand
{
public:
	FLDeleteFlObjectCommand( QUndoCommand *parent);
	~FLDeleteFlObjectCommand();

private:

};


class FLAChangeFlObjectCommand : public FLUndoCommand
{
public:
	FLAChangeFlObjectCommand( QUndoCommand *parent);
	~FLAChangeFlObjectCommand();

private:

};
#endif // FLUNDOCOMMAND_H
