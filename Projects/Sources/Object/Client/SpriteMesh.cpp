#include "SpriteMesh.h"

SpriteMesh::SpriteMesh(Systems* systems) : Interface(systems)
{
}

HRESULT SpriteMesh::Init(void)
{
	return S_OK;
}

void SpriteMesh::Uninit(void)
{
}

int SpriteMesh::SetUpLoading(Loading* loading, int sceneNum)
{
	UNREFERENCED_PARAMETER(loading);
	UNREFERENCED_PARAMETER(sceneNum);
	return 0;
}

HRESULT SpriteMesh::Load(int sceneNum)
{
	UNREFERENCED_PARAMETER(sceneNum);
	return S_OK;
}

void SpriteMesh::Release(bool uninit)
{
	UNREFERENCED_PARAMETER(uninit);
}
