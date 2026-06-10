#include <kamek.h>
#include <game/bases/d_a_player_manager.hpp>
#include <game/bases/d_a_wm_player.hpp>
#include <game/bases/d_game_com.hpp>
#include <game/bases/d_info.hpp>
#include <game/bases/d_SelectCursor.hpp>
#include <game/bases/d_stock_item.hpp>
#include <game/snd/snd_scene_manager.hpp>
#include <game/snd/snd_audio_mgr.hpp>
#include <constants/game_constants.h>
#include <constants/sound_list.h>

int checkValidStockItem(int item, int playerMode, bool *isStarPower);

// Arrays for button group IDs
const int dStockItem_c::sc_onBtnAnimeIdxTbl[] = {
    1, 2, 3, 4, 5, 6, 7, 30
};
const int dStockItem_c::sc_hitBtnAnimeIdxTbl[] = {
    15, 16, 17, 18, 19, 20, 21, 32
};
const int dStockItem_c::sc_offBtnAnimeIdxTbl[] = {
    22, 23, 24, 25, 26, 27, 28, 33
};

// Player Modes to assign for each item button
const int dStockItem_c::sc_itemPlyModeTbl[] = {
    POWERUP_MUSHROOM,
    POWERUP_FIRE_FLOWER,
    POWERUP_PROPELLER_SHROOM,
    POWERUP_ICE_FLOWER,
    POWERUP_PENGUIN_SUIT,
    POWERUP_MINI_MUSHROOM,
    -1, // Dummy for Star Power
    POWERUP_HAMMER_SUIT
};

// dStockItem_c_classInit()
kmBranchDefCpp(0x807AF370, NULL, dStockItem_c *) {
    return new dStockItem_c;
}

// Fix WM_ITEM array offsets
kmWrite16(0x809272CE, offsetof(dStockItem_c, mpNewItems[0]));
kmWrite16(0x807B147A, offsetof(dStockItem_c, mpNewItems[0]));
kmWrite16(0x807B0B8E, offsetof(dStockItem_c, mpNewItems[0]));
kmWrite16(0x807B0312, offsetof(dStockItem_c, mpNewItems[0]));
kmWrite16(0x807B0B6A, offsetof(dStockItem_c, mpNewItems[0]));
kmWrite16(0x807B0B82, offsetof(dStockItem_c, mpNewItems[0]));

// Fix mItemCounts array offsets
kmWrite16(0x807B0A76, offsetof(dStockItem_c, mNewItemCounts[0]));
kmWrite16(0x807B02DA, offsetof(dStockItem_c, mNewItemCounts[0]));
kmWrite16(0x807B02E2, offsetof(dStockItem_c, mNewItemCounts[0]));
kmWrite16(0x807B02F2, offsetof(dStockItem_c, mNewItemCounts[0]));
kmWrite16(0x807B0306, offsetof(dStockItem_c, mNewItemCounts[0]));

// Set shadow info for 8 powerups
kmWrite16(0x807B0A8E, NEW_ITEM_COUNT);

// dStockItem_c::create()
kmBranchDefCpp(0x807AF730, NULL, int, dStockItem_c *this_) {
    if (!this_->mHasLayoutLoaded) {
        if (!this_->createLayout()) {
            return false;
        }

        // Scale down layout for 4:3 mode
        if (dGameCom::GetAspectRatio() == 0) {
            nw4r::math::VEC2 scale(0.85f, 0.85f);
            this_->mpRootPane->SetScale(scale);
        }

        // Set item counts
        for (int i = 0; i < NEW_ITEM_COUNT; i++) {
            this_->mNewItemCounts[i] = dInfo_c::m_instance->getStockItem(i);
            // Hide dummy item icons
            this_->mpNewPicturePanes[dStockItem_c::P_iconKinoko_00 + i]->SetVisible(false);
        }

        this_->mpRootPane->SetVisible(false);
        this_->mpNullPanes[dStockItem_c::N_itemSelect_00]->SetVisible(false);
        this_->mExitDelay = 0;

        this_->mHasLayoutLoaded = true;
        this_->mIsVisible = false;

        // Set dummy values
        for (int i = 0; i < 4; i++) {
            this_->mPlyTypes[0] = PLAYER_COUNT;
        }

        this_->mEffectBtnIdx = NEW_ITEM_COUNT;
        this_->mLayout.mDrawOrder = 15;
    }

    return true;
}

