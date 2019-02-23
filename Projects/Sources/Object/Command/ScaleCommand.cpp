#include "ScaleCommand.h"

ScaleCommand::ScaleCommand(void) : 
	prevScale_(0)
	, newScale_(0)
{
}

ScaleCommand::~ScaleCommand(void)
{
}

void ScaleCommand::Invoke(void)
{
}

void ScaleCommand::Undo(void)
{
	reciver_->scale = prevScale_;
}

void ScaleCommand::Redo(void)
{
	reciver_->scale = newScale_;
}

void ScaleCommand::SetScale(const VECTOR3& prevScale, const VECTOR3& newScale)
{
	prevScale_ = prevScale;
	newScale_  = newScale;
}
