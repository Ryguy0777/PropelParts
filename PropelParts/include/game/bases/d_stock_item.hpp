#pragma once

#include <game/bases/d_a_2d_player.hpp>
#include <game/bases/d_a_wm_item.hpp>
#include <game/bases/d_base.hpp>
#include <game/bases/d_lytbase.hpp>
#include <game/bases/d_stock_item_shadow.hpp>
#include <game/sLib/s_State.hpp>
#include <game/sLib/s_StateMethodUsr_FI.hpp>
#include <game/bases/d_effect.hpp>


/// @unofficial
class dStockItem_c : public dBase_c {
public:
    enum N_PANE_e {
        N_forUse_1PPos, N_forUse_2PPos, N_forUse_3PPos, N_forUse_4PPos,
        // Omitting panes that we grab later
        N_stockItem = 11, N_stockItem_01, N_itemSelect_00,
        N_COUNT
    };

    enum P_PANE_e {
        P_iconBase_00 = 14, P_iconBase_01, P_iconBase_02, P_iconBase_03, P_iconBase_04,
        P_iconBase_05, P_iconBase_06, P_iconBase_07, P_iconBase_08, P_iconBase_09,
        P_COUNT
    };

    enum N_NEW_PANE_e {
        N_iconKinoko_00, N_iconFlower_00, N_iconPro_00, N_iconIce_00,
        N_iconPen_00, N_mameKinoko_00, N_iconStar_00, N_iconHammer_00,
        N_NEW_COUNT
    };

    enum P_NEW_PANE_e {
        P_iconKinoko_00, P_iconFlower_00, P_iconPro_00, P_iconIce_00,
        P_iconPen_00, P_mameKinoko_00, P_iconStar_00, P_iconHammer_00,
        P_buttonBase_00, P_buttonBase_01, P_buttonBase_02, P_buttonBase_06,
        P_buttonBase_04, P_buttonBase_05, P_buttonBase_03, P_buttonBase_07,
        P_NEW_COUNT
    };

    dStockItem_c();

    bool createLayout();
    int getPlyPosPane(int);
    void setItem();
    void createButtonEffect();
    void createPlyItemEffect(int, mVec3_c *, int);
    void setShadowInfo(int count, int item);

    STATE_FUNC_DECLARE(dStockItem_c, ButtonChangeAnimeEndWait);
    STATE_FUNC_DECLARE(dStockItem_c, ItemSelectWait);

    u32 m_70;
    LytBase_c mLayout;
    sFStateMgr_c<dStockItem_c, sStateMethodUsr_FI_c> mStateMgr;
    nw4r::lyt::Pane *mpRootPane;

    nw4r::lyt::Pane *mpNullPanes[N_COUNT];
    nw4r::lyt::Picture *mpPicturePanes[P_COUNT];

    da2DPlayer_c *mpPlayers[4];
    daWmItem_c *mpItems[7];
    dStockItemShadow_c *mpShadow;

    int mScrollDir;
    u32 m_318;
    u32 m_31C;
    u32 m_320;
    u32 m_324;
    int mItemUsed;

    mEf::effect_c mPlayerEffects[4];
    dEf::followEffect_c mButtonEffect;
    PLAYER_TYPE_e mPlyTypes[4]; // Index for what effect obj plays on each player

    int mCurrItem;
    int mPrevItem;
    int mExitDelay;
    int mSuccessionSelectDelay;
    int mPlayerCount;
    int mItemCounts[7];
    int mEffectBtnIdx; // Which button to play the effect on
    int mAnimID;

    bool mPlayerActive[4];
    bool mHasLayoutLoaded;
    bool mIsVisible;
    bool mIsSuccessionSelect;

    // New
    int mGroupNum;
    daWmItem_c *mpNewItems[NEW_ITEM_COUNT];
    nw4r::lyt::Pane *mpNewNullPanes[N_NEW_COUNT];
    nw4r::lyt::Picture *mpNewPicturePanes[P_NEW_COUNT];
    int mNewItemCounts[NEW_ITEM_COUNT];

    static const int sc_onBtnAnimeIdxTbl[];
    static const int sc_hitBtnAnimeIdxTbl[];
    static const int sc_offBtnAnimeIdxTbl[];
    static const int sc_itemPlyModeTbl[];
};
