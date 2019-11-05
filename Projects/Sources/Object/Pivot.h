/*
 * @file		Pivot.h
 * @brief		�s�{�b�g�̕`��
 * @author		���V�đ�
 * @data		2019/03/07
 */
#ifndef _PIVOT_H_
#define _PIVOT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Pivot : public Object
{
public:
	/* @brief	�R���X�g���N�^		*/
	Pivot(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~Pivot(void);

	/* @brief	����������			*/
	virtual void Init(void)		override;
	/* @brief	�㏈��				*/
	virtual void Uninit(void)	override;
	/* @brief	�X�V����			*/
	virtual void Update(void)	override;

	/* @brief	�g�����X�t�H�[���̐ݒ�	*/
	void SetTransform(const Transform& transform) 
	{
		transform_.parent = &transform;
		transform_.scale.x = 0.1f / transform.scale.x;
		transform_.scale.y = 0.1f / transform.scale.y;
		transform_.scale.z = 0.1f / transform.scale.z;
	}

private:
	//! �g�����X�t�H�[��
	Transform		transform_;
	//! �X�v���C�g
	SpriteRenderer* pivot_;
};

#endif // _PIVOT_H_
