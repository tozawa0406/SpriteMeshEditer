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
			GetSpriteMeshVer1_1(file, temp);
		}

		// èIóπ
		file.CloseFile();
	}

	return temp;
}

void LoadSpriteMesh::GetSpriteMeshVer1_1(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh)
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
	for (int i = 0; i < size; ++i)
	{
		SPRITE_MESH_RESOURCE temp;
		GetSpriteMeshVer1_1(file, temp);
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

		SetSpriteMeshVer1_1(file, resource);

		file.CloseFile();
	}
}

void LoadSpriteMesh::SetSpriteMeshVer1_1(IOFile& file, const SPRITE_MESH_RESOURCE& spriteMesh)
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
	for (int i = 0; i < size; ++i)
	{
		SetSpriteMeshVer1_1(file, spriteMesh.children[i]);
	}
}
