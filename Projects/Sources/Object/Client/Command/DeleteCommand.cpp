#include "DeleteCommand.h"

DeleteCommand::DeleteCommand(void) :
	client_(nullptr)
	, parent_(nullptr)
{
}

DeleteCommand::~DeleteCommand(void)
{
}

void DeleteCommand::Uninit(void)
{
	if (receiver_)
	{
		auto list = client_->GetReceiverList();
		for (auto& receiver : list) { if (receiver == receiver_) { return; } }

		if (receiver_->GetPrevCommand() == this)
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

	client_->RemoveSprite(receiver_);
	parent_ = receiver_->GetParent();
	receiver_->SetParent(nullptr);
	receiver_->SetPrevDelete(this);

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

	client_->AddSprite(receiver_);
	receiver_->SetParent(parent_);

	for (auto& child : children_)
	{
		if (child)
		{
			child->SetParent(receiver_);
			if (withChild_)
			{
				ChildCtrl(child, true);
			}
		}
	}
	children_.clear();

	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(true);
	}
}

void DeleteCommand::Redo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->RemoveSprite(receiver_);
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

	child->SetPrevDelete(this);
	if (add)
	{
		client_->AddSprite(child);
	}
	else
	{
		client_->RemoveSprite(child);
	}

	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(child->GetSpriteRenderer()))
	{
		renderer->SetEnable(add);
	}

	for (auto& grandson : child->GetChild())
	{
		ChildCtrl(grandson, add);
	}
}
