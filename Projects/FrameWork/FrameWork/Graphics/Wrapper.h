/*
 * @file		Wrapper.h
 * @brief		グラフィックスAPIの描画処理
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _WRAPPER_H_
#define _WRAPPER_H_

#include "../Define/Define.h"
#include "Utility/Utility.h"

// 前方定義
namespace CanvasRenderer { class Image; }
class SpriteRenderer;
struct MeshRenderer;
class  Particle;
struct ColliderRenderer;
struct LightInfo;
class Shader;
class Font;

class Wrapper
{
protected:
	MATRIX inverse_;		//! viewの逆行列

public:	
	static constexpr uint R_ERROR = UINT_MAX;		//! エラー値
	static constexpr uint S_NULL  = UINT_MAX - 1;	//! 初期化時の値

	//! プリミティブの値
	class PRIMITIVE
	{
	public:
		//! @enum	プリミティブの種類
		enum class TYPE
		{
			POINT = 0,
			LINE,
			LINELIST,
			TRIANGLE,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,
		};
		//! 頂点数
		class V
		{
		public:
			static constexpr int CIRCUMFERENCE		= 100;          //! 円周
			static constexpr int POINT				= 1;			//! 点
			static constexpr int LINE				= 2;			//! 線
			static constexpr int RECT				= 5;			//! 四角(くり抜き)
			static constexpr int FILL_TRIANG		= 3;			//! 三角
			static constexpr int FILL_RECT			= 4;			//! 四角
			static constexpr int FILL_CIRCLE		= 102;			//! 円

			//! 当たり判定描画に使用
			static constexpr int COLLIDER_RECTANGLE = 5;
			static constexpr int COLLIDER_CIRCLE	= CIRCUMFERENCE + 1;
			static constexpr int COLLIDER_SEGMENT	= 2;
			static constexpr int COLLIDER_SPHERE	= CIRCUMFERENCE + 1;
		};
	};

	//! @enum	FVF
	enum class FVF
	{
		VERTEX_2D = 0,
		VERTEX_3D,
		VERTEX_POINT,
		INDEX,
	};
	//! @enum	アルファブレンド
	enum class ALFA_BREND
	{
		DEF = 0,
		ADD,
		MAX
	};

	enum class ShaderType
	{
		Vertex,
		Pixel,
		MAX
	};

	//! テクスチャ情報
	struct TextureData
	{
		VECTOR2 size;
	};

	/* @brief	コンストラクタ		*/
	Wrapper(void) : font_(nullptr) { inverse_.Identity(); }
	/* @brief	デストラクタ		*/
	virtual ~Wrapper(void) {}

	/* @brief	初期化処理			*/
	virtual HRESULT Init(void) = 0;
	/* @brief	後処理				*/
	virtual void	Uninit(void) {}

	/* @brief	2D描画の開始処理				*/
	virtual void    BeginDrawCanvasRenderer(void)	= 0;
	/* @brief	3D描画の開始処理				*/
	virtual void    BeginDrawObjectRenderer(void)	= 0;
	/* @brief	描画の終了処理					*/
	virtual void    EndDrawRenderer(void)			= 0;

	/* @brief	テクスチャの設定処理			*/
	virtual void    SetTexture(int stage, ITextureResource* resource) = 0;

	/* @brief	2D描画処理						*/
	virtual void    Draw(const CanvasRenderer::Image*   obj, const Shader* shader) = 0;
	/* @brief	板ポリゴン描画処理				*/
	virtual void    Draw(const SpriteRenderer*   obj, const Shader* shader) = 0;
	/* @brief	モデル描画処理					*/
	virtual void    Draw(MeshRenderer*     obj, const Shader* shader) = 0;
	/* @brief	パーティクル描画処理			*/
	virtual void	Draw(const Particle*	     obj, const Shader* shader) = 0;
	/* @brief	当たり判定描画処理				*/
	virtual void    Draw(const ColliderRenderer* obj) = 0;

	/* @brief	矩形描画処理					*/
	virtual void DrawQuad(VECTOR2 position, VECTOR2 size, COLOR color = COLOR(1, 1, 1, 1)) = 0;

	/* @brief	頂点シェーダーの作成			*/
	virtual uint	CreateVertexShader(string fileName, string method, string version) = 0;
	/* @brief	ピクセルシェーダーの作成		*/
	virtual uint	CreatePixelShader(string fileName, string method, string version) = 0;

	/* @brief	コンスタントバッファの作成		*/
	virtual uint	CreateConstantBuffer(uint size) = 0;
	/* @brief	シェーダーに値を設定			*/
	virtual void	SetShaderValue(const int buffer, const int valueNum, const string* nameArray, const int* valueSizeArray, const void* valueArray) = 0;
	/* @brief	コンスタントバッファの設定		*/
	virtual void	SetConstantBuffer(ShaderType type, int startSrot, int numBuffers, int constantBuffer) = 0;

	/* @brief	ライトの設定					*/
	virtual void SetLight(LightInfo& light) = 0;

	/* @brief	Guiの更新処理		*/
	virtual void GuiUpdate(void) {}

	/* @brief	文字描画クラスの取得			*/
	inline Font* GetFont(void) { return font_; }

protected:
	//! 文字描画クラス
	Font* font_;
};

#endif // _WRAPPER_H