#include "PivotCommand.h"

PivotCommand::PivotCommand(void) : 
	prevPivot_(0)
	, nextPivot_(0)
{
}

PivotCommand::~PivotCommand(void)
{
}

void PivotCommand::Invoke(void)
{
}

void PivotCommand::Undo(void)
{
	spriteRenderer_->pivot = prevPivot_;
}

void PivotCommand::Redo(void)
{
	spriteRenderer_->pivot = nextPivot_;
}

void PivotCommand::SetPivot(const VECTOR2& prevPivot, const VECTOR2& nextPivot)
{
	prevPivot_ = prevPivot;
	nextPivot_ = nextPivot;
}
