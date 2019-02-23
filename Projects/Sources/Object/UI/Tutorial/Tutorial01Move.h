/*
 * @file		Tutorial01Move.h
 * @brief		移動チュートリアル
 * @author		戸澤翔太
 * @data		2019/02/09
 */
#ifndef _TUTORIAL_01_MOVE_H_
#define _TUTORIAL_01_MOVE_H_

#include "TutorialBase.h"

class Tutorial01Move : public TutorialBase
{
public:
	/* @brief	コンストラクタ		*/
	Tutorial01Move(void);
	/* @brief	デストラクタ		*/
	~Tutorial01Move(void);

	/* @brief	初期化処理			*/
	void			Init(TutorialManager* manager, Controller* ctrl) override;
	/* @brief	後処理				*/
	void			Uninit(void)			override;
	/* @brief	更新処理			*/
	TutorialBase*	Update(void)			override;

private:
	//! スティックUIのポインタ
	CanvasRenderer::Image* stick_;
	//! AWSD 0:a 1:w 2:s 3:d
	CanvasRenderer::Image key_[4];

};

#endif // _TUTORIAL_01_MOVE_H_
