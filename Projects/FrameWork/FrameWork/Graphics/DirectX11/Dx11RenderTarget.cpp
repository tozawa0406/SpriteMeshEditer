#include "Dx11RenderTarget.h"
#include "DirectX11.h"
#include "../../Windows/Windows.h"
#include "../../Systems/Renderer/Sprite/Texture.h"

#include "../../Systems/GameSystems.h"
#include "../../Systems/Renderer/Shader/ZTexture.h"
#include "../../Scene/SceneManager.h"
#include "../../Systems/Light.h"
#include "../../Systems/Camera/CameraManager.h"

/* @biref	コンストラクタ
 * @param	(dx11)	親のポインタ		*/
Dx11RenderTarget::Dx11RenderTarget(DirectX11* dx11) :
	directX11_(dx11)
	, drawShadowMap_(false)
{
	for (auto& r : renderTargetView_)	{ r = nullptr; }
	for (auto& s : shaderResourceView_) { s = nullptr; }
}

/* @brief	デストラクタ				*/
Dx11RenderTarget::~Dx11RenderTarget(void)
{
}

/* @brief	初期化処理
 * @param	なし
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::Init(void)
{
	if (!directX11_)	{ return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window)		{ return E_FAIL; }

	const auto& swapChain = directX11_->GetSwapChain();
	if (!swapChain) { return E_FAIL; }

	// バックバッファの情報を取得
	renderTargetView_[static_cast<int>(List::DEFAULT)] = directX11_->GetRenderTargetView();
	depthStencilView_ = directX11_->GetDepthStencilView();

	HRESULT hr;
	hr = CreateNormalRenderTarget(List::COLOR);
	if (window->ErrorMessage("カラーの初期化に失敗しました。"		, "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::POSITION);					 
	if (window->ErrorMessage("ポジションの初期化に失敗しました。"	, "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::NORMAL);
	if (window->ErrorMessage("ノーマルの初期化に失敗しました。"		, "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::BLUR1);
	if (window->ErrorMessage("ブラーの初期化に失敗しました。", "エラー", hr)) { return hr; }
	hr = CreateNormalRenderTarget(List::BLUR2);
	if (window->ErrorMessage("ブラーの初期化に失敗しました。", "エラー", hr)) { return hr; }


	hr = CreateShadowmapRenderTarget();
	if (window->ErrorMessage("シャドウマップの初期化に失敗しました。", "エラー", hr)) { return hr; }

//	cascade_ = new CascadeManager;
	if (cascade_)
	{
//		cascade_->Init();
	}

	return S_OK;
}

/* @brief	後処理
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::Uninit(void)
{
	for (auto& p : shadowState_.pDSV) { ReleasePtr(p); }
	for (auto& p : shadowState_.pDepthSRV) { ReleasePtr(p); }
	ReleasePtr(shadowState_.pSmp);

	UninitDeletePtr(cascade_);
	ReleasePtr(depthStencilView_);

	for (auto& r : renderTargetView_)	{ ReleasePtr(r); }
	for (auto& s : shaderResourceView_) { ReleasePtr(s); }

}

/* @brief	普通のレンダーターゲットの作成
 * @param	(num)	レンダーターゲットの種類
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateNormalRenderTarget(List num)
{
	if (!directX11_) { return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window) { return E_FAIL; }

	const auto& swapChain = directX11_->GetSwapChain();
	const auto& device = directX11_->GetD3D11Device();

	if (!swapChain) { return E_FAIL; }
	if (!device) { return E_FAIL; }

	ID3D11Texture2D* temp;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&temp);

	D3D11_TEXTURE2D_DESC td;
	temp->GetDesc(&td);
	td.SampleDesc.Count		= 1;
	td.SampleDesc.Quality	= 0;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* tex2D;
	HRESULT hr = device->CreateTexture2D(&td, nullptr, &tex2D);
	if (window->ErrorMessage("テクスチャ2Dの作成に失敗しました。", "エラー", hr)) { return hr; }

	if (FAILED(CreateRenderTarget(num, tex2D, nullptr, nullptr))) { return E_FAIL; }

	ReleasePtr(tex2D);

	return S_OK;
}

/* @brief	シャドウマップのレンダーターゲットの作成
 * @param	なし
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateShadowmapRenderTarget(void)
{
	const auto& device = directX11_->GetD3D11Device();

	HRESULT hr = S_OK;

	for (int i = 0; i < CascadeManager::MAX_CASCADE; ++i)
	{
		ID3D11Texture2D* pDepthTex;
		// 深度テクスチャの生成
		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));

		texDesc.Width				= CascadeManager::MAP_SIZE;
		texDesc.Height				= CascadeManager::MAP_SIZE;
		texDesc.Format				= DXGI_FORMAT_R16_TYPELESS;
		texDesc.MipLevels			= 1;
		texDesc.ArraySize			= 1;
		texDesc.SampleDesc.Count	= 1;
		texDesc.SampleDesc.Quality	= 0;
		texDesc.Usage				= D3D11_USAGE_DEFAULT;
		texDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags		= 0;
		texDesc.MiscFlags			= 0;

		hr = device->CreateTexture2D(&texDesc, nullptr, &pDepthTex);
		if (FAILED(hr)) { return hr; }


		// 深度ステンシルビューの生成
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format				= DXGI_FORMAT_D16_UNORM;
		dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice	= 0;

		hr = device->CreateDepthStencilView(pDepthTex, &dsvDesc, &shadowState_.pDSV[i]);
		if (FAILED(hr)) { return hr; }


		// 深度用シェーダリソースビューの生成
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format						= DXGI_FORMAT_R16_UNORM;
		srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels			= 1;
		srvDesc.Texture2D.MostDetailedMip	= 0;

		hr = device->CreateShaderResourceView(pDepthTex, &srvDesc, &shadowState_.pDepthSRV[i]);
		if (FAILED(hr)) { return hr; }

		ReleasePtr(pDepthTex);
	}

	// ビューポートの設定
	shadowState_.viewport.Width		= static_cast<float>(CascadeManager::MAP_SIZE);
	shadowState_.viewport.Height	= static_cast<float>(CascadeManager::MAP_SIZE);
	shadowState_.viewport.TopLeftX	= 0.0f;
	shadowState_.viewport.TopLeftY	= 0.0f;
	shadowState_.viewport.MinDepth	= 0.0f;
	shadowState_.viewport.MaxDepth	= 1.0f;

	// サンプラーステートの生成
	D3D11_SAMPLER_DESC sDesc;
	ZeroMemory(&sDesc, sizeof(sDesc));
	sDesc.AddressU			= D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressV			= D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.AddressW			= D3D11_TEXTURE_ADDRESS_BORDER;
	sDesc.BorderColor[0]	= 1.0f;
	sDesc.BorderColor[1]	= 1.0f;
	sDesc.BorderColor[2]	= 1.0f;
	sDesc.BorderColor[3]	= 1.0f;
	sDesc.ComparisonFunc	= D3D11_COMPARISON_LESS_EQUAL;
	sDesc.Filter			= D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sDesc.MaxAnisotropy		= 1;
	sDesc.MipLODBias		= 0;
	sDesc.MinLOD			= -FLT_MAX;
	sDesc.MaxLOD			= +FLT_MAX;

	// サンプラーステートを生成
	hr = device->CreateSamplerState(&sDesc, &shadowState_.pSmp);
	if (FAILED(hr)) { return hr; }

	return hr;
}

/* @brief	レンダーターゲットの作成
 * @param	(num)	レンダーターゲットの種類
 * @param	(tex2D)	テクスチャリソース
 * @return	成功失敗					*/
