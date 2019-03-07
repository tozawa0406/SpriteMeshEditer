/*
 * @file		EditScene.h
 * @brief		エディットシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _EDIT_SCENE_H_
#define _EDIT_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>
#include "../Object/Client/Receiver.h"
#include <FrameWork/Object/ObjectManager.h>
#include <FrameWork/Systems/Camera/CameraManager.h>

class Client;
class EditScene : public BaseScene, public GUI
{
	enum class Mode : uint8
	{
		UNKNOWN	= 0,
		MODEL_EDIT,
		ANIMATION_EDIT,
	};

public:
	/* @brief	コンストラクタ		*/
	EditScene(void);
	/* @brief	デストラクタ		*/
	~EditScene(void);


	
	/* @brief	初期化処理
	 * @param	(sceneNum)	シーン番号
	 * @return	なし				*/
	void Init(SceneList sceneNum) override;

	/* @brief	後処理
	 * @param	なし
	 * @return	なし				*/
	void Uninit(void) override;
	
	/* @brief	更新処理
	 * @param	なし
	 * @return	シーン番号			*/
	SceneList	Update(void) override;

	virtual void GuiUpdate(void) override;
	
private:
	void SelectMode(void);
	void CreateModelEdit(void);

	Client* client_;
	Mode	mode_;
	bool	create_;
};

#endif // _EDIT_SCENE_H_
