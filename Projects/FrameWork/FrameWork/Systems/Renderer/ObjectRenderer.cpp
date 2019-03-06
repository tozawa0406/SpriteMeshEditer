#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(RendererType type) : 
	manager_(nullptr)
	, wrapper_(nullptr)
	, transform_(nullptr)
	, flag_(FLAG_ENABLE | FLAG_SORT)
	, type_(type)
	, shader_(Shader::ENUM::UNKOUWN)
{
	material_.diffuse  = COLOR(1, 1,  1,  1);
	material_.ambient  = COLOR(0.3f, 0.3f, 0.3f, 1);
	material_.emission = COLOR(0, 0, 0, 0);
	material_.specular = COLOR(0, 0, 0, 0);
	material_.power    = 0;
}

ObjectRenderer::~ObjectRenderer(void)
{
	if (manager_)
	{
		manager_->Remove(this);
	}
}

void ObjectRenderer::Init(ObjectRendererManager* manager, const Transform* transform)
{
	if (!manager) { return; }

	manager_	= manager;
	transform_	= transform;
	manager->Add(this);

	if (const auto& systems = manager->GetSystems())
	{
		if (const auto& graphics = systems->GetGraphics())
		{
			wrapper_ = graphics->GetWrapper();
		}
	}
}
