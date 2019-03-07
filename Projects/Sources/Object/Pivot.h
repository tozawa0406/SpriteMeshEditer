/*
 * @file		Pivot.h
 * @brief		ピボットの描画
 * @author		戸澤翔太
 * @data		2019/03/07
 */
#ifndef _PIVOT_H_
#define _PIVOT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Pivot : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Pivot(void);
	/* @brief	デストラクタ		*/
	virtual ~Pivot(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)		override;
	/* @brief	後処理				*/
	virtual void Uninit(void)	override;
	/* @brief	更新処理			*/
	virtual void Update(void)	override;

	/* @brief	トランスフォームの設定	*/
	void SetTransform(const Transform& transform) { transform_.parent = &transform; }

private:
	//! トランスフォーム
	Transform		transform_;
	//! スプライト
	SpriteRenderer* pivot_;
};

#endif // _PIVOT_H_
