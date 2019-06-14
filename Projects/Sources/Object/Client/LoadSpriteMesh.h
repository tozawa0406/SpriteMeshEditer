#ifndef _LOAD_SPRITE_MESH_H_
#define _LOAD_SPRITE_MESH_H_

#include "SpriteMesh.h"
#include "../IOFile.h"

static const string SPRITE_MESH_VARSION		= "ver.1.0";
static const string SPRITE_MESH_EXTENSION = "spx";
static const string SPRITE_MESH_ANIMATION_EXTENSION = "spax";

class LoadSpriteMesh
{
public:
	SPRITE_MESH_RESOURCE Load(string fileName, string& version);
	void Save(string fileName, const SPRITE_MESH_RESOURCE& resource);

	SPRITE_MESH_ANIMATION LoadAnimation(string fileName, string& version);
	void Save(string fileName, const SPRITE_MESH_ANIMATION& animation);

private:
	void GetSpriteMeshVer1_0(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh);
	void SetSpriteMeshVer1_0(IOFile& file, const SPRITE_MESH_RESOURCE& spriteMesh);

	void GetSpriteAnimationVer1_0(IOFile& file, SPRITE_MESH_ANIM_DATA& spriteAnimation);
	void SetSpriteAnimationVer1_0(IOFile& file, const SPRITE_MESH_ANIM_DATA& spriteAnimation);
};

#endif // _LOAD_SPRITE_MESH_H_