// dStockItem_c::createLayout()
kmBranchDefCpp(0x807AF8D0, NULL, bool, dStockItem_c *this_) {
    if (!this_->mLayout.ReadResource("stockItem_powerup/stockItem.arc", false)) {
        return false;
    }

    this_->mLayout.build("stockItem_27.brlyt", nullptr);

    static const char *AnmNameTbl[] = {
        "stockItem_27_inWindow.brlan",
        "stockItem_27_itemOnButton.brlan",
        "stockItem_27_itemIdleButton.brlan",
        "stockItem_27_itemHitButton.brlan",
        "stockItem_27_itemOffButton.brlan",
        "stockItem_27_outWindow.brlan"
    };
    static const char *GROUP_NAME_DT[] = {
        "A00_Window",
        "B00_itemButton", "B01_itemButton", "B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
        "B00_itemButton", "B01_itemButton", "B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
        "B00_itemButton", "B01_itemButton", "B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
        "B00_itemButton", "B01_itemButton", "B02_itemButton", "B06_itemButton", "B04_itemButton", "B05_itemButton", "B03_itemButton",
        "A00_Window",
        // New
        "B07_itemButton", "B07_itemButton", "B07_itemButton", "B07_itemButton"
    };
    static const int ANIME_INDEX_TBL[] = {
        0,
        1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4, 4,
        5,
        // New
        1, 2, 3, 4,
    };

    this_->mLayout.AnimeResRegister(AnmNameTbl, ARRAY_SIZE(AnmNameTbl));
    this_->mLayout.GroupRegister(GROUP_NAME_DT, ANIME_INDEX_TBL, ARRAY_SIZE(GROUP_NAME_DT));
    // Need this for later
    this_->mGroupNum = ARRAY_SIZE(ANIME_INDEX_TBL);

    static const char *N_PANE_NAME_DT[] = {
        "N_forUse_1PPos", "N_forUse_2PPos", "N_forUse_3PPos", "N_forUse_4PPos",
        "N_iconKinoko_00", "N_iconFlower_00", "N_iconPro_00", "N_iconIce_00", "N_iconPen_00", "N_mameKinoko_00", "N_iconStar_00",
        "N_stockItem", "N_stockItem_01",
        "N_itemSelect_00"
    };

    static const char *T_PANE_FIXED_NAME_TBL[] = {
        "T_titleStock_00",
        "T_titleUse_01"
    };
    static const int MESSAGE_DATA_TBL[] = {
        0x1E,
        0x2F
    };

    static const char *P_PANE_NAME_DT[] = {
        "P_iconKinoko_00", "P_iconFlower_00", "P_iconPro_00", "P_iconIce_00", "P_iconPen_00", "P_mameKinoko_00", "P_iconStar_00",
        "P_buttonBase_00", "P_buttonBase_01", "P_buttonBase_02", "P_buttonBase_06", "P_buttonBase_04", "P_buttonBase_05", "P_buttonBase_03",
        "P_iconBase_00", "P_iconBase_01", "P_iconBase_02", "P_iconBase_03", "P_iconBase_04",
        "P_iconBase_05", "P_iconBase_06", "P_iconBase_07", "P_iconBase_08", "P_iconBase_09"
    };

    this_->mpRootPane = this_->mLayout.getRootPane();
    this_->mLayout.NPaneRegister(N_PANE_NAME_DT, &this_->mpNullPanes[dStockItem_c::N_forUse_1PPos], ARRAY_SIZE(N_PANE_NAME_DT));
    this_->mLayout.TPaneNameRegister(T_PANE_FIXED_NAME_TBL, MESSAGE_DATA_TBL, 0x2, ARRAY_SIZE(T_PANE_FIXED_NAME_TBL));
    this_->mLayout.PPaneRegister(P_PANE_NAME_DT, &this_->mpPicturePanes[dStockItem_c::P_iconKinoko_00], ARRAY_SIZE(P_PANE_NAME_DT));

    // New
    static const char *N_NEW_PANE_NAME_DT[] = {
        "N_iconKinoko_00", "N_iconFlower_00", "N_iconPro_00", "N_iconIce_00",
        "N_iconPen_00", "N_mameKinoko_00", "N_iconStar_00", "N_iconHammer_00"
    };

    static const char *P_NEW_PANE_NAME_DT[] = {
        "P_iconKinoko_00", "P_iconFlower_00", "P_iconPro_00", "P_iconIce_00",
        "P_iconPen_00", "P_mameKinoko_00", "P_iconStar_00", "P_iconHammer_00",
        "P_buttonBase_00", "P_buttonBase_01", "P_buttonBase_02", "P_buttonBase_06",
        "P_buttonBase_04", "P_buttonBase_05", "P_buttonBase_03", "P_buttonBase_07"
    };

    this_->mLayout.NPaneRegister(N_NEW_PANE_NAME_DT, &this_->mpNewNullPanes[dStockItem_c::N_iconKinoko_00], ARRAY_SIZE(N_NEW_PANE_NAME_DT));
    this_->mLayout.PPaneRegister(P_NEW_PANE_NAME_DT, &this_->mpNewPicturePanes[dStockItem_c::P_iconKinoko_00], ARRAY_SIZE(P_NEW_PANE_NAME_DT));
    return true;
}

