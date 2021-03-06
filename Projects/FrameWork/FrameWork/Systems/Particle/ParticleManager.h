//-----------------------------------------------------------------------------
//
//	p[eBNĮ[ParticleManager.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _PARTICLE_MANAGER_H_ 
#define _PARTICLE_MANAGER_H_

#include "../../Define/Define.h"
#include "../BaseManager.h"

class Systems;
class Particle;
class ParticleManager : public BaseManager<Particle>
{
public:
	ParticleManager(Systems* systems) : BaseManager(systems) {}
	HRESULT Init(void)   override { return S_OK; }
	void	Uninit(void) override;		// ã
	void	Update(void) override;		// XV
	void	Draw(void)   override;		// `æ

private:
	void Sort(void);
	void DestroyCheck(void);

	friend Particle;
};

#endif // _OBJECT_H_