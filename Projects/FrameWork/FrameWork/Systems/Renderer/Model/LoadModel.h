//-----------------------------------------------------------------------------
//
//	assimp[LoadAssimp.h]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _LOAD_MODEL_H_
#define _LOAD_MODEL_H_

#include "../../../Define/Define.h"
#include "Model.h"

class LoadModel
{
public:
	MESH_RESOURCE	Load(string fileName);
	HRESULT			LoadAnimation(string fileName, MESH_RESOURCE& model);

private:
	void GetMesh(FILE* fp, MESH_RESOURCE& model);
	void GetBone(FILE* fp, MESH_RESOURCE& model);
};

#endif // _LOAD_MODEL_H_
