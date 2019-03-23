//-----------------------------------------------------------------------------
//
//	ƒ‚ƒfƒ‹•`‰æ[MeshRenderer.cpp]
//	Auther : ŒËàVãÄ‘¾
//																	2018/08/18
//-----------------------------------------------------------------------------
#include "MeshRenderer.h"
#include "../../../Systems/GameSystems.h"
#include "../../../Graphics/Graphics.h"
#include "../../../Graphics/Wrapper.h"

#include "../../../Graphics/DirectX11/Dx11Wrapper.h"
#include "../Shader/Default.h"
#include "../Shader/ZTexture.h"
#include "../../Camera/CameraManager.h"

MeshRenderer::MeshRenderer(void) : ObjectRenderer(ObjectRenderer::RendererType::MODEL)
	, mesh_(nullptr)
	, pattern_(0)
	, patternOld_(0)
	, animation_(0)
	, animationOld_(-1)
	, rate_(0)
	, addRate_(0)
	, isSkinning_(false)
{
}

MeshRenderer::~MeshRenderer(void)
{
}

void MeshRenderer::Init(int modelNum, const Transform* transform)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	assert(modelNum != static_cast<int>(Resources::Model::Base::UNOWN));
	ObjectRenderer::Init(systems->GetObjectRenderer(), transform);

	if (const auto& model = systems->GetModel())
	{
		mesh_ = model->GetMesh(modelNum);
	}
	shader_		= Shader::ENUM::DEFAULT;

	animationMax_.clear();
	if(mesh_ && mesh_->bone.size() > 0)
	{
		for (const auto& anim : mesh_->bone[0].animMtx)
		{
			animationMax_.emplace_back((int)anim.size());
		}
	}
}

void MeshRenderer::ChangeModel(int modelNum)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }
	if (const auto& model = systems->GetModel())
	{
		mesh_ = model->GetMesh(modelNum);
	}
	animationMax_.clear();
	if (mesh_ && mesh_->bone.size() > 0)
	{
		for (const auto& anim : mesh_->bone[0].animMtx)
		{
			animationMax_.emplace_back((int)anim.size());
		}
	}
}

void MeshRenderer::ChangeAnimation(int animNum, int frame, bool end)
{
	if (animation_ != animNum)
	{
		end_ = end;
		patternOld_		= pattern_;
		animationOld_	= animation_;
		rate_			= 0;
		addRate_		= 1.0f / frame;
		animation_		= animNum;
		pattern_		= 0;
	}
}

void MeshRenderer::AnimEndPattern(void)
{
	pattern_ = static_cast<float>(animationMax_[animation_] - 1);
}

bool MeshRenderer::Animation(float add)
{
	isSkinning_ = false;
	if (animationMax_.size() <= (uint)animation_) { return false; }

	if (animationOld_ >= 0)
	{
		rate_		+= addRate_;
		if (!end_)
		{
			patternOld_ += add;
		}
		if (rate_ >= 1)
		{
			animationOld_	= -1;
			rate_			= 0;
			addRate_		= 0;
			patternOld_		= 0;
		}
	}

	pattern_ += add;
	int m = (add < 0) ? 1 : 0;
	if(fabs(pattern_) >= animationMax_[animation_] - ((4 * m) + 1))
	{
		patternOld_ = pattern_ - add;
		pattern_ = 0;
		return true;
	}
	return false;
}

