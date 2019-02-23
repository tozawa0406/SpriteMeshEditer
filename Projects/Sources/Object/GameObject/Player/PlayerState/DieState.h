/*
 * @file		DieState.h
 * @brief		死亡処理
 * @author		戸澤翔太
 * @data		2018/11/14
 */
#ifndef _DIE_STATE_H_
#define _DIE_STATE_H_

#include "PlayerState.h"

class DieState : public PlayerState
{
public:
	/* @brief	コンストラクタ		*/
	DieState(void);
	/* @brief	デストラクタ		*/
	~DieState(void);


	
	/* @brief	初期化処理
	 * @param	(player)	プレイヤーへのポインタ
	 * @param	(ctrl)		コントローラへのポインタ
	 * @return	なし				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	次のステート		*/
	PlayerState* Update(void) override;
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし
	 * @detail	プレイヤーから呼ばれる	*/
	void GuiUpdate(void) override;
};

#endif // _DIE_STATE_H_
