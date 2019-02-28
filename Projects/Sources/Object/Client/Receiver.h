#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "Command/ICommand.h"
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

	void SaveData(IOFile& file, bool parentCall);
	bool LoadData(IOFile& file, bool parentCall);

	void SetParent(Receiver* parent);
	Receiver* GetParent(void) { return parent_; }

	inline std::vector<Receiver*> GetChild(void) { return child_; }

	bool IsHierarchy(void)				{ return isHierarchy_;		}
	void SetHierarchy(bool hierarchy)	{ isHierarchy_ = hierarchy; }

private:
	template<class T>
	bool InvokeCommand(void);

	void SetChild(Receiver* child, bool add);
	void SelectParam(void);
	void Delete(void);

	string			name_;
	SpriteRenderer* spriteRenderer_;
	Transform		transform_;

	RECEIVER_DATA	beforeData_;
	string			textureName_;

	Client*			client_;

	Controller*		ctrl_;
	LoadAddTexture* loadAdd_;
	
	Receiver*		parent_;
	std::vector<Receiver*> child_;

	bool delete_;
	bool isHierarchy_;
};

#endif // _RECEIVER_H_
