#ifndef _LOAD_SPRITE_MESH_H_
#define _LOAD_SPRITE_MESH_H_

#include "SpriteMesh.h"
#include "../IOFile.h"

static const string version = "ver.1.0";

class LoadSpriteMesh
{
public:
	SPRITE_MESH_RESOURCE Load(string fileName);
	void Save(string fileName, const SPRITE_MESH_RESOURCE& resource);

private:
	void GetSpriteMeshVer1_1(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh);
	void SetSpriteMeshVer1_1(IOFile& file, const SPRITE_MESH_RESOURCE& spriteMesh);
};

#endif // _LOAD_SPRITE_MESH_H_