// dStockItem_c::buttonBaseScaleSet()
kmBranchDefCpp(0x807AFAF0, NULL, void, dStockItem_c *this_) {
    // This seems to be genuinely useless? If the anim ID is not a dummy value,
    // and it is finished playing, then we check the scale of the Mushroom button
    // and set it to 1.0f if it isn't that value... but why? There is no visual
    // difference if we don't do this, so why even bother??

    if (this_->mAnimID != this_->mGroupNum && !this_->mLayout.isAnime(this_->mAnimID)) {
        this_->mAnimID = this_->mGroupNum;
        nw4r::lyt::Picture *P_buttonBase_00 = this_->mpNewPicturePanes[dStockItem_c::P_buttonBase_00];
        if ((P_buttonBase_00->GetScale().x != 1.0f) || (P_buttonBase_00->GetScale().y != 1.0f)) {
            const nw4r::math::VEC2 scale(1.0f, 1.0f);
            P_buttonBase_00->SetScale(scale);
        }
    }
}

// dStockItem_c::calcLayout3D()
kmBranchDefCpp(0x807AFBC0, NULL, void, dStockItem_c *this_) {
    // Players
    for (int i = 0; i < 4; i++) {
        if (this_->mPlayerActive[i]) {
            int paneIdx = this_->getPlyPosPane(i);
            if (paneIdx != 0x18) {
                nw4r::lyt::Pane *pos = this_->mpPicturePanes[paneIdx];
                const nw4r::math::MTX34 glbMtx = pos->GetGlobalMtx();
                mVec3_c panePos(glbMtx[0][3], glbMtx[1][3], 0.0f);

                mVec3_c plyPos;
                dGameCom::calcLyt3DPos(plyPos, panePos);

                da2DPlayer_c *player = this_->mpPlayers[daPyMng_c::mPlayerType[i]];
                player->mPos = plyPos;
                player->mSizeFactor = mVec3_c(0.89999998f, 0.89999998f, 0.89999998f);
                player->mPosOffsY = 26.0f;

                // Fix player scale
                if (dGameCom::GetAspectRatio() == 0) {
                    player->mSizeFactor *= 0.85;
                }
            }
        }
    }

    // Items
    for (int i = 0; i < NEW_ITEM_COUNT; i++) {
        nw4r::lyt::Pane *pos = this_->mpNewNullPanes[dStockItem_c::N_iconKinoko_00+i];
        const nw4r::math::MTX34 glbMtx = pos->GetGlobalMtx();
        mVec3_c panePos(glbMtx[0][3], glbMtx[1][3], 0.0f);

        mVec3_c itemPos;
        dGameCom::calcLyt3DPos(itemPos, panePos);

        daWmItem_c *item = this_->mpNewItems[i];
        item->mPos = itemPos;

        nw4r::lyt::Picture *button = this_->mpNewPicturePanes[dStockItem_c::P_buttonBase_00+i];
        item->mSizeFactor = mVec3_c(button->GetScale().x, button->GetScale().y, 1.0f);

        this_->mpNewItems[7]->mIsVisible = true;

        // Fix player scale
        if (dGameCom::GetAspectRatio() == 0) {
            item->mSizeFactor *= 0.85;
        }
    }

    // Position and scaling for the shadow layout
    nw4r::lyt::Pane *shadRoot = this_->mpShadow->mpRootPane;

    nw4r::math::MTX34 glbMtx = this_->mpNullPanes[dStockItem_c::N_stockItem]->GetGlobalMtx();
    mVec3_c panePos(glbMtx[0][3], glbMtx[1][3], 0.0f);
    shadRoot->SetTranslate(panePos);

    glbMtx = this_->mpNullPanes[dStockItem_c::N_stockItem_01]->GetGlobalMtx();
    mVec2_c shadScale(glbMtx[0][0], glbMtx[1][1]);
    shadRoot->SetScale(shadScale);

    // Scale buttons/item counts
    for (int i = 0; i < NEW_ITEM_COUNT; i++) {
        nw4r::lyt::Picture *button = this_->mpShadow->mpNewPicturePanes[dStockItemShadow_c::P_buttonBaseS_00+i];
        button->SetScale(this_->mpNewPicturePanes[dStockItem_c::P_buttonBase_00+i]->GetScale());
    }
}

