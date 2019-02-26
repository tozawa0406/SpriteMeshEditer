/*
 * @file	SpriteRenderer.h
 * @brief	板ポリ描画
 * @author	戸澤翔太
 * @date	2019/02/26
 */
#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include "../ObjectRenderer.h"

class SpriteRenderer : public ObjectRenderer
{
public:
	/* @brief	コンストラクタ		*/
	SpriteRenderer(void);
	/* @brief	デストラクタ		*/
	virtual ~SpriteRenderer(void);

	/* @brief	初期化処理
	 * @param	(texNum)	テクスチャ番号
	 * @param	(transform)	対応させるトランスフォーム		*/
	void	Init(int texNum, const Transform* transform);

	//void	SetVertex(uint n, uint vnum)  { vertexBuffer = n; vertexNum = vnum; }
	//void	SetIndex(uint n, uint vnum)   { indexBuffer = n; indexNum = vnum;   }

	/* @brief	テクスチャの設定		*/
	inline void SetTexture(int texNum)			{ texNum_ = texNum; }
	/* @brief	分割数の設定			*/
	inline void SetSplit(const VECTOR2& split)	{ split_ = split;	}
	/* @brief	ピボットの設定			*/
	inline void SetPivot(const VECTOR2& pivot)	{ pivot_ = pivot;	}
	/* @brief	ビルボードの設定		*/
	inline void SetBillboard(bool billboard)	{ BitSetFlag(billboard, flagBillboard_, FLAG_BILLBOARD);	}
	/* @brief	X軸の回転のないビルボードの設定		*/
	inline void SetXBillboard(bool xbillboard)	{ BitSetFlag(xbillboard, flagBillboard_, FLAG_X_BILLBOARD); }

	/* @brief	テクスチャの取得		*/
	inline int				GetTexture(void)	const { return texNum_;		}
	/* @brief	アニメーションパターンの取得		*/
	inline float			GetPattern(void)	const { return pattern_;	}
	/* @brief	分割数の取得			*/
	inline const VECTOR2&	GetSplit(void)		const { return split_;		}
	/* @brief	ピボットの取得			*/
	inline const VECTOR2&	GetPivot(void)		const { return pivot_;		}
	/* @ brief	ビルボード状態の取得	*/
	inline bool				IsBillboard(void)	const { return BitCheck(flagBillboard_, FLAG_BILLBOARD);	}
	/* @brief	X軸回転のないビルボードの設定		*/
	inline bool				IsXBillboard(void)	const { return BitCheck(flagBillboard_, FLAG_X_BILLBOARD);	}

	/* @brief	頂点バッファの取得		*/
	uint	GetVertexBuffer(void)	const { return vertexBuffer_;	}
	/* @brief	インデックスバッファの取得		*/
	uint	GetIndexBuffer(void)	const { return indexBuffer_;	}
	/* @brief	頂点数の取得			*/
	uint	GetVertexNum(void)		const { return vertexNum_;		}
	/* @brief	インデックス数の取得	*/
	uint	GetIndexNum(void)		const { return indexNum_;		}
	/* @breif	テクスチャ座標の取得	*/
	VECTOR4 GetTexcoord(void)		const { return texcoord_;		}

	/* @brief	アニメーション処理
	 * @param	(add)	アニメーション速度		*/
	virtual bool Animation(float add) override;

private:
	static constexpr uint8 FLAG_BILLBOARD	= 0x01;
	static constexpr uint8 FLAG_X_BILLBOARD = 0x02;

	//! テクスチャ
	int			texNum_;

	//! アニメーションパターン
	float		pattern_;
	//! 分割数
	VECTOR2		split_;
	//! ピボット
	VECTOR2		pivot_;

	//! ビルボードフラグ
	uint8		flagBillboard_;

	//! 頂点バッファ
	uint	vertexBuffer_;
	//! インデックスバッファ
	uint	indexBuffer_;
	//! 頂点数
	uint	vertexNum_;
	//! インデックス数
	uint	indexNum_;
	//! テクスチャ座標
	VECTOR4 texcoord_;
};

#endif // _SPRITE_RENDERER_H_