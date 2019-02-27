#include "PositionCommand.h"
#include "Receiver.h"

PositionCommand::PositionCommand(void) : 
	prevPosition_(0)
	, nextPosition_(0)
{
}

PositionCommand::~PositionCommand(void)
{
}

void PositionCommand::Invoke(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	prevPosition_ = beforeTransform->position;
	nextPosition_ = receiver_->GetTransform().position;

	beforeTransform->position = nextPosition_;
}

void PositionCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.position = prevPosition_;
	receiver_->SetTransform(transform);
	beforeTransform->position = prevPosition_;
}

void PositionCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.position = nextPosition_;
	receiver_->SetTransform(transform);
	beforeTransform->position = nextPosition_;
}
