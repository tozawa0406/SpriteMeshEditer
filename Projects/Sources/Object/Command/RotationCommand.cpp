#include "RotationCommand.h"

RotationCommand::RotationCommand(void) : 
	prevRotation_(0)
	, nextRotation_(0)
{
}

RotationCommand::~RotationCommand(void)
{
}

void RotationCommand::Invoke(Receiver& beforeData)
{
	if (!beforeData.transform || !receiver_.transform) { return; }

	prevRotation_ = beforeData.transform->rotation;
	nextRotation_ = receiver_.transform->rotation;

	if (beforeData.transform) { beforeData.transform->rotation = nextRotation_; }
}

void RotationCommand::Undo(Receiver& beforeData)
{
	if (receiver_.transform) 
	{
		receiver_.transform->rotation = prevRotation_; 
		if (beforeData.transform)
		{
			beforeData.transform->rotation = prevRotation_;
		}
	}
}

void RotationCommand::Redo(Receiver& beforeData)
{
	if (receiver_.transform)
	{
		receiver_.transform->rotation = nextRotation_; 
		if (beforeData.transform)
		{
			beforeData.transform->rotation = nextRotation_;
		}
	}
}
