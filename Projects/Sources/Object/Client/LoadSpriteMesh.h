#ifndef _LOAD_SPRITE_MESH_H_
#define _LOAD_SPRITE_MESH_H_

#include "SpriteMesh.h"
#include "../IOFile.h"

static const string SPRITE_MESH_VARSION		= "ver.1.0";
static const string SPRITE_MESH_EXTENSION	= "spx";

class LoadSpriteMesh
{
public:
	SPRITE_MESH_RESOURCE Load(string fileName, string& version);
	void Save(string fileName, const SPRITE_MESH_RESOURCE& resource);

private:
	void GetSpriteMeshVer1_0(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh);
	void SetSpriteMeshVer1_0(IOFile& file, const SPRITE_MESH_RESOURCE& spriteMesh);
};

#endif // _LOAD_SPRITE_MESH_H_