// dStockItem_c::setShadowInfo()
kmBranchDefCpp(0x807B0340, NULL, void, dStockItem_c *this_, int count, int item) {
    this_->mpShadow->mDoSetInfo = true;
    this_->mpShadow->mItemCounts[item] = count;
}

// dStockItem_c::getNewItem()
kmBranchDefCpp(0x807B08E0, NULL, void, dStockItem_c *this_, int *newItem) {
    int idx;

    if (this_->mScrollDir == 1) {
        // Scrolling left
        while (*newItem > -1) {
            switch (*newItem) {
                default:
                    idx = *newItem - 1;
                    *newItem = idx;
                    break;
                case ITEM_STAR_POWER:
                    idx = ITEM_HAMMER_SUIT;
                    *newItem = ITEM_HAMMER_SUIT;
                    break;
                case ITEM_HAMMER_SUIT:
                    idx = ITEM_MINI_MUSHROOM;
                    *newItem = ITEM_MINI_MUSHROOM;
                    break;
            }
            if ((idx > -1) && (this_->mNewItemCounts[idx] != 0)) {
                return;
            }
        }
    } else {
        // Scrolling right
        while (*newItem < NEW_ITEM_COUNT) {
            switch (*newItem) {
                default:
                    idx = *newItem + 1;
                    *newItem = idx;
                    break;
                case ITEM_MINI_MUSHROOM:
                    idx = ITEM_HAMMER_SUIT;
                    *newItem = ITEM_HAMMER_SUIT;
                    break;
                case ITEM_HAMMER_SUIT:
                    idx = ITEM_STAR_POWER;
                    *newItem = ITEM_STAR_POWER;
                    break;
                case ITEM_STAR_POWER:
                    *newItem = this_->mCurrItem;
                    SndAudioMgr::sInstance->startSystemSe(SE_SYS_INVALID, 1);
                    return;
            }
            if ((idx < NEW_ITEM_COUNT) && (this_->mNewItemCounts[idx] != 0)) {
                return;
            }
        }
    }

    // No valid button to select
    *newItem = this_->mCurrItem;
    SndAudioMgr::sInstance->startSystemSe(SE_SYS_INVALID, 1);
}

// Use direct item type in chkUseItem()
// This skips indexing into a useless array
kmWrite32(0x807B06F0, 0x7CBB2B78); // or r27, r5, r5