void MeshRenderer::Skinning(void)
{
//	if (isSkinning_) { return; }
	isSkinning_ = true;

	const auto& systems = Systems::Instance();
	if (!systems) { return; }
	if (!mesh_) { return; }

	Dx11Wrapper* dx11 = static_cast<Dx11Wrapper*>(wrapper_);
	if (!dx11) { return; }

	MATRIX rootMtx;
	bool rootFrameTransformMatrix = true;
	if (mesh_->transMtx == 0.0f) { rootFrameTransformMatrix = false; }
	else { rootMtx = mesh_->transMtx; }

	DefaultShader::CONSTANT cbuf;
	MATRIX initMtx = MATRIX().Identity();
	memcpy_s(&cbuf.view , sizeof(MATRIX), &systems->GetSceneManager()->GetScene()->GetCameraManager()->GetView(), sizeof(MATRIX));
	memcpy_s(&cbuf.proj , sizeof(MATRIX), &systems->GetSceneManager()->GetScene()->GetCameraManager()->GetProj(), sizeof(MATRIX));

	cbuf.texcoord = VECTOR4(0, 0, 1, 1);
	memcpy_s(&cbuf.diffuse, sizeof(float) * 4, &GetMaterial().diffuse, sizeof(float) * 4);

	const ZTexture* depth = (ZTexture*)systems->GetShader()->GetShader(Shader::ENUM::ZTEXTURE);
	cbuf.lightView = depth->GetLightView();
	cbuf.lightProj = depth->GetLightProj();

	DefaultShader* defShader = ((DefaultShader*)systems->GetShader()->GetShader(Shader::ENUM::DEFAULT));

	DefaultShader::CONSTANT_BONE cbuf1;
	ZeroMemory(&cbuf1, sizeof(DefaultShader::CONSTANT_BONE));

	int s = (int)mesh_->bone.size();
	for (int j = 0; j < s; ++j)
	{
		memcpy_s(&cbuf1.boneInv[j], sizeof(MATRIX), &mesh_->bone[j].inverseMtx, sizeof(MATRIX));

		int anim = (int)pattern_;
		if (mesh_->bone[j].animMtx.size() <= (uint)animation_ || mesh_->bone[j].animMtx[animation_].size() == 0)
		{
			memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &mesh_->bone[j].offsetMtx, sizeof(MATRIX));
			memcpy_s(&mesh_->bone[j].nowBone, sizeof(MATRIX), &mesh_->bone[j].offsetMtx, sizeof(MATRIX));
		}
		else
		{
			float rate = rate_;
			anim = (anim % mesh_->bone[j].animMtx[animation_].size());
			if (animationOld_ < 0)
			{
				float patternRate = pattern_ - (int)pattern_;
				MATRIX mtx1, mtx2;
				memcpy_s(&mtx1, sizeof(MATRIX), &mesh_->bone[j].animMtx[animation_][anim], sizeof(MATRIX));

				int next = anim + 1;
				next = (next >= animationMax_[animation_]) ? 0 : next;
				memcpy_s(&mtx2, sizeof(MATRIX), &mesh_->bone[j].animMtx[animation_][next], sizeof(MATRIX));

				MATRIX m = (mtx2 * patternRate) + (mtx1 * (1 - patternRate));
				memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &m, sizeof(MATRIX));
				memcpy_s(&mesh_->bone[j].nowBone, sizeof(MATRIX), &m, sizeof(MATRIX));
			}
			else
			{
				MATRIX mtx1, mtx2;
				if (mesh_->bone[j].animMtx.size() >(uint)animationOld_ || mesh_->bone[j].animMtx[animationOld_].size() > 0)
				{
					memcpy_s(&mtx1, sizeof(MATRIX), &mesh_->bone[j].animMtx[animation_][anim], sizeof(MATRIX));
					anim = (int)patternOld_;
					anim = (anim % mesh_->bone[j].animMtx[animationOld_].size());
					memcpy_s(&mtx2, sizeof(MATRIX), &mesh_->bone[j].animMtx[animationOld_][anim], sizeof(MATRIX));

					MATRIX m = (mtx1 * rate) + (mtx2 * (1 - rate));
					memcpy_s(&cbuf1.boneAnim[j], sizeof(MATRIX), &m, sizeof(MATRIX));
					memcpy_s(&mesh_->bone[j].nowBone, sizeof(MATRIX), &m, sizeof(MATRIX));
				}
			}
		}
	}

	string temp = "";
	int size[512] = { sizeof(MATRIX),  };
	dx11->SetShaderValue(defShader->GetConstantBuffer(1), s, &temp, size, &cbuf1);
	dx11->SetConstantBuffer(Wrapper::ShaderType::Vertex, 1, 1, defShader->GetConstantBuffer(1));
}
