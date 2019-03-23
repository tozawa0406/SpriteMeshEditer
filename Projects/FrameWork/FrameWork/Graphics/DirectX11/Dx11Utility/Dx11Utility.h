#ifndef _DX11_UTILITY_
#define _DX11_UTILITY_

#include "../../Utility/Utility.h"
#include "../Dx11Include.h"

/* @brief	DirectX11�̃f�o�C�X			*/
class D3D11Device : public IDevice
{
public:
	/* @brief	�R���X�g���N�^		*/
	D3D11Device(void) : device_(nullptr) {}
	/* @brief	�ł��g����			*/
	virtual ~D3D11Device(void) {}

	/* @brief	�������			*/
	void			Release(void);
	/* @brief	�f�o�C�X�擾����	*/
	ID3D11Device*	GetDevice(void) { return device_; }
	/* @brief	�f�o�C�X�̐ݒ菈��	*/
	void			SetDevice(ID3D11Device* device) { assert(device); device_ = device; }

	/* @brief	�e�N�X�`�����[�h����	*/
	virtual HRESULT Load(ITextureResource**, const string&) override;
	/* @brief	���f�����[�h����		*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name) override;
	/* @brief	���f���A�j���[�V�����̃��[�h����	*/
	virtual HRESULT Load(MESH_RESOURCE& meshResource, const string& name, bool anim) override;

	/* @brief	���_�o�b�t�@��������	*/
	virtual HRESULT CreateBuffer(IVertexBuffer**, const void*, uint, uint) override;
	/* @brief	�C���f�b�N�X�o�b�t�@�̐�������	*/
	virtual HRESULT CreateBuffer(IIndexBuffer** indexBuffer, const WORD* index, uint indexNum) override;

private:
	//! �f�o�C�X�{��
	ID3D11Device* device_;
};

/* @brief	���_�o�b�t�@				*/
class VertexBuffer : public IVertexBuffer
{
public:
	/* @brief	�R���X�g���N�^			*/
	virtual ~VertexBuffer(void) {}

	/* @brief	�o�b�t�@�擾����		*/
	inline  ID3D11Buffer*	GetBuffer(void) { return buffer_; }
	/* @brief	�������				*/
	virtual void			Release(void) override;

private:
	/* @brief	�R���X�g���N�^			*/
	VertexBuffer(void) : buffer_(nullptr) {}
	/* @brief	�o�b�t�@��������		*/
	virtual HRESULT Create(IDevice*, const void*, uint, uint) override;

	//! �o�b�t�@�{��
	ID3D11Buffer* buffer_;
	//! �@�\�����̂��߂̃t�����h�N���X
	friend D3D11Device;
};

/* @brief	�C���f�b�N�X�o�b�t�@		*/
class IndexBuffer : public IIndexBuffer
{
public:
	/* @brief	�f�X�g���N�^		*/
	virtual ~IndexBuffer(void) {}

	/* @brief	�o�b�t�@�擾����	*/
	inline  ID3D11Buffer*	GetBuffer(void) { return buffer_; }
	/* @brief	�������			*/
	virtual void			Release(void) override;

private:
	/* @brief	�R���X�g���N�^		*/
	IndexBuffer(void) {}
	/* @brief	�o�b�t�@��������	*/
	virtual HRESULT Create(IDevice*, const WORD*, uint) override;

	//! �o�b�t�@�{��
	ID3D11Buffer* buffer_;
	//! �@�\�����̂��߂̃t�����h�N���X
	friend D3D11Device;
};

/* @brief	�e�N�X�`�����\�[�X			*/
class TextureResource : public ITextureResource
{
public:
	/* @brief	�f�X�g���N�^			*/
	virtual ~TextureResource(void) {}

	/* @brief	�e�N�X�`���擾����		*/
	inline ID3D11ShaderResourceView*	GetResource(void) { return resource_; }
	/* @brief	�������				*/
	virtual void						Release(void) override;

private:
	/* @brief	�R���X�g���N�^			*/
	TextureResource(void) : resource_(nullptr) {}
	/* @brief	���[�h����				*/
	virtual HRESULT Load(IDevice*, const string&) override;

	//! �e�N�X�`���{��
	ID3D11ShaderResourceView* resource_;
	//! �@�\�����̂��߂̃t�����h�N���X
	friend D3D11Device;
};

#endif // !_DX11_UTILITY_
