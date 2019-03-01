#include "LoadSpriteMesh.h"

SPRITE_MESH_RESOURCE LoadSpriteMesh::Load(string fileName)
{
	readNum_ = 0;
	SPRITE_MESH_RESOURCE temp;

	// 読み込み
	IOFile file;
	if (file.OpenFile(fileName, std::ios::in))
	{
		// 全体量の取得
		size_t size = 0;
		file.ReadParam(&size, sizeof(size_t));

		for (size_t i = 0; i < size; ++i)
		{
			// 読み込みで全体量に達したら
			if (readNum_ >= size) { break; }

			// それぞれの読込
			GetSpriteMesh(file, temp, false);
		}
		// 終了
		file.CloseFile();
	}

	return temp;
}

void LoadSpriteMesh::GetSpriteMesh(IOFile& file, SPRITE_MESH_RESOURCE& spriteMesh, bool parent)
{
	bool isParent = false;
	file.ReadParam(&isParent, sizeof(bool));
	if (!parent && isParent) { return; }

	readNum_++;

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
		GetSpriteMesh(file, temp, true);
		spriteMesh.children.emplace_back(temp);
	}
}

void LoadSpriteMesh::Save(string fileName, SPRITE_MESH_RESOURCE resource)
{

}
