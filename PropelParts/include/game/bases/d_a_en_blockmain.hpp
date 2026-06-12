#pragma once
#include <game/bases/d_enemy.hpp>
#include <game/bases/d_bg_ctr.hpp>

class daEnBlockMain_c : public dEn_c {
public:
    struct BlockSpawnInfo_s {
        mVec3_c mBlockPos;
        u8 mPlrNo;
        int mItemIdx;
        bool mIsMultiplayer;
        bool mNotGroundPound;
        u8 m_16; // Dunno what this is for
    };

    virtual ~daEnBlockMain_c(); 

    virtual void initialize_upmove();
    virtual void initialize_downmove();

    virtual void block_upmove();
    virtual void block_upmove_diff();
    virtual void block_downmove();
    virtual void block_downmove_end();
    virtual void block_downmove_diff();
    virtual void block_downmove_diffend();

    virtual void block_scale_set(u8 movingDown);

    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, UpMove);
    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, DownMove);
    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, DownMoveEnd);
    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, UpMove_Diff);
    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, DownMove_Diff);
    STATE_VIRTUAL_FUNC_DECLARE(daEnBlockMain_c, DownMove_DiffEnd);

    u8 ObjBgHitCheck();
    void ObjBg_PonCheck();
    void ObjBg_PonCheck_jump();

    void Block_CreateClearSet(float initialY);
    void HopCoinBgcheckSet();
    u32 HopCoinBgcheck();
    void Block_ExecuteClearSet();

    void jumpdai_set();
    void itemkey_set(u8);
    void item_ivy_set(u8, u8);

    bool isYossyColor(u16);
    u16 yossy_color_search();
    void yossy_set(ulong);
    void multi_yossy_set(ulong);
    void eggitem_set(ulong);
    void multi_eggitem_set(ulong);

    void player_set(int, int);
    void continue_star_check(int *item_type, s8 player_no);
    int player_bigmario_check(s8 player_no);
    int propeller_kinoko_check(int, s8);
    int playernumber_set();

    int YoshiEggCreateCheck(int);
    void item_sound_set(mVec3_c& position, int type, s8 player_no, u8, u8);
    void item_start_set(BlockSpawnInfo_s*, int); /// @unofficial
    void item_set(BlockSpawnInfo_s*, int); /// @unofficial
    bool item_start_check(int); /// @unofficial

    static void callBackF(dActor_c*, dActor_c*);
    static void nomal_callBackF(dActor_c*, dActor_c*);
    static void callBackH(dActor_c*, dActor_c*);
    static void side_block_moveset(daEnBlockMain_c*, dActor_c*, u8);
    static void shell_callBackW(dActor_c*, dActor_c*, u8);
    static void callBackW(dActor_c*,dActor_c*, u8);
    static void obj_callBackW(dActor_c*, dActor_c*, u8);
    static void enemy_only_callBackW(dActor_c*, dActor_c*, u8);
    static void playeronly_callBackF(dActor_c*, dActor_c*);
    static void playeronly_callBackH(dActor_c*, dActor_c*);
    static void playeronly_callBackW(dActor_c*, dActor_c*);

    static bool checkRevFoot(dActor_c*, dActor_c*);
    static bool checkRevHead(dActor_c*, dActor_c*);
    static bool checkRevWall(dActor_c*, dActor_c*, u8);

    static void clear_block_collcallback(dCc_c*, dCc_c*);

    u8 mPad[0x10];
    dBg_ctr_c mBg;
    u8 mPad2[0x18];
    float mInitialY;
    float mMoveAccel;
    float m_114, m_118, m_11c;
    u32 mDownMoveCounter;
    int mHasBeenHit, m_128;
    u32 mAmountOfItems;
    u8 mPad3[0xC];
    short mTimer;
    u8 mPad4[0x27];
    u8 mIsGroundPound, mAnotherFlag;
    u8 m_167, m_168, m_169;
    u8 mPad5[0x6];
    u8 mPlayerID;
    u8 mPad6[0x3];
};

extern const u32 l_item_values[18]; /// @unofficial