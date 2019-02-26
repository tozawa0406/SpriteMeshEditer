/*
 * @file		TitleScene.h
 * @brief		タイトルシーン
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _TITLE_SCENE_H_
#define _TITLE_SCENE_H_

#include <FrameWork/Scene/BaseScene.h>
#include "../Object/Command/Client.h"
#include <FrameWork/Object/ObjectManager.h>

#include "../Object/Pivot.h"

class TitleScene : public BaseScene, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	TitleScene(void);
	/* @brief	デストラクタ		*/
	~TitleScene(void);


	
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
	
	/* @brief	Guiの更新処理
	 * @param	なし
	 * @return	なし				*/
	void InspectorView(void) override;

	void HierarchyView(void) override;

	void ConsoleView(void) override;

private:
	void CreateClient(void);

	ObjectManager*	objectManager_;
	std::vector<Client*>	clientList_;
	Client*					currentClient_;

	Pivot* pivot_;

};

#endif // _TITLE_SCENE_H_
