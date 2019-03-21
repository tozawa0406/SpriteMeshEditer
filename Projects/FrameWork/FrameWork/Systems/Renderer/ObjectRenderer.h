/*
 * @file	ObjectRenderer.h
 * @brief	3D�I�u�W�F�N�g�`��̊��N���X
 * @author	���V�đ�
 * @date	2019/02/25
 */
#ifndef _OBJECT_RENDERER_H_
#define _OBJECT_RENDERER_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"
#include "Sprite/Texture.h"
#include "Model/Model.h"
#include "Shader/ShaderManager.h"
#include "../../Graphics/Utility/Utility.h"

class ObjectRendererManager;
class ObjectRenderer
{
public:
	enum RendererType
	{
		SPRITE = 0,
		MODEL,
	};

	/* @brief	�R���X�g���N�^		*/
	ObjectRenderer(RendererType type);
	/* @brief	�f�X�g���N�^		*/
	virtual ~ObjectRenderer(void);

	/* @brief	�A�j���[�V����		*/
	virtual bool Animation(float add) = 0;

	/* @brief	�g�����X�t�H�[���̎擾		*/
	const Transform*	GetTransform(void)	const { return transform_; }

	/* @brief	�V�F�[�_�[�̐ݒ�	*/
	inline void SetShader(Shader::ENUM shader)			{ shader_ = shader;		}
	/* @brief	�}�e���A���̐ݒ�	*/
	inline void SetMaterial(const MATERIAL& material)	{ material_ = material; }
	/* @brief	�g�p��Ԃ̐ݒ�		*/
	inline void SetEnable(bool enable)		{ BitSetFlag(enable, flag_, FLAG_ENABLE);		}
	/* @brief	��ɕ`�悷��ݒ�	*/
	inline void SetFastDarw(bool fastDraw)	{ BitSetFlag(fastDraw, flag_, FLAG_FAST_DRAW);	}
	/* @brief	�e�̗L���̐ݒ�		*/
	inline void SetShadow(bool shadow)		{ BitSetFlag(shadow, flag_, FLAG_SHADOW);		}
	/* @brief	�\�[�g�L���̐ݒ�	*/
	inline void SetSort(bool sort)			{ BitSetFlag(sort, flag_, FLAG_SORT);			}

	/* @brief	�g�p����V�F�[�_�[�̎擾	*/
	inline Shader::ENUM		GetShader(void)		const { return shader_;		}
	/* @brief	�}�e���A���̎擾	*/
	inline const MATERIAL&	GetMaterial(void)	const { return material_;	}
	/* @brief	�g�p��Ԃ̎擾		*/
	inline bool	IsEnable(void)		const { return BitCheck(flag_, FLAG_ENABLE); }
	/* @brief	��ɕ`�悷�邩		*/
	inline bool	IsFastDraw(void)	const { return BitCheck(flag_, FLAG_FAST_DRAW); }
	/* @brief	�e�̗L��			*/
	inline bool	IsShadow(void)		const { return BitCheck(flag_, FLAG_SHADOW); }
	/* @brief	�\�[�g���邩		*/
	inline bool	IsSort(void)		const { return BitCheck(flag_, FLAG_SORT); }

	inline RendererType GetType(void) const { return type_; }

protected:
	/* @brief	������
	 * @param	(manager)	�}�l�[�W���[
	 * @param	(transform)	�Ή�����h�����X�t�H�[���̃|�C���^		*/
	void Init(ObjectRendererManager* manager, const Transform* transform);

	//! �}�l�[�W���[
	ObjectRendererManager*	manager_;
	Wrapper*				wrapper_;
	//! �g�����X�t�H�[��
	const Transform*		transform_;

	//! �}�e���A��
	MATERIAL		material_;
	//! �g�p����V�F�[�_�[
	Shader::ENUM	shader_;

private:
	static constexpr uint8 FLAG_ENABLE		= 0x01;
	static constexpr uint8 FLAG_FAST_DRAW	= 0x02;
	static constexpr uint8 FLAG_SHADOW		= 0x04;
	static constexpr uint8 FLAG_SORT		= 0x08;

	//! ��Ԃ������t���O
	uint8			flag_;
	//! �����_���̎��
	RendererType	type_;
};

#endif // _CANVAS_RENDERER_H_