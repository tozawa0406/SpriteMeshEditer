//-----------------------------------------------------------------------------
//
//	GUI[GUI.cpp]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "GUI.h"
#include "../GameSystems.h"
#include "GuiManager.h"

GUI::GUI(Systems* systems, Object* obj, string name) : guiManager(nullptr)
{
	this->obj  = obj;
	this->name = name;
	this->tag  = name;

	if (systems)
	{
		this->guiManager = systems->GetDebug()->GetGuiManager();
		guiManager->Add(this);
	}
}

//デストラクタ
GUI::~GUI(void)
{
	guiManager->Remove(this);
}
