#include "RangeCommand.h"

RangeCommand::RangeCommand(void) :
	prevRange_(0)
	, nextRange_(0)
	, min_(false)
	, animationEditor_(nullptr)
{
}

RangeCommand::~RangeCommand(void)
{
}

void RangeCommand::Invoke(void)
{
	if (!animationEditor_) { return; }
	auto beforeData = animationEditor_->GetAnimationEditorData();

	prevRange_ = (min_) ? beforeData.min : beforeData.max;
	nextRange_ = animationEditor_->GetRange(min_);

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = nextRange_;
	animationEditor_->SetAnimationEditorData(beforeData);
}

void RangeCommand::Undo(void)
{
	if (!animationEditor_) { return; }
	auto beforeData = animationEditor_->GetAnimationEditorData();

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = prevRange_;
	animationEditor_->SetRange(prevRange_, min_);

	animationEditor_->SetAnimationEditorData(beforeData);
}

void RangeCommand::Redo(void)
{
	if (!animationEditor_) { return; }
	auto beforeData = animationEditor_->GetAnimationEditorData();

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = nextRange_;
	animationEditor_->SetRange(nextRange_, min_);

	animationEditor_->SetAnimationEditorData(beforeData);
}
