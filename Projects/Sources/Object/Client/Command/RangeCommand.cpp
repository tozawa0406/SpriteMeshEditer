#include "RangeCommand.h"

RangeCommand::RangeCommand(void) :
	prevRange_(0)
	, nextRange_(0)
	, min_(false)
	, animationEditer_(nullptr)
{
}

RangeCommand::~RangeCommand(void)
{
}

void RangeCommand::Invoke(void)
{
	if (!animationEditer_) { return; }
	auto beforeData = animationEditer_->GetAnimationEditerData();

	prevRange_ = (min_) ? beforeData.min : beforeData.max;
	nextRange_ = animationEditer_->GetRange(min_);

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = nextRange_;
	animationEditer_->SetAnimationEditerData(beforeData);
}

void RangeCommand::Undo(void)
{
	if (!animationEditer_) { return; }
	auto beforeData = animationEditer_->GetAnimationEditerData();

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = prevRange_;
	animationEditer_->SetRange(prevRange_, min_);

	animationEditer_->SetAnimationEditerData(beforeData);
}

void RangeCommand::Redo(void)
{
	if (!animationEditer_) { return; }
	auto beforeData = animationEditer_->GetAnimationEditerData();

	int& temp = (min_) ? beforeData.min : beforeData.max;
	temp = nextRange_;
	animationEditer_->SetRange(nextRange_, min_);

	animationEditer_->SetAnimationEditerData(beforeData);
}
