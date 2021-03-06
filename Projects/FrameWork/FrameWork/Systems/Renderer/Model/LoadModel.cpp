//-----------------------------------------------------------------------------
//
//	assimp[LoadAssimp.cpp]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "LoadModel.h"
#include "../../../Systems/GameSystems.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"

MESH_RESOURCE LoadModel::Load(string fileName)
{
	MESH_RESOURCE model;
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");

	fread(&model.transMtx, sizeof(MATRIX), 1, fp);

	uint size = 0;
	fread(&size, sizeof(size), 1, fp);
	for (uint i = 0; i < size; ++i)
	{
		GetMesh(fp, model);
	}

	const auto& bone = model.bone;
	size = (uint)bone.size();
	fread(&size, sizeof(size), 1, fp);
	for (uint i = 0; i < size; ++i)
	{
		GetBone(fp, model);
	}

	fclose(fp);
	return model;
}

HRESULT LoadModel::LoadAnimation(string fileName, MESH_RESOURCE& model)
{
	FILE* fp;
	fopen_s(&fp, fileName.c_str(), "rb");
	if (!fp) { return E_FAIL; }

	MESH_RESOURCE animModel;
	bool end = true;
	while (end)
	{
		BONE bone;
		uint size = 0;
		int n = (int)fread(&size, sizeof(size), 1, fp);
		if (n < 1) { end = false; }
		char buf[1028];
		ZeroMemory(buf, sizeof(char) * 1028);
		fread(buf, size, 1, fp);
		bone.name = buf;

		fread(&size, sizeof(size), 1, fp);
		std::vector<MATRIX> anim;
		for (uint i = 0; i < size; ++i)
		{
			MATRIX am;
			fread(&am, sizeof(MATRIX), 1, fp);
			anim.emplace_back(am);
		}
		bone.animMtx.emplace_back(anim);
		animModel.bone.emplace_back(bone);
	}

	for (auto& bone : model.bone)
	{
		for (const auto& anim : animModel.bone)
		{
			if (bone.name == anim.name)
			{
				if (anim.animMtx.size() > 0)
				{
					bone.animMtx.emplace_back(anim.animMtx[0]);
				}
			}
		}
	}

	fclose(fp);

	return S_OK;
}

void LoadModel::GetMesh(FILE* fp, MESH_RESOURCE& model)
{
	MESH tempMesh;
	uint size = 0;

	fread(&size, sizeof(size), 1, fp);
	char buf[1028];
	ZeroMemory(buf, sizeof(char) * 1028);
	fread(buf, size, 1, fp);
	tempMesh.name = buf;

	fread(&tempMesh.transMtx, sizeof(MATRIX), 1, fp);

	fread(&size, sizeof(size), 1, fp);
	for (uint i = 0; i < size; ++i)
	{
		VERTEX v;
		fread(&v, sizeof(VERTEX), 1, fp);
		tempMesh.vertex.emplace_back(v);
	}

	fread(&size, sizeof(size), 1, fp);
	for (uint i = 0; i < size; ++i)
	{
		uint temp;
		fread(&temp, sizeof(uint), 1, fp);
		tempMesh.index.emplace_back(static_cast<WORD>(temp));
	}

	fread(&tempMesh.material.ambient , sizeof(COLOR), 1, fp);
	fread(&tempMesh.material.diffuse , sizeof(COLOR), 1, fp);
	fread(&tempMesh.material.specular, sizeof(COLOR), 1, fp);
	fread(&tempMesh.material.emission, sizeof(COLOR), 1, fp);
	fread(&tempMesh.material.power   , sizeof(float), 1, fp);

	int texMax = static_cast<int>(MaterialType::MAX);
	for (int i = 0; i < texMax; ++i)
	{
		fread(&size, sizeof(size), 1, fp);
		ZeroMemory(buf, sizeof(char) * 1028);
		fread(buf, size, 1, fp);
		TempTextureResource* resource = new TempTextureResource;
		if (resource)
		{
			resource->SetName(buf);
			tempMesh.material.texture[i] = resource;
		}
	}

	model.mesh.emplace_back(tempMesh);
}

void LoadModel::GetBone(FILE* fp, MESH_RESOURCE& model)
{
	BONE tempBone;
	uint size = 0;

	fread(&size, sizeof(size), 1, fp);
	char buf[1028];
	ZeroMemory(buf, sizeof(char) * 1028);
	fread(buf, size, 1, fp);
	tempBone.name = buf;
	fread(&tempBone.offsetMtx, sizeof(MATRIX), 1, fp);
	tempBone.nowBone.Identity();

	model.bone.emplace_back(tempBone);
}
