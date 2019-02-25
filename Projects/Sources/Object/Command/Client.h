#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "ICommand.h"

class Client
{
public:
	/* @brief	コンストラクタ		*/
	Client(void);
	/* @brief	デストラクタ		*/
	~Client(void);

	void Init(void);
	void Uninit(void);
	void Update(void);

	void ConsoleWindow(void);

	void Undo(void);
	void Redo(void);

	void SetReciver(Transform* transform) { reciver_ = transform; }
	void SetReciver2(SpriteRenderer* spriteRenderer) { reciver2_ = spriteRenderer; }
	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

private:
	void SetNewCommand(ICommand* command);
	void SaveData(void);

	void AddMessage(const string& mesaage);

	Transform*		reciver_;
	SpriteRenderer* reciver2_;
	Transform	beforeData_;
	VECTOR2		beforeData2_;

	std::vector<ICommand*> prevCommand_;
	std::vector<ICommand*> nextCommand_;

	Controller* ctrl_;

	std::vector<string> message_;
};

#endif // _CLIENT_H_
