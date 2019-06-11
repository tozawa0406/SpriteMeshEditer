/*
 * @file		Receiver.h
 * @brief		���V�[�o�[
 * @author		���V�đ�
 * @data		2019/03/07
 */
#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "Command/ICommand.h"
#include "SpriteMesh.h"

class ModelEditer;
class Receiver
{
	//! �t���O
	static constexpr uint8 FLAG_IS_HIERARCHY_CHILD	= 0x01;
	static constexpr uint8 FLAG_DELETE				= 0x02;
	static constexpr uint8 FLAG_WITH_CHILD			= 0x04;
	static constexpr uint8 FLAG_IS_HERARCHY			= 0x08;

public:
	/* @brief	�R���X�g���N�^		*/
	Receiver(void);
	/* @brief	�f�X�g���N�^		*/
	~Receiver(void);

	/* @brief	����������
	 * @param	(client)	�N���C�A���g	*/
	void Init(ModelEditer* client);
	/* @brief	�㏈��				*/
	void Uninit(void);
	/* @brief	�X�V����			*/
	void Update(void);

	/* @brief	���O�̎擾			*/
	const string&			GetName(void)			{ return name_;				}
	/* @brief	�p���̎擾			*/
	const Transform&		GetTransform(void)		{ return transform_;		}
	/* @breif	�����_���̎擾		*/
	const SpriteRenderer*	GetSpriteRenderer(void) { return spriteRenderer_;	}
	/* @brief	�O�̃f�[�^�̎擾	*/
	const RECEIVER_DATA&	GetBeforeData(void)		{ return beforeData_;		}

	/* @brief	���O�̐ݒ�			*/
	void SetName(const string& name)					{ name_ = name;				}
	/* @brief	�p���̐ݒ�			*/
	void SetTransform(const Transform& transform)		{ transform_ = transform;	}
	/* @breif	�O�̃f�[�^�̐ݒ�	*/
	void SetBeforeData(const RECEIVER_DATA& beforeData) { beforeData_ = beforeData; }
	/* @brief	�R���g���[���̐ݒ�	*/
	void SetCtrl(Controller* ctrl)						{ ctrl_ = ctrl;				}

	/* @breif	�g�p��Ԃ̐ݒ�		*/
	void SetEnable(bool enable);

	/* @breif	�Z�[�u				*/
	void SaveData(SPRITE_MESH_RESOURCE& resource);
	/* @breif	���[�h				*/
	bool LoadData(SPRITE_MESH_RESOURCE& resource);

	/* @brief	�e�̐ݒ�			*/
	void		SetParent(Receiver* parent);
	/* @brief	�e�̎擾			*/
	Receiver*	GetParent(void) { return parent_; }

	/* @brief	�q�̎擾			*/
	inline std::vector<Receiver*> GetChild(void) { return child_; }

	/* @brief	�q�G�����L�[��Ԃ̎擾		*/
	bool IsHierarchy(void)				{ return BitCheck(flag_, FLAG_IS_HERARCHY);			}
	/* @brief	�q�G�����L�[��Ԃ̐ݒ�		*/
	void SetHierarchy(bool hierarchy)	{ BitSetFlag(hierarchy, flag_, FLAG_IS_HERARCHY);	}

	/* @brief	�q�G�����L�[�q�v�f��Ԃ̎擾		*/
	bool IsHierarchChild(void)			{ return BitCheck(flag_, FLAG_IS_HIERARCHY_CHILD);	}
	/* @brief	�q�G�����L�[�q�v�f��Ԃ̐ݒ�		*/
	void SetHierarchyChild(bool is)		{ BitSetFlag(is, flag_, FLAG_IS_HIERARCHY_CHILD);	}

	/* @brief	���O�ɉe�����󂯂��f���[�g�R�}���h�̐ݒ�	*/
	void		SetPrevDelete(ICommand* command)	{ prevDelete_ = command;	}
	/* @brief	���O�ɉe�����󂯂��f���[�g�R�}���h�̎擾	*/
	ICommand*	GetPrevCommand(void)				{ return prevDelete_;		}

	void Animation(int frame);

	void AddAnim(int frame);
	void AddAnim(int frame, const SPRITE_MESH_ANIMATION& anim, int animNum);
	void RemoveAnim(int frame);

	void ResetAnimData(void) { anim_.clear(); for (auto& c : child_) { if (c) { c->ResetAnimData(); } } }

	void CreateAnimation(SPRITE_MESH_ANIMATION& animation);
	const std::vector<SPRITE_MESH_ANIM_DATA>& GetAnimData(void) { return anim_; }

	const std::vector<SPRITE_MESH_ANIMATION>& GetAnimation(void) { return animation_; }

private:
	/* @brief	���x�Ăяo������			*/
	template<class T>
	bool InvokeCommand(void);

	/* @brief	�q�v�f�̐ݒ�
	 * @param	(child)		�ݒ肷��q
	 * @param	(add)		�ǉ����폜��	*/
	void SetChild(Receiver* child, bool add);
	/* @brief	�C���X�y�N�^�ł�����l		*/
	void SelectParam(void);
	/* @brief	�폜����					*/
	void Delete(void);
	/* @brief	�f���[�g�R�}���h�̌Ăяo��
	 * @param	(withChild)		�q�v�f�����ɉʂĂ邩		*/
	void InvokeDeleteCommand(bool withChild);

	//! ���O
	string			name_;
	//! �����_��
	SpriteRenderer* spriteRenderer_;
	//! �p��
	Transform		transform_;

	//! �O�̃f�[�^
	RECEIVER_DATA	beforeData_;
	//! ���O�ɉe�����󂯂��̃f���[�g�R�}���h
	ICommand*		prevDelete_;
	//! �e�N�X�`�����O
	string			textureName_;

	//! �N���C�A���g
	ModelEditer*	client_;

	//! �R���g���[��
	Controller*		ctrl_;
	//! �ǉ����[�h
	LoadAddTexture* loadAdd_;
	
	//! �e
	Receiver*		parent_;
	//! �q
	std::vector<Receiver*> child_;

	//! ��ԃt���O
	uint8 flag_;

	//! �A�j���[�V����
	std::vector<SPRITE_MESH_ANIM_DATA> anim_;

	std::vector<SPRITE_MESH_ANIMATION> animation_;

	int animCnt_;
};

#endif // _RECEIVER_H_
