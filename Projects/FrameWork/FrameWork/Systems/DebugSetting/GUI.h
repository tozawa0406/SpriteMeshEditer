//-----------------------------------------------------------------------------
//
//	GUI[GUI.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _GUI_H_
#define _GUI_H_

#include "../../Define/Define.h"
#include "../../Object/Object.h"
#include "ImGUI/imgui.h"

class Systems;
class GUI
{
	friend  class GuiManager;
public:
	GUI(Systems* systems, Object* obj, string name = "");
	~GUI(void);
	virtual void GuiUpdate(void) {}
	virtual void InspectorView(void)	{}
	virtual void ConsoleView(void)		{}
	virtual void HierarchyView(void)	{}

protected:
	string name;

private:
	Object* obj;
	string tag;
	bool    window = false;
	int     drawNum;
	GuiManager* guiManager;
};

namespace ImGui
{
	inline void TextAlign(const string& text) { ImGui::Text(text.c_str()); ImGui::SameLine(); }
}

#endif // _GUI_H_
