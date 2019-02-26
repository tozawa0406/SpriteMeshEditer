#ifndef _I_COMMAND_H_
#define _I_COMMAND_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include <FrameWork/Systems/Input/Controller.h>

enum class CommandType : int8
{
	UNKOWN = -1,
	SET_POSITION,

	MAX
};

struct Receiver
{
	Transform*	transform;
	VECTOR2*	pivot;
};

class ICommand
{
public:
	/* @brief	コンストラクタ		*/
	ICommand(void) { receiver_.transform = nullptr; receiver_.pivot = nullptr; }
	/* @brief	デストラクタ		*/
	~ICommand(void) {}

	/* @brief	処理		*/
	virtual void Invoke(Receiver& beforeData)	= 0;
	/* @brief	進む		*/
	virtual void Undo(Receiver& beforeData)		= 0;
	/* @brief	戻る		*/
	virtual void Redo(Receiver& beforeData)		= 0;

	void SetReceiver(Receiver& receiver) { receiver_ = receiver; }

protected:
	Receiver	receiver_;
};

#endif // _I_COMMAND_H_
