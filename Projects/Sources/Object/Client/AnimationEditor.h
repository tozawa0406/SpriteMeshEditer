/*
 * @file		AnimationEditor.h
 * @brief		アニメーションエディタ
 * @author		戸澤翔太
 * @data		2019/03/07
 */
#ifndef _ANIMATION_Editor_H_
#define _ANIMATION_Editor_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "SpriteMesh.h"

struct ANIMATION_Editor_DATA
{
	int min;
	int max;
};

class Editor;
class Receiver;
class AnimationEditor : public Object, public GUI
{
public:
	/* @brief	コンストラクタ		*/
	AnimationEditor(void);
	/* @brief	デストラクタ		*/
	virtual ~AnimationEditor(void);

	/* @brief	初期化処理			*/
	virtual void Init(void)		override;
	/* @brief	後処理				*/
	virtual void Uninit(void)	override;
	/* @brief	更新処理			*/
	virtual void Update(void)	override;

	virtual void GuiUpdate(void) override;

	void Load(void);

	void CreateAnimation(void);

	const ANIMATION_Editor_DATA& GetAnimationEditorData(void) { return beforeData_; }
	void SetAnimationEditorData(const ANIMATION_Editor_DATA& data) { beforeData_ = data; }

	void SetReceiver(Receiver* receiver)	{ receiver_ = receiver; }
	void SetEditor(Editor* editor)			{ editor_	= editor;	}
	void SetCtrl(Controller* ctrl)			{ ctrl_		= ctrl;		}

	inline void SetRange(int range, bool min)	{ (min) ? minFrame_ = range : maxFrame_ = range;	}
	inline int	GetRange(bool min)				{ return (min) ? minFrame_ : maxFrame_;				}

	inline int GetCurrentFrame(void) { return currentFrame_; }
	void SetCurrentFrame(int frame);

	void HierarchyView(void);

	void SaveData(void);

private:
	void LoadData(void);

	void ChangeRange(int& range, bool min);

	void GetChildrenAnim(SPRITE_MESH_ANIMATION& tempAnimation, Receiver& receiver);

	char			animationName_[256];
	int				currentFrame_;
	int				minFrame_;
	int				maxFrame_;
	bool			regeneration_;

	ANIMATION_Editor_DATA beforeData_;

	Receiver*		receiver_;
	Editor*			editor_;

	Controller*		ctrl_;
};

#endif // _ANIMATION_Editor_H_
