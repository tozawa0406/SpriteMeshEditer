#include "RotationCommand.h"
#include "../Receiver.h"

RotationCommand::RotationCommand(void) : 
	prevRotation_(0)
	, nextRotation_(0)
{
}

RotationCommand::~RotationCommand(void)
{
}

void RotationCommand::Invoke(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	prevRotation_ = beforeTransform->rotation;
	nextRotation_ = receiver_->GetTransform().rotation;

	beforeTransform->rotation = nextRotation_;
}

void RotationCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.rotation = prevRotation_;
	receiver_->SetTransform(transform);
	beforeTransform->rotation = prevRotation_;
}

void RotationCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.rotation = nextRotation_;
	receiver_->SetTransform(transform);
	beforeTransform->rotation = nextRotation_;
}
