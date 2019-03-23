//-----------------------------------------------------------------------------
//
//	ÉÇÉfÉãä«óù[Model.h]
//	Auther : åÀ‡V„ƒëæ
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"
#include "../../../Graphics/Utility/Utility.h"

enum class MaterialType : uint8
{
	Diffuse = 0,
	Normal,

	MAX
};

struct MATERIAL
{
	COLOR		ambient;
	COLOR		diffuse;
	COLOR		specular;
	COLOR		emission;
	float		power;
	ITextureResource* texture[static_cast<int>(MaterialType::MAX)];
};

struct BONE
{
	string	name;
	MATRIX	offsetMtx;
	MATRIX  inverseMtx;
	std::vector<std::vector<MATRIX>> animMtx;
	MATRIX  nowBone;
};

struct MESH
{
	string						name;
	MATRIX						transMtx;
	std::vector<VERTEX>			vertex;
	std::vector<WORD>			index;
	IVertexBuffer*				vertexBuffer;
	IIndexBuffer*				indexBuffer;
	uint						computeShader;
	MATERIAL					material;
};

struct MESH_RESOURCE
{
	MATRIX				transMtx;
	std::vector<MESH>	mesh;
	std::vector<BONE>	bone;
};

class Loading;
class Model : public Interface
{
	friend class Systems;
public:
	~Model(void) {}
	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);

	MESH_RESOURCE* GetMesh(int modelNum) { if (modelNum < (int)mesh_.size()) { return &mesh_[modelNum]; } return nullptr; }

private:
	Model(Systems* systems);
	HRESULT Init(void)   override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;
	std::vector<MESH_RESOURCE> mesh_;
};

#endif // _X_MODEL_H_
