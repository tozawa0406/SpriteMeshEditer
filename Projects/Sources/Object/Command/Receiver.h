#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "ICommand.h"
#include "../IOFile.h"

class Client;
class Receiver
{
public:
	/* @brief	コンストラクタ		*/
	Receiver(void);
	/* @brief	デストラクタ		*/
	~Receiver(void);

	void Init(Client* client);
	void Uninit(void);
	void Update(void);

	const string&			GetName(void)			{ return name_;				}
	const Transform&		GetTransform(void)		{ return transform_;		}
	const SpriteRenderer*	GetSpriteRenderer(void) { return spriteRenderer_;	}

	const RECEIVER_DATA& GetBeforeData(void) { return beforeData_; }

	void SetName(const string& name)				{ name_ = name;				}
	void SetTransform(const Transform& transform)	{ transform_ = transform;	}

	void SetBeforeData(const RECEIVER_DATA& beforeData) { beforeData_ = beforeData; }

	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

	void SaveData(IOFile& file);
	void LoadData(IOFile& file);

private:
	template<class T>
	bool InvokeCommand(void);

	string			name_;
	SpriteRenderer* spriteRenderer_;
	Transform		transform_;

	RECEIVER_DATA	beforeData_;

	Client*			client_;

	Controller*		ctrl_;
	LoadAddTexture* loadAdd_;
};

#endif // _RECEIVER_H_
