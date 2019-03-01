#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include "SpriteMesh.h"

class Pivot;
class Receiver;
class Client : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Client(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Client(void);

	/* @brief	����������			*/
	virtual void Init(void)		override;
	/* @brief	�㏈��				*/
	virtual void Uninit(void)	override;
	/* @brief	�X�V����			*/
	virtual void Update(void)	override;

	/* @brief	���[�h����			*/
	void Load(void);

	/* @brief	�C���X�y�N�^�̕`��	*/
	void InspectorView(void);
	/* @brief	�R���\�[���̕`��	*/
	void ConsoleView(void);
	/* @brief	�q�G�����L�[�̕`��	*/
	void HierarchyView(void);

	/* @brief	�R�}���h�̒ǉ�
	 * @param	(command)	�ǉ�����R�}���h	*/
	void AddCommand(ICommand* command);
	/* @brief	���b�Z�[�W�̒ǉ�
	 * @param	(message)	�ǉ����郁�b�Z�[�W	*/
	void AddMessage(const string& message);

	/* @brief	�R���g���[���̐ݒ�
	 * @param	(ctrl)	�R���g���[���̃|�C���^	*/
	inline void SetCtrl(Controller* ctrl)	{ ctrl_ = ctrl;		}
	/* @brief	�s�{�b�g�̐ݒ�
	 * @param	(pivot)	�s�{�b�g�̃|�C���^		*/
	inline void SetPivot(Pivot* pivot)		{ pivot_ = pivot;	}

	/* @brief	�q�G�����L�[�������
	 * @param	(receiver)	��������X�v���C�g	*/
	int  RemoveSprite(Receiver* receiver);
	/* @brief	�q�G�����L�[�ւ̒ǉ�
	 * @param	(receiver)	�V�K�X�v���C�g
	 * @param	(place)		�z��ԍ�(Undo��)	*/
	void AddSprite(Receiver* receiver, int place);

	/* @brief	�q�G�����L�[�̃��X�g���擾		*/
	const std::vector<Receiver*>& GetReceiverList(void) { return receiverList_; }

private:
	/* @brief	�߂鏈��
	 * @sa		Update(), HierarchyView()		*/
	void Undo(void);
	/* @brief	�i�ޏ���
	 * @sa		Update(), HierarchyView()		*/
	void Redo(void);

	/* @brief	�q�G�����L�[�r���[�ɕ`��
	 * @sa		HierarchyView(), DrawHierarchy()
	 * @param	(draw)		�`�悷�����
	 * @param	(blank)		��				*/
	void DrawHierarchy(Receiver* draw, string& blank);

	/* @brief	�X�v���C�g�̐���
	 * @sa		HierarchyView(), LoadData()
	 * @param	(resource)	���f�[�^(���[�h���͏����������ς��)	*/
	void CreateReceiver(SPRITE_MESH_RESOURCE* resource = nullptr);

	/* @brief	�Z�[�u		*/
	void SaveData(void);
	/* @brief	���[�h		*/
	void LoadData(void);

	//! �߂�R�}���h
	std::vector<ICommand*>	prevCommand_;
	//! �i�ރR�}���h
	std::vector<ICommand*>	nextCommand_;
	//! ���b�Z�[�W���X�g
	std::vector<string>		message_;

	//! �t�@�C����
	string					name_;
	//! �s�{�b�g
	Pivot*					pivot_;
	//! �_�u���N���b�N����
	int						selectedCnt_;
	//! ���[�N�X�y�[�X
	Receiver*				workReceiver_;
	//! �q�G�����L�[�r���[
	std::vector<Receiver*>	receiverList_;
	//! ���݂̃��[�N�X�y�[�X
	Receiver*				currentReceiver_;

	//! �R���g���[���̃|�C���^
	Controller*		ctrl_;
};

#endif // _CLIENT_H_
