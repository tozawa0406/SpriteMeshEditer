#include "DeleteCommand.h"

DeleteCommand::DeleteCommand(void) :
	place_(-1)
	, client_(nullptr)
	, parent_(nullptr)
{
}

DeleteCommand::~DeleteCommand(void)
{
}

void DeleteCommand::Uninit(void)
{
	if (receiver_ && place_ >= 0)
	{
		UninitDeletePtr(receiver_);
		if (withChild_)
		{
			for (auto& child : children_)
			{
				UninitChild(child);
			}
		}
	}
}

void DeleteCommand::UninitChild(Receiver* child)
{
	if (!child) { return; }

	for (auto& grandson : child->GetChild())
	{
		UninitChild(grandson);
	}
	UninitDeletePtr(child);
}

void DeleteCommand::Invoke(void)
{
	if (!client_ || !receiver_) { return; }

	place_ = client_->RemoveSprite(receiver_);
	parent_ = receiver_->GetParent();
	receiver_->SetParent(nullptr);

	auto children = receiver_->GetChild();
	for(auto& child : children)
	{
		if(child)
		{ 
			child->SetParent(nullptr);
			children_.emplace_back(child);
			if (withChild_)
			{
				ChildCtrl(child, false);
				if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(child->GetSpriteRenderer()))
				{
					renderer->SetEnable(false);
				}
			}
		}
	}

	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(false);
	}
}

void DeleteCommand::Undo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->AddSprite(receiver_, place_);
	receiver_->SetParent(parent_);

	for (auto& child : children_)
	{
		if (child)
		{
			child->SetParent(receiver_);
			if (withChild_)
			{
				ChildCtrl(child, true);
				if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(child->GetSpriteRenderer()))
				{
					renderer->SetEnable(true);
				}
			}
		}
	}
	children_.clear();

	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(true);
	}

	place_ = -1;
}

void DeleteCommand::Redo(void)
{
	if (!client_ || !receiver_) { return; }

	place_ = client_->RemoveSprite(receiver_);
	parent_ = receiver_->GetParent();
	receiver_->SetParent(nullptr);

	auto children = receiver_->GetChild();
	for (auto& child : children)
	{
		if (child)
		{
			child->SetParent(nullptr);
			children_.emplace_back(child);
			if (withChild_)
			{
				ChildCtrl(child, false);
				if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(child->GetSpriteRenderer()))
				{
					renderer->SetEnable(false);
				}
			}
		}
	}

	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(false);
	}
}

void DeleteCommand::ChildCtrl(Receiver* child, bool add)
{
	if (!child) { return; }

	if (add)
	{
		client_->AddSprite(child, -1);
	}
	else
	{
		client_->RemoveSprite(child);
	}

	for (auto& grandson : child->GetChild())
	{
		ChildCtrl(grandson, add);
	}
}