HRESULT Dx11RenderTarget::CreateRenderTarget(List num, ID3D11Texture2D* tex2D, D3D11_RENDER_TARGET_VIEW_DESC* rtDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srDesc)
{
	if (!directX11_)	{ return E_FAIL; }
	const auto& window = directX11_->GetWindow();
	if (!window)		{ return E_FAIL; }
	const auto& device = directX11_->GetD3D11Device();
	if (!device)	{ return E_FAIL; }

	int listNum = static_cast<int>(num);
	HRESULT hr;
	hr = device->CreateRenderTargetView(tex2D, (rtDesc) ? rtDesc : 0, &renderTargetView_[listNum]);
	if (window->ErrorMessage("レンダーターゲットビューの作成に失敗しました。", "エラー", hr)) { return hr; }

	hr = device->CreateShaderResourceView(tex2D, (srDesc) ? srDesc : 0, &shaderResourceView_[listNum]);
	if (window->ErrorMessage("シェーダーリソースビューの作成に失敗しました。", "エラー", hr)) { return hr; }

	return S_OK;
}

/* @brief	レンダーターゲットのクリア
 * @param	(num)	レンダーターゲットの種類
 * @return	なし						*/
void Dx11RenderTarget::ClearRendererTarget(List num, COLOR color)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	int listNum = static_cast<int>(num);

	context->ClearRenderTargetView(renderTargetView_[listNum], (float*)color);
	context->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

