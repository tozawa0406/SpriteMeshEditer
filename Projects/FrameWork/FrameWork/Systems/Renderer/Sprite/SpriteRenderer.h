/*
 * @file	SpriteRenderer.h
 * @brief	�|���`��
 * @author	���V�đ�
 * @date	2019/02/26
 */
#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "../ObjectRenderer.h"

class SpriteRenderer : public ObjectRenderer
{
public:
	/* @brief	�R���X�g���N�^		*/
	SpriteRenderer(void);
	/* @brief	�f�X�g���N�^		*/
	virtual ~SpriteRenderer(void);

	/* @brief	����������
	 * @param	(texNum)	�e�N�X�`���ԍ�
	 * @param	(transform)	�Ή�������g�����X�t�H�[��		*/
	void	Init(int texNum, const Transform* transform);

	//void	SetVertex(uint n, uint vnum)  { vertexBuffer = n; vertexNum = vnum; }
	//void	SetIndex(uint n, uint vnum)   { indexBuffer = n; indexNum = vnum;   }

	/* @brief	�e�N�X�`���̐ݒ�		*/
	inline void SetTexture(int texNum)			{ texNum_ = texNum; }
	/* @brief	�������̐ݒ�			*/
	inline void SetSplit(const VECTOR2& split)	{ split_ = split;	}
	/* @brief	�s�{�b�g�̐ݒ�			*/
	inline void SetPivot(const VECTOR2& pivot)	{ pivot_ = pivot;	}
	/* @brief	�r���{�[�h�̐ݒ�		*/
	inline void SetBillboard(bool billboard)	{ BitSetFlag(billboard, flagBillboard_, FLAG_BILLBOARD);	}
	/* @brief	X���̉�]�̂Ȃ��r���{�[�h�̐ݒ�		*/
	inline void SetXBillboard(bool xbillboard)	{ BitSetFlag(xbillboard, flagBillboard_, FLAG_X_BILLBOARD); }

	/* @brief	�e�N�X�`���̎擾		*/
	inline int				GetTexture(void)	const { return texNum_;		}
	/* @brief	�A�j���[�V�����p�^�[���̎擾		*/
	inline float			GetPattern(void)	const { return pattern_;	}
	/* @brief	�������̎擾			*/
	inline const VECTOR2&	GetSplit(void)		const { return split_;		}
	/* @brief	�s�{�b�g�̎擾			*/
	inline const VECTOR2&	GetPivot(void)		const { return pivot_;		}
	/* @ brief	�r���{�[�h��Ԃ̎擾	*/
	inline bool				IsBillboard(void)	const { return BitCheck(flagBillboard_, FLAG_BILLBOARD);	}
	/* @brief	X����]�̂Ȃ��r���{�[�h�̐ݒ�		*/
	inline bool				IsXBillboard(void)	const { return BitCheck(flagBillboard_, FLAG_X_BILLBOARD);	}

	/* @brief	���_�o�b�t�@�̎擾		*/
	uint	GetVertexBuffer(void)	const { return vertexBuffer_;	}
	/* @brief	�C���f�b�N�X�o�b�t�@�̎擾		*/
	uint	GetIndexBuffer(void)	const { return indexBuffer_;	}
	/* @brief	���_���̎擾			*/
	uint	GetVertexNum(void)		const { return vertexNum_;		}
	/* @brief	�C���f�b�N�X���̎擾	*/
	uint	GetIndexNum(void)		const { return indexNum_;		}
	/* @breif	�e�N�X�`�����W�̎擾	*/
	VECTOR4 GetTexcoord(void)		const { return texcoord_;		}

	/* @brief	�A�j���[�V��������
	 * @param	(add)	�A�j���[�V�������x		*/
	virtual bool Animation(float add) override;

private:
	static constexpr uint8 FLAG_BILLBOARD	= 0x01;
	static constexpr uint8 FLAG_X_BILLBOARD = 0x02;

	//! �e�N�X�`��
	int			texNum_;

	//! �A�j���[�V�����p�^�[��
	float		pattern_;
	//! ������
	VECTOR2		split_;
	//! �s�{�b�g
	VECTOR2		pivot_;

	//! �r���{�[�h�t���O
	uint8		flagBillboard_;

	//! ���_�o�b�t�@
	uint	vertexBuffer_;
	//! �C���f�b�N�X�o�b�t�@
	uint	indexBuffer_;
	//! ���_��
	uint	vertexNum_;
	//! �C���f�b�N�X��
	uint	indexNum_;
	//! �e�N�X�`�����W
	VECTOR4 texcoord_;
};

#endif // _SPRITE_RENDERER_H_