// Check if the selected item is "valid" based on the current powerup
kmBranchDefCpp(0x800FC960, NULL, int, int item, int playerMode, bool *isStarPower) {
    // Might have this backwards? IDK
    bool isItemValid[NEW_ITEM_COUNT] = {
        false, false, false, false,
        false, false, false, false,
    };
    *isStarPower = false;
    int ret = playerMode;

    if (item == ITEM_STAR_POWER) {
        *isStarPower = true;
        for (int i = 0; i < NEW_ITEM_COUNT; i++) {
            isItemValid[i] = true;
        }
    } else if (item == ITEM_MUSHROOM) {
        isItemValid[0] = false;
        isItemValid[1] = true;
        isItemValid[2] = true;
        isItemValid[3] = false;
        isItemValid[4] = true;
        isItemValid[5] = true;
        isItemValid[6] = true;
        isItemValid[7] = true;
        ret = dStockItem_c::sc_itemPlyModeTbl[ITEM_MUSHROOM];
    } else {
        // Just do whatever we have selected
        ret = dStockItem_c::sc_itemPlyModeTbl[item];
        if (item == ITEM_HAMMER_SUIT) {
            isItemValid[7] = true;
        }
    }

    if (isItemValid[playerMode]) {
        ret = playerMode;
    }
    return ret;
}

// dStockItem_c::createButtonEffect()
kmBranchDefCpp(0x807B0470, NULL, void, dStockItem_c *this_) {
    nw4r::lyt::Pane *pane = this_->mpNewNullPanes[dStockItem_c::N_iconKinoko_00+this_->mCurrItem];
    const nw4r::math::MTX34 glbMtx = pane->GetGlobalMtx();
    mVec3_c panePos(glbMtx[0][3], glbMtx[1][3], 0.0f);

    mVec3_c effPos;
    dGameCom::calcLyt3DPos(effPos, panePos);

    dEf::createPlayerEffect(0, &this_->mButtonEffect, "Wm_2d_stockitem", 0, &effPos, nullptr, nullptr);
    this_->mEffectBtnIdx = this_->mCurrItem;
}

// dStockItem_c::calcButtonEffect()
kmBranchDefCpp(0x807B05B0, NULL, void, dStockItem_c *this_) {
    if (this_->mEffectBtnIdx != NEW_ITEM_COUNT) {
        nw4r::lyt::Pane *pane = this_->mpNewNullPanes[dStockItem_c::N_iconKinoko_00+this_->mCurrItem];
        const nw4r::math::MTX34 glbMtx = pane->GetGlobalMtx();
        mVec3_c panePos(glbMtx[0][3], glbMtx[1][3], 0.0f);

        mVec3_c effPos;
        dGameCom::calcLyt3DPos(effPos, panePos);

        this_->mButtonEffect.follow(&effPos, nullptr, nullptr);
    }
}

