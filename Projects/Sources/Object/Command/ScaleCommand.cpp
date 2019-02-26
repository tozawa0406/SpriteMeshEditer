#include "ScaleCommand.h"

ScaleCommand::ScaleCommand(void) : 
	prevScale_(0)
	, nextScale_(0)
{
}

ScaleCommand::~ScaleCommand(void)
{
}

void ScaleCommand::Invoke(Receiver& beforeData)
{
	if (!beforeData.transform || !receiver_.transform) { return; }

	prevScale_ = beforeData.transform->scale;
	nextScale_ = receiver_.transform->scale;

	if (beforeData.transform) { beforeData.transform->scale = nextScale_; }
}

void ScaleCommand::Undo(Receiver& beforeData)
{
	if (receiver_.transform) 
	{
		receiver_.transform->scale = prevScale_; 
		if (beforeData.transform)
		{
			beforeData.transform->scale = prevScale_;
		}
	}
}

void ScaleCommand::Redo(Receiver& beforeData)
{
	if (receiver_.transform)
	{
		receiver_.transform->scale = nextScale_; 
		if (beforeData.transform)
		{
			beforeData.transform->scale = nextScale_;
		}
	}
}
