#include "ScaleCommand.h"

ScaleCommand::ScaleCommand(void) : 
	prevScale_(0)
	, nextScale_(0)
{
}

ScaleCommand::~ScaleCommand(void)
{
}

void ScaleCommand::Invoke(void)
{
	if (!beforeData_->transform || !receiver_.transform) { return; }

	prevScale_ = beforeData_->transform->scale;
	nextScale_ = receiver_.transform->scale;

	if (beforeData_->transform) { beforeData_->transform->scale = nextScale_; }
}

void ScaleCommand::Undo(void)
{
	if (receiver_.transform) 
	{
		receiver_.transform->scale = prevScale_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->scale = prevScale_;
		}
	}
}

void ScaleCommand::Redo(void)
{
	if (receiver_.transform)
	{
		receiver_.transform->scale = nextScale_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->scale = nextScale_;
		}
	}
}
