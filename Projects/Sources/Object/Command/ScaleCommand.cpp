#include "ScaleCommand.h"
#include "Receiver.h"

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
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	prevScale_ = beforeTransform->scale;
	nextScale_ = receiver_->GetTransform().scale;

	beforeTransform->scale = nextScale_;
}

void ScaleCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.scale = prevScale_;
	receiver_->SetTransform(transform);
	beforeTransform->scale = prevScale_;
}

void ScaleCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto beforeTransform = receiver_->GetBeforeData().transform;
	if (!beforeTransform) { return; }

	Transform transform = receiver_->GetTransform();
	transform.scale = nextScale_;
	receiver_->SetTransform(transform);
	beforeTransform->scale = nextScale_;
}
