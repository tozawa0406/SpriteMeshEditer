/*
 * @file	ObjectRenderer.h
 * @brief	3Dオブジェクト描画の基底クラス
 * @author	戸澤翔太
 * @date	2019/02/25
 */
#ifndef _OBJECT_RENDERER_H_
#define _OBJECT_RENDERER_H_

#include "../../Define/Define.h"
#include "../../Graphics/Wrapper.h"
#include "../GameSystems.h"
#include "Sprite/Texture.h"
#include "Model/Model.h"
#include "Shader/ShaderManager.h"
#include "../../Graphics/Utility/Utility.h"

class ObjectRendererManager;
class ObjectRenderer
{
public:
	enum RendererType
	{
		SPRITE = 0,
		MODEL,
	};

	/* @brief	コンストラクタ		*/
	ObjectRenderer(RendererType type);
	/* @brief	デストラクタ		*/
	virtual ~ObjectRenderer(void);

	/* @brief	アニメーション		*/
	virtual bool Animation(float add) = 0;

	/* @brief	トランスフォームの取得		*/
	const Transform*	GetTransform(void)	const { return transform_; }

	/* @brief	シェーダーの設定	*/
	inline void SetShader(Shader::ENUM shader)			{ shader_ = shader;		}
	/* @brief	マテリアルの設定	*/
	inline void SetMaterial(const MATERIAL& material)	{ material_ = material; }
	/* @brief	使用状態の設定		*/
	inline void SetEnable(bool enable)		{ BitSetFlag(enable, flag_, FLAG_ENABLE);		}
	/* @brief	先に描画する設定	*/
	inline void SetFastDarw(bool fastDraw)	{ BitSetFlag(fastDraw, flag_, FLAG_FAST_DRAW);	}
	/* @brief	影の有無の設定		*/
	inline void SetShadow(bool shadow)		{ BitSetFlag(shadow, flag_, FLAG_SHADOW);		}
	/* @brief	ソート有無の設定	*/
	inline void SetSort(bool sort)			{ BitSetFlag(sort, flag_, FLAG_SORT);			}

	/* @brief	使用するシェーダーの取得	*/
	inline Shader::ENUM		GetShader(void)		const { return shader_;		}
	/* @brief	マテリアルの取得	*/
	inline const MATERIAL&	GetMaterial(void)	const { return material_;	}
	/* @brief	使用状態の取得		*/
	inline bool	IsEnable(void)		const { return BitCheck(flag_, FLAG_ENABLE); }
	/* @brief	先に描画するか		*/
	inline bool	IsFastDraw(void)	const { return BitCheck(flag_, FLAG_FAST_DRAW); }
	/* @brief	影の有無			*/
	inline bool	IsShadow(void)		const { return BitCheck(flag_, FLAG_SHADOW); }
	/* @brief	ソートするか		*/
	inline bool	IsSort(void)		const { return BitCheck(flag_, FLAG_SORT); }

	inline RendererType GetType(void) const { return type_; }

protected:
	/* @brief	初期化
	 * @param	(manager)	マネージャー
	 * @param	(transform)	対応するドランスフォームのポインタ		*/
	void Init(ObjectRendererManager* manager, const Transform* transform);

	//! マネージャー
	ObjectRendererManager*	manager_;
	Wrapper*				wrapper_;
	//! トランスフォーム
	const Transform*		transform_;

	//! マテリアル
	MATERIAL		material_;
	//! 使用するシェーダー
	Shader::ENUM	shader_;

private:
	static constexpr uint8 FLAG_ENABLE		= 0x01;
	static constexpr uint8 FLAG_FAST_DRAW	= 0x02;
	static constexpr uint8 FLAG_SHADOW		= 0x04;
	static constexpr uint8 FLAG_SORT		= 0x08;

	//! 状態を示すフラグ
	uint8			flag_;
	//! レンダラの種類
	RendererType	type_;
};

#endif // _CANVAS_RENDERER_H_