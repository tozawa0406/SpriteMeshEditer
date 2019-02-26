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
	if (!beforeData_->spriteRenderer || !receiver_.spriteRenderer) { return; }

	prevPivot_ = beforeData_->spriteRenderer->GetPivot();
	nextPivot_ = receiver_.spriteRenderer->GetPivot();

	beforeData_->spriteRenderer->SetPivot(nextPivot_);
}

void PivotCommand::Undo(void)
{
	if (receiver_.spriteRenderer) 
	{
		receiver_.spriteRenderer->SetPivot(prevPivot_);
		if (beforeData_->spriteRenderer)
		{
			beforeData_->spriteRenderer->SetPivot(prevPivot_);
		}
	}
}

void PivotCommand::Redo(void)
{
	if (receiver_.spriteRenderer) 
	{
		receiver_.spriteRenderer->SetPivot(nextPivot_); 
		if (beforeData_->spriteRenderer)
		{
			beforeData_->spriteRenderer->SetPivot(nextPivot_);
		}
	}
}
