#include "GuiManager.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_impl_dx11.h"

#include "../../Windows/Windows.h"
#include "../../Graphics/DirectX11/DirectX11.h"

#include "../GameSystems.h"
#include "Debug.h"
#include "../../Scene/SceneManager.h"
#include "../Input/Controller.h"

static constexpr int   WINDOW_MARGIN_X = 20;
static constexpr int   WINDOW_MARGIN_Y = 20;
static constexpr float WINDOW_WIDTH = (Windows::WIDTH - WINDOW_MARGIN_X * 6) / 5.0f;
static constexpr float WINDOW_HEIGHT = (Windows::HEIGHT - WINDOW_MARGIN_Y * 6) / 5.0f;

/* @fn		コンストラクタ
 * @brief	変数の初期化			*/
GuiManager::GuiManager(void) : BaseManager(Systems::Instance())
	, debug_(nullptr), guiObject_(false), guiSubject_(false)
	, lookObject_(false), draw_(true), drawNum_(0), frame_(0)
{
}

/* @fn		デストラクタ
 * @brief	...						*/
GuiManager::~GuiManager(void)
{
}

/* @fn		Init
 * @brief	初期化
 * @param	なし
 * @return	初期化に成功したかどうか		*/
HRESULT GuiManager::Init(void)
{
	assert(debug_);
	const auto& window = systems_->GetWindow();
	if (!window) { return E_FAIL; }

	ImGui::CreateContext();
	if (!ImGui_ImplWin32_Init(window->GetHWND())) { return E_FAIL; }
	if (window->GetGraphicsType() == Graphics::Type::DirectX9)
	{
	}
	else if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		DirectX11* directX = static_cast<DirectX11*>(window->GetGraphics());
		if (!directX) { return E_FAIL; }

		if (!ImGui_ImplDX11_Init(directX->GetD3D11Device(), directX->GetDeviceContext())) { return E_FAIL; }
	}

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Resource/Data/ProggyClean.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	obj_.clear();
	return S_OK;
}

/* @fn		Uninit
 * @brief	後処理
 * @param	なし
 * @return	なし					*/
void GuiManager::Uninit(void)
{
	// オブジェクトリストのクリア
	for (int i = 0; i < static_cast<int>(obj_.size());)
	{
		RemoveVector(obj_, obj_[0]);
	}

	const auto& window = systems_->GetWindow();
	if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		ImGui_ImplDX11_Shutdown();
	}
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

/* @fn		GuiUpdate
 * @brief	Guiの更新
 * @param	なし
 * @return	なし					*/
void GuiManager::GuiUpdate(void)
{
	if (!systems_ || !debug_) { return; }
	const auto& window = systems_->GetWindow();
	if (!window) { return; }


	// 描画していないなら更新しない(ImGuiがエラーになる)
	if (!draw_) { return; }

	draw_ = false;
	if (window->GetGraphicsType() == Graphics::Type::DirectX9)
	{
	}
	else if (window->GetGraphicsType() == Graphics::Type::DirectX11)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	else
	{
		return;
	}

	for (auto& obj : obj_)
	{
		if (obj)
		{
			obj->GuiUpdate();
		}
	}
}

void GuiManager::Update(void)
{
	GuiUpdate();
	if (frame_ > 0)
	{
		frame_++;
		if (frame_ > SELECT_GUI + 1)
		{
			frame_ = 0;
		}
	}
}

const VECTOR3* GuiManager::GetSelect(void)
{
	for (auto g : obj_)
	{
		if (g->obj != nullptr)
		{
			if (g->window)
			{
				return &g->obj->GetTransform().position;
			}
		}
	}
	return nullptr;
}

void GuiManager::Draw(void)
{
	if (draw_) { return; }
	draw_ = true;

	if (!systems_) { return; }
	const auto& window = systems_->GetWindow();
	if (!window) { return; }

	const auto& type = window->GetGraphicsType();
	if (type == Graphics::Type::DirectX9 ||
		type == Graphics::Type::DirectX11)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}
