//=============================================================================
//
// モーション処理  [ motion.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "kananlibrary.h"
#include <vector>

//=============================================================================
// クラス定義
//=============================================================================
class CModelCharacter;

class CMotion
{
public:
	// キー
	typedef struct
	{
		D3DXVECTOR3 posDest;		// 目的の位置
		D3DXVECTOR3 rotDest;		// 目的の回転
	} KEY;

	// キー情報
	typedef struct
	{
		KEY *pKey;					// キーのポインタ
		int nFrame;					// フレーム数
		float fMove;				// 移動値
		std::vector<int> nAttackIndex;	// 攻撃パーツの取得
	} KEY_INFO;

	// モーションの情報
	typedef struct
	{
		KEY_INFO *pKeyInfo;			// キー情報のポインタ
		int nNumKey;				// キー数
		int nAttackKey;				// 攻撃キー
		bool bLoop;					// ループするかどうか
	} MOTION_INFO;

	// モーション
	typedef enum
	{
		MOTION_NONE,
		PLAYER_NEUTRAL = 0,		// プレイヤー ニュートラル
		PLAYER_WALK,			// プレイヤー 走る
		PLAYER_JUMP,			// プレイヤー ジャンプ
		PLAYER_FALL,			// プレイヤー 落下
		PLAYER_LIFT,			// プレイヤー もの持ち上げる
		PLAYER_LIFT_NEUTRAL,	// プレイヤー 物持ちニュートラル
		PLAYER_LIFT_WALK,		// プレイヤー 物持ち歩き
		PLAYER_THROW,			// プレイヤー もの投げ
		PLAYER_ATTACK_0,		// プレイヤー 攻撃1
		PLAYER_ATTACK_1,		// プレイヤー 攻撃2
		PLAYER_ATTACK_2,		// プレイヤー 攻撃3
		PLAYER_ATTACK_3,		// プレイヤー 攻撃4
		PLAYER_SMASH_CHARGE,	// プレイヤー スマッシュチャージ
		PLAYER_SMASH,			// プレイヤー スマッシュ
		PLAYER_DAUNTED,			// プレイヤー 怯みダメージ
		PLAYER_BLOWAWAY,		// プレイヤー 吹っ飛びダメージ
		PLAYER_DOWN,			// プレイヤー ダウン
		PLAYER_GETUP,			// プレイヤー 起き上がり
		PLAYER_GETUP_ACTIVE,	// プレイヤー アクティブな起き上がり
		//PLAYER_WALLDOWN,		// プレイヤー 壁は貼り付き
		MOTION_MAX				// 最大値
	} MOTION_TYPE;

	CMotion();
	~CMotion();

	static HRESULT Load();						// モーションのロード
	static void UnLoad();						// モーションのアンロード

	static D3DXVECTOR3 GetRotDest(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].pKey[nIndex].rotDest; }	// モーションの回転のゴール取得
	static D3DXVECTOR3 GetPosDest(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].pKey[nIndex].posDest; }	// モーションの回転のゴール取得
	static int GetFrame(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nFrame; }					// モーションの総フレーム数の取得
	static float GetMotionMove(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].fMove; }					// モーションの移動値取得
	static int GetNumKey(PARAM_TYPE charaType, MOTION_TYPE motiontype)
		{ return m_pMotionInfo[charaType][motiontype].nNumKey; }								// モーションの総キー数の取得
	static bool GetLoop(PARAM_TYPE charaType, MOTION_TYPE motiontype)
		{ return m_pMotionInfo[charaType][motiontype].bLoop; }									// モーションがループするかどうか取得
	static int GetAttackKey(PARAM_TYPE charaType, MOTION_TYPE motiontype) 
		{ return m_pMotionInfo[charaType][motiontype].nAttackKey; }								// 攻撃キーの取得
	static int *GetnAttackIndex(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nAttackIndex.data(); }		// モーションの攻撃パーツの取得
	static int GetnAttackIndexSize(PARAM_TYPE charaType, MOTION_TYPE motiontype, int nKey)
		{ return m_pMotionInfo[charaType][motiontype].pKeyInfo[nKey].nAttackIndex.size(); }		// モーションの攻撃パーツのサイズの取得

private:
	static MOTION_INFO m_pMotionInfo[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX];		// モーション情報
	static HRESULT LoadMotion(PARAM_TYPE charaType, MOTION_TYPE motiontype);	// モーション読み込み	

	static char m_apFileName[MAX_CHARACTER_TYPE][CMotion::MOTION_MAX][MAX_TEXT];
};
#endif