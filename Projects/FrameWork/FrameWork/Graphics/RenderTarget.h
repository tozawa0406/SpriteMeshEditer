/*
 * @file		RenderTarget.h
 * @brief		グラフィックスAPI毎のレンダーターゲットの処理
 * @author		戸澤翔太
 * @data		2018/12/21
 */

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "../Define/Define.h"
#include "Utility/CascadeManager.h"

class RenderTarget
{
public:
	/* @enum	レンダーターゲットの種類	*/
	enum class List : uint8
	{
		DEFAULT = 0,
		COLOR,
		POSITION,
		NORMAL,
		BLUR1,
		BLUR2,
		MAX
	};

	//! @def	描画の大きさ横
	static constexpr float SIZE_X = 240;
	//! @def	描画の大きさ縦
	static constexpr float SIZE_Y = 135;

	/* @brief	コンストラクタ		*/
	RenderTarget(void) : debugDraw_(List::MAX), cascade_(nullptr), feedbackBlur_(0), coefficientBlur_(0) {}
	/* @brief	デストラクタ		*/
	virtual ~RenderTarget(void) {}

	/* @brief	初期化処理			*/
	virtual HRESULT Init(void)		= 0;
	/* @brief	後処理				*/
	virtual void	Uninit(void)	= 0;

	/* @brief	レンダーターゲットのクリア処理			*/
	virtual void ClearRendererTarget(List num, COLOR color) = 0;

	/* @brief	影の描画開始		*/
	virtual void BeginDrawShadow(int i) = 0;
	/* @brief	影の描画終了		*/
	virtual void EndDrawShadow(void) = 0;

	/* @brief	マルチレンダーターゲットの描画開始		*/
	virtual void BeginMultiRendererTarget(void) = 0;
	/* @brief	マルチレンダーターゲットの描画終了		*/
	virtual void EndMultiRendererTarget(void)	= 0;


	/* @brief	Guiの更新処理		*/
	virtual void GuiUpdate(void) {}

	/* @brief	デバッグ用描画処理	*/
	virtual void Draw(List num, VECTOR2 position, VECTOR2 size) = 0;
	/* @brief	デバッグ用描画処理	*/
	virtual void DrawShadowMap(void) = 0;

	/* @brief	デバッグ用に描画するレンダーターゲットの設定
	 * @param	(num)	描画したいレンダーターゲットの種類
	 * @return	なし				*/
	inline void SetDebugDraw(List num) { debugDraw_ = num; }

	/* @brief	カスケードマネージャーの取得
	 * @param	なし
	 * @return	カスケードマネージャー		*/
	inline CascadeManager* GetCascadeManager(void) { return cascade_; }

	/* @brief	ブラーテクスチャ
	 * @param	(blur)	true時にブラー処理
	 * @return	なし				*/
	inline void FeedbackBlur(int8 blur) { feedbackBlur_ = blur; coefficientBlur_ = 0.025f / blur; }

	/* @brief	ブラーの確認処理
	 * @param	なし
	 * @return	ブラーしてるかどうか	*/
	inline bool IsFeedbackBlur(void) { return (feedbackBlur_ < 0) ? true : false; }


protected:
	//! デバッグで画面に描画するレンダーターゲットの種類
	List debugDraw_;
	//! カスケードのマネージャー
	CascadeManager* cascade_;
	//! フィードバックブラーフラグ
	int8 feedbackBlur_;
	//! フィードバックブラー終了フェード
	float coefficientBlur_;
};

#endif // _RENDER_TARGET_H_