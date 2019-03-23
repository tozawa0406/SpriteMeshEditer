#ifndef _UTILTTY_
#define _UTILTTY_

#include "../../Define/Define.h"

/* @brief	全ての基底クラス
 * @detail	自分のポインタを持ち、Releaseを呼んでメモリを解放する	*/
class IBaseUnknown
{
public:
	/* @brief	コンストラクタ	*/
	IBaseUnknown(void) : pointer_(this) {}
	/* @brief	デストラクタ	*/
	virtual ~IBaseUnknown(void) {}

	/* @brief	解放処理(派生川で必ず呼ぶこと)	*/
	virtual void Release(void) { DeletePtr(pointer_); }

private:
	//! 自分自身のポインタ
	IBaseUnknown* pointer_;
};

class IDevice;

/* @brief	頂点バッファ	*/
class IVertexBuffer : public IBaseUnknown
{
public:
	/* @brief	コンストラクタ	*/
	IVertexBuffer(void) : size_(0) {}
	/* @brief	デストラクタ	*/
	virtual ~IVertexBuffer(void) {}

	/* @brief	1頂点のバイト数取得処理		*/
	inline uint GetSize(void) { return size_; }

protected:
	/* @brief	生成処理
	 * @sa		IDeviceから呼び出す			*/
	virtual HRESULT Create(IDevice*, const void*, uint, uint) = 0;

	//! 1頂点のバイト数
	uint size_;
};

/* @brief	インデックスバッファ	*/
class IIndexBuffer : public IBaseUnknown
{
public:
	/* @brief	コンストラクタ	*/
	IIndexBuffer(void) {}
	/* @brief	デストラクタ	*/
	virtual ~IIndexBuffer(void) {}

protected:
	/* @brief	生成処理
	 * @sa		IDeviceから呼び出す			*/
	virtual HRESULT Create(IDevice*, const WORD*, uint) = 0;
};

/* @brief	リソース基底クラス		*/
class IResourece : public IBaseUnknown
{
public:
	/* @brief	コンストラクタ	*/
	IResourece(void) : name_("") {}
	/* @brief	デストラクタ	*/
	virtual ~IResourece(void) {}

	/* @brief	リソース名取得処理	*/
	inline const string& GetName(void) { return name_; }

protected:
	/* @brief	ロード処理
	 * @sa		IDevice				*/
	virtual HRESULT Load(IDevice*, const string&) = 0;

	//! リソース名
	string		name_;
};

/* @brief	テクスチャリソース		*/
class ITextureResource : public IResourece
{
public:
	/* @brief	コンストラクタ	*/
	ITextureResource(void) : size_(VECTOR2(0)) {}
	/* @brief	デストラクタ	*/
	virtual ~ITextureResource(void) {}

	/* @brief	大きさ取得処理		*/
	inline const VECTOR2& GetSize(void) { return size_; }

protected:
	//! 大きさ
	VECTOR2 size_;
};

class TempTextureResource : public ITextureResource
{
public:
	TempTextureResource(void) {}
	virtual ~TempTextureResource(void) {}

	virtual void Release(void) override { IBaseUnknown::Release(); }
	const void SetName(const string& name) { name_ = name; }

private:
	// 空関数を持たせて実態を持てるように
	virtual HRESULT Load(IDevice*, const string&) override { return S_OK; }
};

struct MESH_RESOURCE;
/* @brief	デバイス
 * @detail	リソースの生成を行う	*/
class IDevice : public IBaseUnknown
{
public:
	/* @brief	コンストラクタ	*/
	IDevice(void) {}
	/* @brief	デストラクタ	*/
	virtual ~IDevice(void) {}

	/* @brief	テクスチャロード処理	*/
	virtual HRESULT Load(ITextureResource** textureResource, const string& name) = 0;
	/* @brief	モデルロード処理		*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name) = 0;
	/* @brief	モデルアニメーションのロード処理	*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name, bool anim) = 0;

	/* @brief	頂点バッファ生成処理	*/
	virtual HRESULT CreateBuffer(IVertexBuffer** vertexBuffer, const void* vertex, uint size, uint vertexNum) = 0;
	/* @brief	インデックスバッファの生成処理	*/
	virtual HRESULT CreateBuffer(IIndexBuffer** indexBuffer, const WORD* index, uint indexNum) = 0;
};

#endif // !_UTILITY_
