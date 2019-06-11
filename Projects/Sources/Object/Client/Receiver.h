/*
 * @file		Receiver.h
 * @brief		レシーバー
 * @author		戸澤翔太
 * @data		2019/03/07
 */
#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "Command/ICommand.h"
#include "SpriteMesh.h"

class ModelEditer;
class Receiver
{
	//! フラグ
	static constexpr uint8 FLAG_IS_HIERARCHY_CHILD	= 0x01;
	static constexpr uint8 FLAG_DELETE				= 0x02;
	static constexpr uint8 FLAG_WITH_CHILD			= 0x04;
	static constexpr uint8 FLAG_IS_HERARCHY			= 0x08;

public:
	/* @brief	コンストラクタ		*/
	Receiver(void);
	/* @brief	デストラクタ		*/
	~Receiver(void);

	/* @brief	初期化処理
	 * @param	(client)	クライアント	*/
	void Init(ModelEditer* client);
	/* @brief	後処理				*/
	void Uninit(void);
	/* @brief	更新処理			*/
	void Update(void);

	/* @brief	名前の取得			*/
	const string&			GetName(void)			{ return name_;				}
	/* @brief	姿勢の取得			*/
	const Transform&		GetTransform(void)		{ return transform_;		}
	/* @breif	レンダラの取得		*/
	const SpriteRenderer*	GetSpriteRenderer(void) { return spriteRenderer_;	}
	/* @brief	前のデータの取得	*/
	const RECEIVER_DATA&	GetBeforeData(void)		{ return beforeData_;		}

	/* @brief	名前の設定			*/
	void SetName(const string& name)					{ name_ = name;				}
	/* @brief	姿勢の設定			*/
	void SetTransform(const Transform& transform)		{ transform_ = transform;	}
	/* @breif	前のデータの設定	*/
	void SetBeforeData(const RECEIVER_DATA& beforeData) { beforeData_ = beforeData; }
	/* @brief	コントローラの設定	*/
	void SetCtrl(Controller* ctrl)						{ ctrl_ = ctrl;				}

	/* @breif	使用状態の設定		*/
	void SetEnable(bool enable);

	/* @breif	セーブ				*/
	void SaveData(SPRITE_MESH_RESOURCE& resource);
	/* @breif	ロード				*/
	bool LoadData(SPRITE_MESH_RESOURCE& resource);

	/* @brief	親の設定			*/
	void		SetParent(Receiver* parent);
	/* @brief	親の取得			*/
	Receiver*	GetParent(void) { return parent_; }

	/* @brief	子の取得			*/
	inline std::vector<Receiver*> GetChild(void) { return child_; }

	/* @brief	ヒエラルキー状態の取得		*/
	bool IsHierarchy(void)				{ return BitCheck(flag_, FLAG_IS_HERARCHY);			}
	/* @brief	ヒエラルキー状態の設定		*/
	void SetHierarchy(bool hierarchy)	{ BitSetFlag(hierarchy, flag_, FLAG_IS_HERARCHY);	}

	/* @brief	ヒエラルキー子要素状態の取得		*/
	bool IsHierarchChild(void)			{ return BitCheck(flag_, FLAG_IS_HIERARCHY_CHILD);	}
	/* @brief	ヒエラルキー子要素状態の設定		*/
	void SetHierarchyChild(bool is)		{ BitSetFlag(is, flag_, FLAG_IS_HIERARCHY_CHILD);	}

	/* @brief	直前に影響を受けたデリートコマンドの設定	*/
	void		SetPrevDelete(ICommand* command)	{ prevDelete_ = command;	}
	/* @brief	直前に影響を受けたデリートコマンドの取得	*/
	ICommand*	GetPrevCommand(void)				{ return prevDelete_;		}

	void Animation(int frame);

	void AddAnim(int frame);
	void AddAnim(int frame, const SPRITE_MESH_ANIMATION& anim, int animNum);
	void RemoveAnim(int frame);

	void ResetAnimData(void) { anim_.clear(); for (auto& c : child_) { if (c) { c->ResetAnimData(); } } }

	void CreateAnimation(SPRITE_MESH_ANIMATION& animation);
	const std::vector<SPRITE_MESH_ANIM_DATA>& GetAnimData(void) { return anim_; }

	const std::vector<SPRITE_MESH_ANIMATION>& GetAnimation(void) { return animation_; }

private:
	/* @brief	今度呼び出し処理			*/
	template<class T>
	bool InvokeCommand(void);

	/* @brief	子要素の設定
	 * @param	(child)		設定する子
	 * @param	(add)		追加か削除か	*/
	void SetChild(Receiver* child, bool add);
	/* @brief	インスペクタでいじる値		*/
	void SelectParam(void);
	/* @brief	削除処理					*/
	void Delete(void);
	/* @brief	デリートコマンドの呼び出し
	 * @param	(withChild)		子要素も共に果てるか		*/
	void InvokeDeleteCommand(bool withChild);

	//! 名前
	string			name_;
	//! レンダラ
	SpriteRenderer* spriteRenderer_;
	//! 姿勢
	Transform		transform_;

	//! 前のデータ
	RECEIVER_DATA	beforeData_;
	//! 直前に影響を受けたのデリートコマンド
	ICommand*		prevDelete_;
	//! テクスチャ名前
	string			textureName_;

	//! クライアント
	ModelEditer*	client_;

	//! コントローラ
	Controller*		ctrl_;
	//! 追加ロード
	LoadAddTexture* loadAdd_;
	
	//! 親
	Receiver*		parent_;
	//! 子
	std::vector<Receiver*> child_;

	//! 状態フラグ
	uint8 flag_;

	//! アニメーション
	std::vector<SPRITE_MESH_ANIM_DATA> anim_;

	std::vector<SPRITE_MESH_ANIMATION> animation_;

	int animCnt_;
};

#endif // _RECEIVER_H_
