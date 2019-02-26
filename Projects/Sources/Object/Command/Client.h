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

	const string& GetName(void) { return name_; }

	void ConsoleWindow(void);

	void Undo(void);
	void Redo(void);

	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

	const Transform& GetTransform(void) { return transform_; }

private:
	template<class T>
	bool InvokeCommand(void);

	void SaveData(void);

	void AddMessage(const string& mesaage);

	string			name_;
	SpriteRenderer* spriteRenderer_;
	Transform		transform_;

	Receiver		receiver_;
	Receiver		beforeData_;

	std::vector<ICommand*>	prevCommand_;
	std::vector<ICommand*>	nextCommand_;
	std::vector<string>		message_;

	Controller*		ctrl_;
	LoadAddTexture* loadAdd_;
};

#endif // _CLIENT_H_
