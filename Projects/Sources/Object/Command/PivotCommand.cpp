#include "PivotCommand.h"

PivotCommand::PivotCommand(void) : 
	prevPivot_(0)
	, nextPivot_(0)
{
}

PivotCommand::~PivotCommand(void)
{
}

void PivotCommand::Invoke(Receiver& beforeData)
{
	if (!beforeData.pivot || !receiver_.pivot) { return; }

	prevPivot_ = *beforeData.pivot;
	nextPivot_ = *receiver_.pivot;;

	*beforeData.pivot = nextPivot_;
}

void PivotCommand::Undo(Receiver& beforeData)
{
	if (receiver_.pivot) 
	{
		*receiver_.pivot = prevPivot_; 
		if (beforeData.pivot)
		{
			*beforeData.pivot = prevPivot_;
		}
	}
}

void PivotCommand::Redo(Receiver& beforeData)
{
	if (receiver_.pivot) 
	{
		*receiver_.pivot = nextPivot_; 
		if (beforeData.pivot)
		{
			*beforeData.pivot = nextPivot_;
		}
	}
}
