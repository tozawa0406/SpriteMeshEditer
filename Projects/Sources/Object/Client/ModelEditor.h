/*
 * @file		ModelEditor.h
 * @brief		���f���G�f�B�^
 * @author		���V�đ�
 * @data		2019/03/07
 */
#ifndef _MODEL_EDITOR_H_
#define _MODEL_EDITOR_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>
#include "SpriteMesh.h"

class Pivot;
class Receiver;
class Editor;
class AnimationEditor;
class ModelEditor : public Object, public GUI
{
public:
	/* @brief	�R���X�g���N�^		*/
	ModelEditor(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~ModelEditor(void);

	/* @brief	����������			*/
	virtual void Init(void)		override;
	/* @brief	�㏈��				*/
	virtual void Uninit(void)	override;
	/* @brief	�X�V����			*/
	virtual void Update(void)	override;

	/* @brief	���[�h����			*/
	void Load(void);

	/* @brief	Gui�X�V����			*/
	virtual void GuiUpdate(void) override;

	/* @brief	�R���g���[���̐ݒ�
	 * @param	(ctrl)	�R���g���[���̃|�C���^	*/
	inline void SetCtrl(Controller* ctrl)	{ ctrl_ = ctrl;		}
	/* @brief	�s�{�b�g�̐ݒ�
	 * @param	(pivot)	�s�{�b�g�̃|�C���^		*/
	inline void SetPivot(Pivot* pivot)		{ pivot_ = pivot;	}

	/* @brief	�q�G�����L�[�������
	 * @param	(receiver)	��������X�v���C�g	*/
	void RemoveSprite(Receiver* receiver);
	/* @brief	�q�G�����L�[�ւ̒ǉ�
	 * @param	(receiver)	�V�K�X�v���C�g		*/
	void AddSprite(Receiver* receiver);

	/* @brief	�q�G�����L�[�̃��X�g���擾		*/
	const std::vector<Receiver*>& GetReceiverList(void) { return receiverList_; }

	/* @brief	�Z�[�u		*/
	void SaveData(void);

	inline void SetEditor(Editor* editor) { editor_ = editor; }
	inline Editor* GetEditor(void) { return editor_; }

private:
	/* @brief	�C���X�y�N�^�̕`��	*/
	void InspectorView(void);
	/* @brief	�R���\�[���̕`��	*/
	void ConsoleView(void);
	/* @brief	�q�G�����L�[�̕`��	*/
	void HierarchyView(void);

	/* @brief	�q�G�����L�[�r���[�ɕ`��
	 * @sa		HierarchyView(), DrawHierarchy()
	 * @param	(draw)		�`�悷�����
	 * @param	(blank)		��				*/
	void DrawHierarchy(Receiver* draw, string& blank);

	/* @brief	�X�v���C�g�̐���
	 * @sa		HierarchyView(), LoadData()
	 * @param	(resource)	���f�[�^(���[�h���͏����������ς��)	*/
	void CreateReceiver(SPRITE_MESH_RESOURCE* resource = nullptr);

	/* @brief	���[�h		*/
	void LoadData(void);

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

	//! �t���O
	uint8					flag_;
	//! �A�j���[�V�����t���O
	static constexpr uint8 FLAG_ANIMATION = 0x01;

	//! �R���g���[���̃|�C���^
	Controller*		ctrl_;
	Editor*			editor_;
	//! �A�j���[�V����
	AnimationEditor* animation_;
};

#endif // _MODEL_EDITOR_H_
