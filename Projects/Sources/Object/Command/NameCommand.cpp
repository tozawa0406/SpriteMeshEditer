#include "NameCommand.h"
#include "Receiver.h"

NameCommand::NameCommand(void) : 
	prevName_("")
	, nextName_("")
{
}

NameCommand::~NameCommand(void)
{
}

void NameCommand::Invoke(void)
{
	if (!receiver_) { return; }

	prevName_ = *receiver_->GetBeforeData().name;
	nextName_ = receiver_->GetName();

	auto beforeData = receiver_->GetBeforeData();
	*beforeData.name = nextName_;
	receiver_->SetBeforeData(beforeData);
}

void NameCommand::Undo(void)
{
	if (!receiver_) { return; }

	receiver_->SetName(prevName_);

	auto beforeData = receiver_->GetBeforeData();
	*beforeData.name = prevName_;
	receiver_->SetBeforeData(beforeData);
}

void NameCommand::Redo(void)
{
	if (!receiver_) { return; }

	receiver_->SetName(nextName_);

	auto beforeData = receiver_->GetBeforeData();
	*beforeData.name = nextName_;
	receiver_->SetBeforeData(beforeData);
}
