#ifndef _DX11_UTILITY_
#define _DX11_UTILITY_

#include "../../Utility/Utility.h"
#include "../Dx11Include.h"

/* @brief	DirectX11のデバイス			*/
class D3D11Device : public IDevice
{
public:
	/* @brief	コンストラクタ		*/
	D3D11Device(void) : device_(nullptr) {}
	/* @brief	ですトラ歌			*/
	virtual ~D3D11Device(void) {}

	/* @brief	解放処理			*/
	void			Release(void);
	/* @brief	デバイス取得処理	*/
	ID3D11Device*	GetDevice(void) { return device_; }
	/* @brief	デバイスの設定処理	*/
	void			SetDevice(ID3D11Device* device) { assert(device); device_ = device; }

	/* @brief	テクスチャロード処理	*/
	virtual HRESULT Load(ITextureResource** resource, const string& name) override;

	/* @brief	頂点バッファ生成処理	*/
	virtual HRESULT CreateBuffer(IVertexBuffer** vertexBuffer, const void* vertex, uint size, uint vertexNum) override;

private:
	//! デバイス本体
	ID3D11Device* device_;
};

/* @brief	頂点バッファ				*/
class VertexBuffer : public IVertexBuffer
{
public:
	/* @brief	コンストラクタ			*/
	virtual ~VertexBuffer(void) {}

	/* @brief	バッファ取得処理		*/
	inline  ID3D11Buffer*	GetBuffer(void) { return buffer_; }
	/* @brief	解放処理				*/
	virtual void			Release(void) override;

private:
	/* @brief	コンストラクタ			*/
	VertexBuffer(void) : buffer_(nullptr) {}
	/* @brief	バッファ生成処理		*/
	virtual HRESULT Create(IDevice* device, const void* vertex, uint size, uint vertexNum) override;

	//! バッファ本体
	ID3D11Buffer* buffer_;
	//! 機能制限のためのフレンドクラス
	friend D3D11Device;
};

/* @brief	インデックスバッファ		*/
class IndexBuffer : public IIndexBuffer
{
public:
	/* @brief	デストラクタ		*/
	virtual ~IndexBuffer(void) {}

	/* @brief	バッファ取得処理	*/
	inline  ID3D11Buffer*	GetBuffer(void) { return buffer_; }
	/* @brief	解放処理			*/
	virtual void			Release(void) override {};

private:
	/* @brief	コンストラクタ		*/
	IndexBuffer(void) {}
	/* @brief	バッファ生成処理	*/
	virtual HRESULT Create(IDevice*, const WORD*, uint) override {};

	//! バッファ本体
	ID3D11Buffer* buffer_;
	//! 機能制限のためのフレンドクラス
	friend D3D11Device;
};

/* @brief	テクスチャリソース			*/
class TextureResource : public ITextureResource
{
public:
	/* @brief	デストラクタ			*/
	virtual ~TextureResource(void) {}

	/* @brief	テクスチャ取得処理		*/
	inline ID3D11ShaderResourceView*	GetResource(void) { return resource_; }
	/* @brief	解放処理				*/
	virtual void						Release(void) override;

private:
	/* @brief	コンストラクタ			*/
	TextureResource(void) : resource_(nullptr) {}
	/* @brief	ロード処理				*/
	virtual HRESULT Load(IDevice* device, const string& name) override;

	//! テクスチャ本体
	ID3D11ShaderResourceView* resource_;
	//! 機能制限のためのフレンドクラス
	friend D3D11Device;
};

#endif // !_DX11_UTILITY_
