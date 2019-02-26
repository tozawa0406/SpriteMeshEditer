#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "ICommand.h"

class Client;
class Receiver
{
public:
	/* @brief	�R���X�g���N�^		*/
	Receiver(void);
	/* @brief	�f�X�g���N�^		*/
	~Receiver(void);

	void Init(Client* client);
	void Uninit(void);
	void Update(void);

	const string& GetName(void) { return name_; }

	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

	const Transform& GetTransform(void) { return transform_; }

private:
	template<class T>
	bool InvokeCommand(void);

	void SaveData(void);

	string			name_;
	SpriteRenderer* spriteRenderer_;
	Transform		transform_;

	RECEIVER_DATA	receiver_;
	RECEIVER_DATA	beforeData_;

	Client*			client_;

	Controller*		ctrl_;
	LoadAddTexture* loadAdd_;
};

#endif // _RECEIVER_H_