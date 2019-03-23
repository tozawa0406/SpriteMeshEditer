#ifndef _UTILTTY_
#define _UTILTTY_

#include "../../Define/Define.h"

/* @brief	�S�Ă̊��N���X
 * @detail	�����̃|�C���^�������ARelease���Ă�Ń��������������	*/
class IBaseUnknown
{
public:
	/* @brief	�R���X�g���N�^	*/
	IBaseUnknown(void) : pointer_(this) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~IBaseUnknown(void) {}

	/* @brief	�������(�h����ŕK���ĂԂ���)	*/
	virtual void Release(void) { DeletePtr(pointer_); }

private:
	//! �������g�̃|�C���^
	IBaseUnknown* pointer_;
};

class IDevice;

/* @brief	���_�o�b�t�@	*/
class IVertexBuffer : public IBaseUnknown
{
public:
	/* @brief	�R���X�g���N�^	*/
	IVertexBuffer(void) : size_(0) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~IVertexBuffer(void) {}

	/* @brief	1���_�̃o�C�g���擾����		*/
	inline uint GetSize(void) { return size_; }

protected:
	/* @brief	��������
	 * @sa		IDevice����Ăяo��			*/
	virtual HRESULT Create(IDevice*, const void*, uint, uint) = 0;

	//! 1���_�̃o�C�g��
	uint size_;
};

/* @brief	�C���f�b�N�X�o�b�t�@	*/
class IIndexBuffer : public IBaseUnknown
{
public:
	/* @brief	�R���X�g���N�^	*/
	IIndexBuffer(void) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~IIndexBuffer(void) {}

protected:
	/* @brief	��������
	 * @sa		IDevice����Ăяo��			*/
	virtual HRESULT Create(IDevice*, const WORD*, uint) = 0;
};

/* @brief	���\�[�X���N���X		*/
class IResourece : public IBaseUnknown
{
public:
	/* @brief	�R���X�g���N�^	*/
	IResourece(void) : name_("") {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~IResourece(void) {}

	/* @brief	���\�[�X���擾����	*/
	inline const string& GetName(void) { return name_; }

protected:
	/* @brief	���[�h����
	 * @sa		IDevice				*/
	virtual HRESULT Load(IDevice*, const string&) = 0;

	//! ���\�[�X��
	string		name_;
};

/* @brief	�e�N�X�`�����\�[�X		*/
class ITextureResource : public IResourece
{
public:
	/* @brief	�R���X�g���N�^	*/
	ITextureResource(void) : size_(VECTOR2(0)) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~ITextureResource(void) {}

	/* @brief	�傫���擾����		*/
	inline const VECTOR2& GetSize(void) { return size_; }

protected:
	//! �傫��
	VECTOR2 size_;
};

class TempTextureResource : public ITextureResource
{
public:
	TempTextureResource(void) {}
	virtual ~TempTextureResource(void) {}

	virtual void Release(void) override { IBaseUnknown::Release(); }
	const void SetName(const string& name) { name_ = name; }

private:
	// ��֐����������Ď��Ԃ����Ă�悤��
	virtual HRESULT Load(IDevice*, const string&) override { return S_OK; }
};

struct MESH_RESOURCE;
/* @brief	�f�o�C�X
 * @detail	���\�[�X�̐������s��	*/
class IDevice : public IBaseUnknown
{
public:
	/* @brief	�R���X�g���N�^	*/
	IDevice(void) {}
	/* @brief	�f�X�g���N�^	*/
	virtual ~IDevice(void) {}

	/* @brief	�e�N�X�`�����[�h����	*/
	virtual HRESULT Load(ITextureResource** textureResource, const string& name) = 0;
	/* @brief	���f�����[�h����		*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name) = 0;
	/* @brief	���f���A�j���[�V�����̃��[�h����	*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name, bool anim) = 0;

	/* @brief	���_�o�b�t�@��������	*/
	virtual HRESULT CreateBuffer(IVertexBuffer** vertexBuffer, const void* vertex, uint size, uint vertexNum) = 0;
	/* @brief	�C���f�b�N�X�o�b�t�@�̐�������	*/
	virtual HRESULT CreateBuffer(IIndexBuffer** indexBuffer, const WORD* index, uint indexNum) = 0;
};

#endif // !_UTILITY_
