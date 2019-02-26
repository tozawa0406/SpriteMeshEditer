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

	void SetReceiver(SpriteRenderer* spriteRenderer);
	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

private:
	template<class T>
	bool InvokeCommand(void);

	void SaveData(void);

	void AddMessage(const string& mesaage);

	Receiver		receiver_;
	Receiver		beforeData_;

	std::vector<ICommand*> prevCommand_;
	std::vector<ICommand*> nextCommand_;

	Controller* ctrl_;

	std::vector<string> message_;
};

#endif // _CLIENT_H_
