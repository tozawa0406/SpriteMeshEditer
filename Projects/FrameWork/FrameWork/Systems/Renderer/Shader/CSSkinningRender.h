//-----------------------------------------------------------------------------
//
//	ftHgVF[_[[Default.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _CS_SKINNING_RENDER_SHADER_H_
#define _CS_SKINNING_RENDER_SHADER_H_

#include "Shader.h"
#include "../Sprite/Texture.h"

class CSSkinningRenderShader : public Shader
{
	static constexpr char* FILENAME = "Default.fx";

	HRESULT Init(void) override;
public:
	CSSkinningRenderShader(ShaderManager* manager) : Shader(manager, shaderDirectoryName + FILENAME) {}
	HRESULT SetParam(const MATRIX& mtx, const COLOR& color, VECTOR4 texcoord) const override;

	struct CONSTANT
	{
		MATRIX  world;
		MATRIX  view;
		MATRIX  proj;

		VECTOR4 texcoord;
		VECTOR4 diffuse;
		MATRIX  lightView;
		MATRIX  lightProj;
	};
};

#endif // _CS_SKINNING_RENDER_SHADER_H_