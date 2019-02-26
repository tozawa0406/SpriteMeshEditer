/*
 * @file	ObjectRendererManager.h
 * @brief	3D�I�u�W�F�N�g�`��̊Ǘ�
 * @author	���V�đ�
 * @date	2019/02/25
 */
#ifndef _OBJECT_RENDERER_MANAGER_H_
#define _OBJECT_RENDERER_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class ObjectRenderer;
class ObjectRendererManager : public BaseManager<ObjectRenderer>
{
	friend ObjectRenderer;
public:
	ObjectRendererManager(Systems* systems) : BaseManager(systems) {}

	virtual HRESULT Init(void) override { return S_OK; }
	virtual void    Draw(void) override;
	void	FastDraw(void);
	void	DrawShadow(void);

private:
	virtual void Add(ObjectRenderer* obj) override;
	void Sort(void);
};


#endif // _OBJECT_RENDERER_MANAGER_H_