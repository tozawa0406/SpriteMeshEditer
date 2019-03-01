#ifndef _LOAD_SPRITE_MESH_H_
#define _LOAD_SPRITE_MESH_H_

#include "SpriteMesh.h"
#include "../IOFile.h"

static const string version = "ver.1.0";

class LoadSpriteMesh
{
public:
	SPRITE_MESH_RESOURCE Load(string fileName);
	void Save(string fileName, SPRITE_MESH_RESOURCE resource);

private:
	void GetSpriteMesh(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh, bool parent);

	int readNum_;
};

#endif // _LOAD_SPRITE_MESH_H_