/* @brief	マルチレンダーターゲットの開始
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::BeginMultiRendererTarget(void)
{
	if (!directX11_)	{ return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context)		{ return; }

	ID3D11RenderTargetView* nullRTV = nullptr; 
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->OMSetRenderTargets(3, &renderTargetView_[static_cast<int>(List::COLOR)], depthStencilView_);
	COLOR color[3] = { COLOR(0.5f, 0.2f, 0.3f, 1), COLOR(0.2f, 0.5f, 0.3f, 1), COLOR(0.3f, 0.2f, 0.5f, 1) };
	for (int i = 0; i < 3; ++i)
	{
		int n = static_cast<int>(List::COLOR) + i;
		ClearRendererTarget(static_cast<List>(n), COLOR::RGBA(32, 100, 92, 255));
	}
}

/* @brief	マルチレンダーターゲットの終了
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::EndMultiRendererTarget(void)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }
	const auto& wrapper = directX11_->GetWrapper();
	if (!wrapper) { return; }

	int rtvNum = static_cast<int>((feedbackBlur_) ? List::BLUR1 : List::DEFAULT);
	context->OMSetRenderTargets(1, &renderTargetView_[rtvNum], depthStencilView_);

	int draw = static_cast<int>((debugDraw_ == List::MAX) ? List::COLOR : debugDraw_);
	context->PSSetShaderResources(0, 1, &shaderResourceView_[draw]);
	wrapper->DrawQuad(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)), VECTOR2(Windows::WIDTH, Windows::HEIGHT));

	if (feedbackBlur_)
	{
		float magnification = (feedbackBlur_ < 0) ? 1.025f : 1 + (coefficientBlur_ * feedbackBlur_);

		int def = static_cast<int>(List::DEFAULT);
		int blur1 = static_cast<int>(List::BLUR1);
		int blur2 = static_cast<int>(List::BLUR2);
		context->PSSetShaderResources(0, 1, &shaderResourceView_[blur2]);
		wrapper->DrawQuad(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)), VECTOR2(Windows::WIDTH, Windows::HEIGHT) * magnification, COLOR(1, 1, 1, 0.8f));
		context->OMSetRenderTargets(1, &renderTargetView_[def], depthStencilView_);
		ClearRendererTarget(List::DEFAULT, COLOR::RGBA(32, 100, 92, 255));

		context->PSSetShaderResources(0, 1, &shaderResourceView_[blur1]);
		wrapper->DrawQuad(VECTOR2(Half(Windows::WIDTH), Half(Windows::HEIGHT)), VECTOR2(Windows::WIDTH, Windows::HEIGHT));

		auto tempSRV = shaderResourceView_[blur1];
		shaderResourceView_[blur1] = shaderResourceView_[blur2];
		shaderResourceView_[blur2] = tempSRV;

		auto tempRTV = renderTargetView_[blur1];
		renderTargetView_[blur1] = renderTargetView_[blur2];
		renderTargetView_[blur2] = tempRTV;

		feedbackBlur_--;
		if (feedbackBlur_ == 0)
		{
			ClearRendererTarget(List::BLUR1, COLOR(0));
			ClearRendererTarget(List::BLUR2, COLOR(0));
		}
	}
}

/* @brief	シャドウマップの描画開始
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::BeginDrawShadow(int i)
{
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

//	if (i == 0) { cascade_->ComputeShadowMatrixPSSM(); }

	ID3D11RenderTargetView* pRTV = nullptr;
	context->OMSetRenderTargets(1, &pRTV, shadowState_.pDSV[i]);

	context->ClearDepthStencilView(shadowState_.pDSV[i], D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->RSSetViewports(1, &shadowState_.viewport);
}

/* @brief	シャドウマップの描画終了
 * @param	なし
 * @return	なし						*/
void Dx11RenderTarget::EndDrawShadow(void)
{
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }

	ID3D11RenderTargetView * nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, 0);
	context->RSSetViewports(1, &directX11_->GetViewport());
	context->OMSetRenderTargets(1, &renderTargetView_[static_cast<int>(List::DEFAULT)], depthStencilView_);

	context->PSSetSamplers(3, 1, &shadowState_.pSmp);	
	context->PSSetShaderResources(3, 4, shadowState_.pDepthSRV);
}

/* @brief	レンダーターゲットの描画
 * @param	(num)	レンダーターゲットの種類
 * @return	なし						*/
void Dx11RenderTarget::Draw(List num, VECTOR2 position, VECTOR2 size)
{
	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }
	const auto& wrapper = directX11_->GetWrapper();
	if (!wrapper) { return; }

	int listNum = static_cast<int>(num);
	context->PSSetShaderResources(0, 1, &shaderResourceView_[listNum]);
	wrapper->DrawQuad(position, size);

	ID3D11ShaderResourceView* temp = nullptr;
	context->PSSetShaderResources(0, 1, &temp);
}

void Dx11RenderTarget::DrawShadowMap(void)
{
	if (!drawShadowMap_) { return; }

	if (!directX11_) { return; }
	const auto& context = directX11_->GetDeviceContext();
	if (!context) { return; }
	const auto& wrapper = directX11_->GetWrapper();
	if (!wrapper) { return; }

	int i = 0;
	for (auto& srv : shadowState_.pDepthSRV)
	{
		context->PSSetShaderResources(0, 1, &srv);
		wrapper->DrawQuad(VECTOR2(10.0f + 100 + (i * 210), Windows::HEIGHT - 10 - 100.0f), VECTOR2(200, 200));
		i++;
	}
}

void Dx11RenderTarget::GuiUpdate(void)
{
}
