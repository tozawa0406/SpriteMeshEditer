#ifndef _PIVOT_H_
#define _PIVOT_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Object/Object.h>

class Pivot : public Object
{
public:
	/* @brief	コンストラクタ		*/
	Pivot(void);
	/* @brief	デストラクタ		*/
	virtual ~Pivot(void);

	virtual void Init(void)		override;
	virtual void Uninit(void)	override;
	virtual void Update(void)	override;

	void SetTransform(const Transform& transform) { transform_.position = transform.position; }

private:
	Transform		transform_;
	SpriteRenderer* pivot_;
};

#endif // _PIVOT_H_
