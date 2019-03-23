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
	MeshResource Load(string fileName);
	HRESULT		 LoadAnimation(string fileName, MeshResource& model);

private:
	void GetMesh(FILE* fp, MeshResource& model);
	void GetBone(FILE* fp, MeshResource& model);
};

#endif // _LOAD_MODEL_H_
