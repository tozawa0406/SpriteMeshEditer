#include "LoadSpriteMesh.h"

SPRITE_MESH_RESOURCE LoadSpriteMesh::Load(string fileName, string& version)
{
	SPRITE_MESH_RESOURCE temp;

	// ì«Ç›çûÇ›
	IOFile file;
	if (file.OpenFile(fileName, std::ios::in))
	{
		string ver;
		size_t size = 0;
		file.ReadParam(&size, sizeof(size_t));
		ver.resize(size);
		file.ReadParam(&ver[0], sizeof(char) * size);

		version = ver;
		// ÇªÇÍÇºÇÍÇÃì«çû
		if (ver == "ver.1.0")
		{
			GetSpriteMeshVer1_0(file, temp);
		}

		// èIóπ
		file.CloseFile();
	}

	return temp;
}

void LoadSpriteMesh::GetSpriteMeshVer1_0(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh)
{	
	size_t size = 0;
	file.ReadParam(&size, sizeof(size_t));
	spriteMesh.name.resize(size);
	file.ReadParam(&spriteMesh.name[0], sizeof(char) * size);
	file.ReadParam(&spriteMesh.transform.position, sizeof(VECTOR3));
	file.ReadParam(&spriteMesh.transform.rotation, sizeof(VECTOR3));
	file.ReadParam(&spriteMesh.transform.scale, sizeof(VECTOR3));
	file.ReadParam(&spriteMesh.pivot, sizeof(VECTOR2));
	file.ReadParam(&spriteMesh.layer, sizeof(uint8));
	size = 0;
	file.ReadParam(&size, sizeof(size_t));
	spriteMesh.textureName.resize(size);
	file.ReadParam(&spriteMesh.textureName[0], sizeof(char) * size);

	size = 0;
	file.ReadParam(&size, sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
	{
		SPRITE_MESH_RESOURCE temp;
		GetSpriteMeshVer1_0(file, temp);
		spriteMesh.children.emplace_back(temp);
	}
}

void LoadSpriteMesh::Save(string fileName, const SPRITE_MESH_RESOURCE& resource)
{
	// ì«Ç›çûÇ›
	IOFile file;
	if (file.OpenFile(fileName, std::ios::out))
	{
		size_t size = SPRITE_MESH_VARSION.size();
		file.WriteParam(&size, sizeof(size_t));
		file.WriteParam(&SPRITE_MESH_VARSION[0], sizeof(char) * size);

		SetSpriteMeshVer1_0(file, resource);

		file.CloseFile();
	}
}

void LoadSpriteMesh::SetSpriteMeshVer1_0(IOFile& file, const SPRITE_MESH_RESOURCE& spriteMesh)
{
	size_t size = spriteMesh.name.size();
	file.WriteParam(&size, sizeof(size_t));
	file.WriteParam(&spriteMesh.name[0], sizeof(char) * size);
	file.WriteParam(&spriteMesh.transform.position, sizeof(VECTOR3));
	file.WriteParam(&spriteMesh.transform.rotation, sizeof(VECTOR3));
	file.WriteParam(&spriteMesh.transform.scale, sizeof(VECTOR3));
	file.WriteParam(&spriteMesh.pivot, sizeof(VECTOR2));
	file.WriteParam(&spriteMesh.layer, sizeof(uint8));
	size = spriteMesh.textureName.size();
	file.WriteParam(&size, sizeof(size_t));
	file.WriteParam(&spriteMesh.textureName[0], sizeof(char) * size);

	size = spriteMesh.children.size();
	file.WriteParam(&size, sizeof(size_t));
	for (size_t i = 0; i < size; ++i)
	{
		SetSpriteMeshVer1_0(file, spriteMesh.children[i]);
	}
}

SPRITE_MESH_ANIMATION LoadSpriteMesh::LoadAnimation(string fileName, string& version)
{
	SPRITE_MESH_ANIMATION temp;

	// ì«Ç›çûÇ›
	IOFile file;
	if (file.OpenFile(fileName, std::ios::in))
	{
		string ver;
		size_t size = 0;
		file.ReadParam(&size, sizeof(size_t));
		ver.resize(size);
		file.ReadParam(&ver[0], sizeof(char) * size);

		file.ReadParam(&size, sizeof(size_t));
		temp.animationName.resize(size);
		file.ReadParam(&temp.animationName[0], sizeof(char) * size);
		file.ReadParam(&temp.min, sizeof(uint));
		file.ReadParam(&temp.max, sizeof(uint));

		version = ver;
		// ÇªÇÍÇºÇÍÇÃì«çû
		if (ver == "ver.1.0")
		{
			GetSpriteAnimationVer1_0(file, temp.data);
		}

		// èIóπ
		file.CloseFile();
	}

	return temp;

}

void LoadSpriteMesh::GetSpriteAnimationVer1_0(IOFile& file, SPRITE_MESH_ANIM_DATA& spriteAnimation)
{
	size_t size = 0;
	file.ReadParam(&size, sizeof(size_t));
	for (int i = 0; i < size; ++i)
	{
		SPRITE_MESH_TRANSFORM tempAnimData;
		size_t s = 0;
		file.ReadParam(&s, sizeof(size_t));
		tempAnimData.spriteMeshName.resize(s);
		file.ReadParam(&tempAnimData.spriteMeshName[0], sizeof(char) * s);
		file.ReadParam(&tempAnimData.frame, sizeof(int));
		file.ReadParam(&tempAnimData.position, sizeof(VECTOR3));
		file.ReadParam(&tempAnimData.rotation, sizeof(VECTOR3));
		file.ReadParam(&tempAnimData.scale, sizeof(VECTOR3));
		s = 0;
		file.ReadParam(&s, sizeof(size_t));
		tempAnimData.textureName.resize(s);
		file.ReadParam(&tempAnimData.textureName[0], sizeof(char) * s);

		spriteAnimation.anim.emplace_back(tempAnimData);
	}

	size = 0;
	file.ReadParam(&size, sizeof(size_t));
	for (int i = 0; i < size; ++i)
	{
		SPRITE_MESH_ANIM_DATA child;
		GetSpriteAnimationVer1_0(file, child);
		spriteAnimation.child.emplace_back(child);
	}
}

void LoadSpriteMesh::Save(string fileName, const SPRITE_MESH_ANIMATION& animation)
{
	// ì«Ç›çûÇ›
	IOFile file;
	if (file.OpenFile(fileName, std::ios::out))
	{
		size_t size = SPRITE_MESH_VARSION.size();
		file.WriteParam(&size, sizeof(size_t));
		file.WriteParam(&SPRITE_MESH_VARSION[0], sizeof(char) * size);

		size = animation.animationName.size();
		file.WriteParam(&size, sizeof(size_t));
		file.WriteParam(&animation.animationName[0], sizeof(char) * size);
		file.WriteParam(&animation.min, sizeof(uint));
		file.WriteParam(&animation.max, sizeof(uint));

		SetSpriteAnimationVer1_0(file, animation.data);

		file.CloseFile();
	}
}

void LoadSpriteMesh::SetSpriteAnimationVer1_0(IOFile& file, const SPRITE_MESH_ANIM_DATA& spriteAnimation)
{
	size_t size = spriteAnimation.anim.size();
	file.WriteParam(&size, sizeof(size_t));
	for (int i = 0; i < size; ++i)
	{
		auto anim = spriteAnimation.anim[i];
		size_t s = anim.spriteMeshName.size();
		file.WriteParam(&s, sizeof(size_t));
		file.WriteParam(&anim.spriteMeshName[0], sizeof(char) * s);
		file.WriteParam(&anim.frame, sizeof(int));
		file.WriteParam(&anim.position, sizeof(VECTOR3));
		file.WriteParam(&anim.rotation, sizeof(VECTOR3));
		file.WriteParam(&anim.scale, sizeof(VECTOR3));
		s = anim.textureName.size();
		file.WriteParam(&s, sizeof(size_t));
		file.WriteParam(&anim.textureName[0], sizeof(char) * s);
	}

	size = spriteAnimation.child.size();
	file.WriteParam(&size, sizeof(size_t));
	for (auto c : spriteAnimation.child)
	{
		SetSpriteAnimationVer1_0(file, c);
	}
}
