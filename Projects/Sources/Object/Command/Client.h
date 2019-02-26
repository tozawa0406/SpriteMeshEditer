#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "ICommand.h"
#include <FrameWork/Object/Object.h>

class Pivot;
class Receiver;
class Client : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Client(void);
	/* @brief	デストラクタ		*/
	virtual ~Client(void);

	virtual void Init(void)		override;
	virtual void Uninit(void)	override;
	virtual void Update(void)	override;

	void InspectorView(void);
	void ConsoleView(void);
	void HierarchyView(void);

	void AddCommand(ICommand* command);
	void AddMessage(const string& message);

	inline void SetCtrl(Controller* ctrl)	{ ctrl_ = ctrl;		}
	inline void SetPivot(Pivot* pivot)		{ pivot_ = pivot;	}

private:
	void Undo(void);
	void Redo(void);

	void CreateReceiver(void);
	void SaveData(void);

	std::vector<ICommand*>	prevCommand_;
	std::vector<ICommand*>	nextCommand_;
	std::vector<string>		message_;

	Pivot*					pivot_;
	std::vector<Receiver*>	clientList_;
	Receiver*				currentReceiver_;

	Controller*		ctrl_;
};

#endif // _CLIENT_H_