// dStockItem_c::setItem()
kmBranchDefCpp(0x807B00A0, NULL, void, dStockItem_c *this_) {
    bool doSubItem = false;
    int currItem = this_->mCurrItem;

    if (currItem == ITEM_STAR_POWER) {
        for (int i = 0; i < 4; i++) {
            int plyType = daPyMng_c::mPlayerType[i];
            if (((daPyMng_c::mCreateItem[plyType] & CREATE_ITEM_STAR_POWER) == 0) && this_->mPlayerActive) {
                if (!doSubItem) {
                    daWmPlayer_c::startStarSound();
                }

                daPyMng_c::mCreateItem[plyType] = CREATE_ITEM_STAR_POWER;
                da2DPlayer_c *player = this_->mpPlayers[plyType];
                player->mpMdlMng->mpMdl->onStarAnm();
                player->mpMdlMng->mpMdl->onStarEffect();
                this_->mPlyTypes[i] = (PLAYER_TYPE_e)plyType;
                doSubItem = true;

                mVec3_c effPos(player->mPos);
                this_->createPlyItemEffect(player->mParam & 0xF, &effPos, i);
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            int plyType = daPyMng_c::mPlayerType[i];
            bool isStarPower = (daPyMng_c::mCreateItem[plyType] & CREATE_ITEM_STAR_POWER) != 0;

            int newMode = checkValidStockItem(this_->mItemUsed, daPyMng_c::mPlayerMode[plyType], &isStarPower);
            if ((newMode != daPyMng_c::mPlayerMode[plyType]) && this_->mPlayerActive[i]) {
                daPyMng_c::mPlayerMode[plyType] = (PLAYER_POWERUP_e)newMode;
                doSubItem = true;

                da2DPlayer_c *player = this_->mpPlayers[plyType];
                player->setPlayerMode(dStockItem_c::sc_itemPlyModeTbl[currItem]);
                this_->mPlyTypes[i] = (PLAYER_TYPE_e)plyType;

                mVec3_c effPos(player->mPos);
                this_->createPlyItemEffect(player->mParam & 0xF, &effPos, i);
            }
        }
    }

    if (doSubItem) {
        dInfo_c::m_instance->subStockItem(currItem);
        this_->mNewItemCounts[currItem] -= 1;
        this_->setShadowInfo(this_->mNewItemCounts[currItem], currItem);
        if (this_->mNewItemCounts[currItem] == 0) {
            this_->mpNewItems[currItem]->mNextAnm = 1;
        }
    }
}

// dStockItem_c::chkSetItem()
kmBranchDefCpp(0x807B06C0, NULL, bool, dStockItem_c *this_) {
    this_->mItemUsed = this_->mCurrItem;

    if (this_->mCurrItem == ITEM_STAR_POWER) {
        bool isMiniMushroom = false;
        if (this_->mPlayerActive[0] && (daPyMng_c::mPlayerMode[daPyMng_c::mPlayerType[0]] == POWERUP_MINI_MUSHROOM)) {
            isMiniMushroom = true;
        }

        for (int i = 0; i < 4; i++) {
            int plyType = daPyMng_c::mPlayerType[i];
            if (((daPyMng_c::mCreateItem[plyType] & CREATE_ITEM_STAR_POWER) == 0) && this_->mPlayerActive[i]) {
                SndAudioMgr::sInstance->startSystemSe((isMiniMushroom) ? SE_PLY_CS_CHANGE_MAME_STAR : SE_PLY_CS_CHANGE_STAR, 1);
                return true;
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            int plyType = daPyMng_c::mPlayerType[i];
            bool isStarPower = (daPyMng_c::mCreateItem[plyType] & CREATE_ITEM_STAR_POWER) != 0;

            int newMode = checkValidStockItem(this_->mItemUsed, daPyMng_c::mPlayerMode[plyType], &isStarPower);
            if ((newMode != daPyMng_c::mPlayerMode[plyType]) && this_->mPlayerActive[i]) {
                int soundID;
                switch (this_->mItemUsed) {
                    default:
                        soundID = SE_PLY_CS_CHANGE_BIG;
                        break;
                    case 2:
                        soundID = SE_PLY_CS_CHANGE_PRPL;
                        break;
                    case 4:
                        soundID = SE_PLY_CS_CHANGE_PNGN;
                        break;
                    case 5:
                        soundID = SE_PLY_CS_CHANGE_MAME;
                        break;
                }

                SndAudioMgr::sInstance->startSystemSe(soundID, 1);
                return true;
            }
        }
    }
    return false;
}

// dStockItem_c::initializeState_WindowOpenAnimeEndWait()
kmBranchDefCpp(0x807B0AF0, NULL, void, dStockItem_c *this_) {
    this_->mLayout.AllAnimeEndSetup();
    // Reset onButton anims
    for (int i = 0; i < ARRAY_SIZE(dStockItem_c::sc_onBtnAnimeIdxTbl); i++) {
        this_->mLayout.ReverseAnimeStartSetup(dStockItem_c::sc_onBtnAnimeIdxTbl[i], false);
    }

    this_->mLayout.AnimeStartSetup(0, false);
    this_->mpRootPane->SetVisible(true);

    // Init items
    for (int i = 0; i < NEW_ITEM_COUNT; i++) {
        daWmItem_c *item = this_->mpNewItems[i];
        item->mIsVisible = true;
        item->calcMdl();
        // 0 is idle, 1 is disabled
        item->mNextAnm = (this_->mNewItemCounts[i] == 0) ? 1 : 0;
    }
    SndSceneMgr::sInstance->openWindow(4);
}

// dStockItem_c::executeState_WindowOpenAnimeEndWait()
kmBranchDefCpp(0x807B0BE0, NULL, void, dStockItem_c *this_) {
    if (this_->mLayout.isAnime(-1)) {
        return;
    }

    // Try to select the last used item in the menu
    int prevItem = dInfo_c::m_instance->mPrevStockItem;

    // Check validity of previous item
    if ((prevItem < 0) || this_->mNewItemCounts[prevItem] == 0) {
        this_->mCurrItem = -1;

        // Check each item to find the first valid one
        for (int i = 0; i < NEW_ITEM_COUNT; i++) {
            if (this_->mNewItemCounts[i] != 0) {
                this_->mCurrItem = i;
                break;
            }
        }

        // Do we have any items?
        if (this_->mCurrItem < 0) {
            this_->mStateMgr.changeState(dStockItem_c::StateID_ItemSelectWait);
        } else {
            this_->mStateMgr.changeState(dStockItem_c::StateID_ButtonChangeAnimeEndWait);
        }
    } else {
        // Just go with that then
        this_->mCurrItem = prevItem;
        this_->mStateMgr.changeState(dStockItem_c::StateID_ButtonChangeAnimeEndWait);
    }
}

// dStockItem_c::initializeState_ButtonChangeAnimeEndWait()
kmBranchDefCpp(0x807B0D90, NULL, void, dStockItem_c *this_) {
    if (this_->mPrevItem > -1) {
        this_->mLayout.AnimeStartSetup(dStockItem_c::sc_offBtnAnimeIdxTbl[this_->mPrevItem], false);
        SndAudioMgr::sInstance->startSystemSe(SE_SYS_CURSOR, 1);
        dSelectCursor_c::m_instance->Cancel(0);
        if (this_->mPrevItem = ITEM_MUSHROOM) {
            this_->mAnimID = 22;
        }
    }

    this_->mLayout.AnimeEndSetup(dStockItem_c::sc_offBtnAnimeIdxTbl[this_->mCurrItem]);
    this_->mLayout.AnimeStartSetup(dStockItem_c::sc_onBtnAnimeIdxTbl[this_->mCurrItem], false);
}

// dStockItem_c::finalizeState_ButtonChangeAnimeEndWait()
kmBranchDefCpp(0x807B0EC0, NULL, void, dStockItem_c *this_) {
    nw4r::lyt::Pane *target = this_->mpNewPicturePanes[dStockItem_c::P_buttonBase_00+this_->mCurrItem];
    dGameCom::SelectCursorSetup(target, 0, false);
}

// dStockItem_c::finalizeState_WindowCloseAnimeEndWait
kmBranchDefCpp(0x807B13C0, NULL, void, dStockItem_c *this_) {
    for (int i = 0; i < 4; i++) {
        if (this_->mPlayerActive[i]) {
            this_->mpPlayers[daPyMng_c::mPlayerType[i]]->mDoReset = true;
        }
    }

    // Hide items
    for (int i = 0; i < NEW_ITEM_COUNT; i++) {
        daWmItem_c *item = this_->mpNewItems[i];
        item->mIsVisible = false;
        item->calcMdl();
    }
    this_->mpShadow->mIsVisible = false;
    SndSceneMgr::sInstance->closeWindow();
}

// dStockItem_c::initializeState_HitAnimeEndWait()
kmBranchDefCpp(0x807B14D0, NULL, void, dStockItem_c *this_) {
    this_->setItem();
    this_->createButtonEffect();
    this_->mLayout.AnimeStartSetup(dStockItem_c::sc_hitBtnAnimeIdxTbl[this_->mCurrItem], false);
}
