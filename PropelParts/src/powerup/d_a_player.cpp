#include <kamek.h>
#include <game/bases/d_a_player.hpp>
#include <game/bases/d_s_stage.hpp>
#include <constants/sound_list.h>
#include <game/bases/d_a_fireball_player.hpp>
#include <game/bases/d_a_iceball.hpp>

// Throw a hammer if the player has a hammer suit
kmBranchDefCpp(0x8013bcd0, NULL, void, dAcPy_c *this_, int i) {
    mMtx_c mtx;
    mVec3_c pos;
    mVec3_c loopPos;
    mVec3_c pos2;
    this_->mPyMdlMng.mpMdl->getJointMtx(&mtx, 11);
    mtx.concat(mMtx_c::createTrans(-1.0f, 4.0f, 4.0f));
    mtx.multVecZero(pos);
    this_->mPyMdlMng.mpMdl->getJointMtx(&mtx, 0);
    mtx.multVecZero(pos2);
    mVec3_c diff = pos - pos2;
    float dist = diff.xzLen();
    float dirSpeed = daPlBase_c::sc_DirSpeed[this_->m_12f4];
    loopPos.set(
        dScStage_c::getLoopPosX(pos2.x + dirSpeed * dist),
        pos.y,
        this_->mPos.z
    );
    if (this_->mPowerup == POWERUP_FIRE_FLOWER) {
        this_->startSound(SE_PLY_THROW_FIRE, false);
        pos = loopPos;
        u32 param = (i << 16) | (this_->mAmiLayer << 12) | (this_->mLayer << 8) | (this_->m_12f4 << 4) | this_->mPlayerNo;
        dActor_c::construct(fProfile::PL_FIREBALL, param, &pos, nullptr, 0);
    } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
        this_->startSound(SE_PLY_THROW_ICEBALL, false);
        pos = loopPos;
        u32 param = (i << 16) | (this_->mAmiLayer << 12) | (this_->mLayer << 8) | (this_->m_12f4 << 4) | this_->mPlayerNo;
        dActor_c::construct(fProfile::ICEBALL, param, &pos, nullptr, 0);
    } else if (this_->mPowerup == POWERUP_HAMMER_SUIT) {
        this_->startSound(SE_PLY_THROW_FIRE, false);
        pos = loopPos;
        u32 param = (i << 16) | (this_->mAmiLayer << 12) | (this_->mLayer << 8) | (this_->m_12f4 << 4) | this_->mPlayerNo;
        dActor_c *hammer = dActor_c::construct(fProfile::HAMMER, param, &pos, nullptr, 0);
        hammer->setEatSpitOut(this_);
        hammer->mScale.set(1.0f, 1.0f, 1.0f);
        hammer->mCc.mCcData.mVsKind = 8;
    }
}

// Limit thrown hammers
extern bool daHammer_c__CheckHammerLimit(int playerNo, int limitMode);

kmBranchDefCpp(0x8013bb00, NULL, bool, dAcPy_c *this_) {
    if (this_->mKey.triggerFire()) {
        if (this_->isCarry()) {
            return false;
        }
        if (this_->mPowerup == POWERUP_FIRE_FLOWER) {
            if (daFireBall_Player_c::CheckFireBallLimit(this_->mPlayerNo, 0)) {
                return true;
            }
        } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
            if (daIceBall_c::CheckIceballLimit(this_->mPlayerNo, 0)) {
                return true;
            }
        } else if (this_->mPowerup == POWERUP_HAMMER_SUIT) {
            if (daHammer_c__CheckHammerLimit(this_->mPlayerNo, 0)) {
                return true;
            }
        }
    }
    return false;
}

kmBranchDefCpp(0x8013c2f0, NULL, void, dAcPy_c *this_) {
    if (this_->isCarry()) {
        dAcPy_c *carryPlayer = this_->getCarryPlayer();
        if (carryPlayer != nullptr) {
            carryPlayer->m_12f4 = this_->m_12f4;
            carryPlayer->setSpinFireBall();
        }
    } else if (this_->mPowerup == POWERUP_FIRE_FLOWER) {
        if (daFireBall_Player_c::CheckFireBallLimit(this_->mPlayerNo, 1)) {
            this_->createFireBall(1);
        }
    } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
        if (daIceBall_c::CheckIceballLimit(this_->mPlayerNo, 1)) {
            this_->createFireBall(1);
        }
    } else if (this_->mPowerup == POWERUP_HAMMER_SUIT) {
        if (daHammer_c__CheckHammerLimit(this_->mPlayerNo, 1)) {
            this_->createFireBall(1);
        }
    }
}

// Here are the hacks that fix arrays indexed by powerup type

extern sBcPlayerPointData scBgPointData_Normal_[5]; // Two symbols have the name scBgPointData_Normal for some reason, I add _ to differentiate them
extern sBcPlayerPointData scBgPointData_Super[5];
extern sBcPlayerPointData scBgPointData_Mame[5];

const sBcPlayerPointData *scBgPointDataNew[NEW_POWERUP_COUNT] = {
    scBgPointData_Normal_,  // Small
    scBgPointData_Super,    // Super
    scBgPointData_Super,    // Fire
    scBgPointData_Mame,     // Mini
    scBgPointData_Super,    // Propeller
    scBgPointData_Super,    // Penguin
    scBgPointData_Super,    // Ice
    scBgPointData_Super,    // Hammer
};

kmBranchDefCpp(0x80142650, NULL, const sBcPlayerPointData *, dAcPy_c *this_, PLAYER_POWERUP_e powerup, int index) {
    return &scBgPointDataNew[powerup][index];
}

kmBranchDefCpp(0x80132670, NULL, float, dAcPy_c *this_) {
    static const float scHangOffsetY[] = {
        16.0f, 27.0f, 27.0f, 12.0f, 27.0f, 27.0f, 27.0f, 27.0f
    };
    static const float scKinopioHangOffsetY[] = {
        13.0f, 21.0f, 21.0f, 9.0f, 21.0f, 21.0f, 21.0f, 21.0f
    };
    // [probably some inline?]
    if (!(this_->mPlayerType != PLAYER_MARIO && this_->mPlayerType != PLAYER_LUIGI)) {
        return scHangOffsetY[this_->mPowerup];
    } else {
        return scKinopioHangOffsetY[this_->mPowerup];
    }
}

extern const sCcDatNew *l_cc_size_data[11];

enum CcSizeDataIndex_e {
    CC_SIZE_DATA_MAME,
    CC_SIZE_DATA_NORMAL,
    CC_SIZE_DATA_SUPER,
    CC_SIZE_DATA_PROPEL,
    CC_SIZE_DATA_NORMAL_SIT,
    CC_SIZE_DATA_SUPER_SIT,
    CC_SIZE_DATA_NORMAL_SWIM,
    CC_SIZE_DATA_SUPER_SWIM,
    CC_SIZE_DATA_NORMAL_WATERWALK,
    CC_SIZE_DATA_SUPER_WATERWALK,
    CC_SIZE_DATA_PENGUIN_SLID
};

const CcSizeDataIndex_e l_mode_cc_size_data_new[] = {
    CC_SIZE_DATA_NORMAL, CC_SIZE_DATA_SUPER, CC_SIZE_DATA_SUPER, CC_SIZE_DATA_MAME, CC_SIZE_DATA_PROPEL, CC_SIZE_DATA_SUPER, CC_SIZE_DATA_SUPER, CC_SIZE_DATA_SUPER
};

kmBranchDefCpp(0x80141F70, NULL, void, dAcPy_c *this_) {
    const sCcDatNew *ccData = l_cc_size_data[l_mode_cc_size_data_new[this_->mPowerup]];
    this_->m_0c.x = ccData->mOffset.x;
    this_->m_0c.y = ccData->mOffset.y + this_->m_1598 + this_->m_159c;
    this_->mCc.mCcData.mBase.mOffset.set(this_->m_0c.x, this_->m_0c.y);
    this_->mCc.mCcData.mBase.mSize.set(ccData->mSize.x, ccData->mSize.y);
}

kmBranchDefCpp(0x801466C0, NULL, u8, dAcPy_c *this_, s8 powerup) {
    const u8 l_tall_type[] = {
        1, 2, 2, 0, 2, 2, 2, 2
    };

    if (powerup == -1) {
        powerup = this_->mPowerup;
    }
    return l_tall_type[powerup];
}

kmBranchDefCpp(0x80142170, NULL, void, dAcPy_c *this_) {
    int sizeIndex = l_mode_cc_size_data_new[this_->mPowerup];
    if (this_->mPyMdlMng.getFlags() & 1) {
        switch (this_->getPowerup()) {
            case POWERUP_MINI_MUSHROOM:
                break;
            case POWERUP_NONE:
                sizeIndex = CC_SIZE_DATA_NORMAL_SIT;
                break;
            default:
                sizeIndex = CC_SIZE_DATA_SUPER_SIT;
                break;
        }
    } else if (this_->mPyMdlMng.getFlags() & 0x10) {
        switch (this_->getPowerup()) {
            case POWERUP_MINI_MUSHROOM:
                break;
            case POWERUP_NONE:
                sizeIndex = CC_SIZE_DATA_NORMAL_SWIM;
                break;
            default:
                sizeIndex = CC_SIZE_DATA_SUPER_SWIM;
                break;
        }
    } else if (this_->isStatus(dAcPy_c::STATUS_PENGUIN_SLIDE)) {
        sizeIndex = CC_SIZE_DATA_PENGUIN_SLID;
    } else if (this_->isStatus(dAcPy_c::STATUS_SWIM)) {
        switch (this_->getPowerup()) {
            case POWERUP_MINI_MUSHROOM:
                break;
            case POWERUP_NONE:
                sizeIndex = CC_SIZE_DATA_NORMAL_WATERWALK;
                break;
            default:
                sizeIndex = CC_SIZE_DATA_SUPER_WATERWALK;
                break;
        }
    }

    if (this_->isStatus(dAcPy_c::STATUS_PENGUIN_SWIM)) {
        this_->onStatus(dAcPy_c::STATUS_78);
        mMtx_c mtx;
        mVec3_c pos;
        this_->mPyMdlMng.mpMdl->getJointMtx(&mtx, 1);
        mtx.concat(mMtx_c::createTrans(2.0f, 0.0f, 0.0f));
        mtx.multVecZero(pos);
        this_->mCc.mCcData.mBase.mSize.set(6.0f, 6.0f);
        this_->mCc.mCcData.mBase.mOffset.set(pos.x - this_->mPos.x, pos.y - this_->mPos.y);
        this_->mPyMdlMng.mpMdl->getJointMtx(&mtx, this_->mPyMdlMng.mpMdl->getFaceJointIdx());
        mtx.concat(mMtx_c::createTrans(0.0f, -4.0f, 0.0f));
        mtx.multVecZero(pos);
        this_->mCc1.mCcData.mBase.mSize.set(6.0f, 6.0f);
        this_->mCc1.mCcData.mBase.mOffset.set(pos.x - this_->mPos.x, pos.y - this_->mPos.y);
        return;
    }

    const sCcDatNew *ccData = l_cc_size_data[sizeIndex];
    float x = ccData->mOffset.x;
    if (this_->mDirection == DIR_LR_L) {
        x = -x;
    }
    float sx = ccData->mSize.x;
    float sy = ccData->mSize.y;
    this_->m_0c.set(x, ccData->mOffset.y + this_->m_1598 + this_->m_159c);
    this_->mCc.mCcData.mBase.mSize.set(sx, sy);
    this_->mCc.mCcData.mBase.mOffset.setX(this_->m_0c.x);
    if (this_->mPyMdlMng.mpMdl->mFlags & 4) {
        mMtx_c mtx;
        mVec3_c pos;
        mVec3_c anklePos = this_->getAnkleCenterPos();
        float tmp = anklePos.y - 2.0f;
        if (tmp > this_->mPos.y) {
            tmp = this_->mPos.y;
        }
        this_->mPyMdlMng.mpMdl->getJointMtx(&mtx, this_->mPyMdlMng.mpMdl->getFaceJointIdx());
        mtx.concat(mMtx_c::createTrans(0.0f, -4.0f, 0.0f));
        mtx.multVecZero(pos);
        float tmp1 = (pos.y + tmp) / 2.0f;
        this_->mCc.mCcData.mBase.mOffset.y = tmp1 - this_->mPos.y;
        float tmp2 = (pos.y - tmp) / 2.0f;
        this_->mCc.mCcData.mBase.mSize.y = std::fabs(tmp2);
    } else {
        this_->mCc.mCcData.mBase.mOffset.y = this_->m_0c.y;
    }
    dAcPy_c *ridePlayer = this_->getRidePlayer();
    if (ridePlayer != nullptr && ridePlayer->mPowerup == POWERUP_NONE) {
        if (sizeIndex == 2) {
            this_->mCc.mCcData.mBase.mOffset.y = 16.0f;
            this_->mCc.mCcData.mBase.mSize.y = 8.0f;
            this_->mCc.mCcData.mBase.mSize.x = 4.0f;
        } else if (sizeIndex == 1) {
            this_->mCc.mCcData.mBase.mOffset.y = 9.0f;
            this_->mCc.mCcData.mBase.mSize.y = 5.0f;
            this_->mCc.mCcData.mBase.mSize.x = 4.0f;
        }
    }
    if (this_->isStatus(dAcPy_c::STATUS_8E) || this_->isStatus(dAcPy_c::STATUS_4F)) {
        this_->mCc.mCcData.mVsKind &= ~(BIT_FLAG(CC_KIND_YOSHI) | BIT_FLAG(CC_KIND_PLAYER) | BIT_FLAG(CC_KIND_PLAYER_ATTACK));
    } else {
        this_->mCc.mCcData.mVsKind |= (BIT_FLAG(CC_KIND_YOSHI) | BIT_FLAG(CC_KIND_PLAYER) | BIT_FLAG(CC_KIND_PLAYER_ATTACK));
    }
}

kmBranchDefCpp(0x80146710, NULL, void, dAcPy_c *this_) {
    float l_powerup_offsets[] = {
        16.0f, 31.0f, 31.0f, 6.0f, 34.0f, 31.0f, 31.0f, 31.0f
    };

    this_->mModelHeight = l_powerup_offsets[this_->mPowerup];

    mVec3_c offset(0.0f, 0.0f, 0.0f);
    if (this_->mPyMdlMng.mpMdl->mFlags & 1) {
        offset.y = this_->mModelHeight / 4.0f;
    } else if (this_->mPyMdlMng.mpMdl->mFlags & 4) {
        offset.y = -(this_->mModelHeight / 2.0f);
    } else {
        offset.y = this_->mModelHeight / 2.0f;
    }
    this_->mCenterOffs = offset;

    float target = 16.0f;
    if (this_->mPowerup == POWERUP_MINI_MUSHROOM) {
        target = 8.0f;
    }
    sLib::chase(&this_->mViewLimitPadding, target, 0.1f);
}

kmBranchDefCpp(0x80133DF0, NULL, void, dAcPy_c *this_) {
    this_->onStatus(dAcPy_c::STATUS_HANG);
    this_->mAccelY = 0.0f;
    this_->mSpeedF = 0.0f;
    this_->mMaxSpeedF = 0.0f;
    this_->mSpeed.x = 0.0f;
    this_->mSpeed.y = 0.0f;
    this_->m_1598 = 0.0f;
    if (this_->mPlayerType == PLAYER_YELLOW_TOAD || this_->mPlayerType == PLAYER_BLUE_TOAD) {
        static const float scHangOffsetY[NEW_POWERUP_COUNT] = { 4.5f, 10.0f, 10.0f, 1.0f, 10.0f, 9.0f, 10.0f, 10.0f };
        this_->m_1598 = scHangOffsetY[this_->mPowerup];
    }
    this_->mPyMdlMng.setAnm(PLAYER_ANIM_MONKEY_START);
    this_->setZPositionDirect(400.0f);
    this_->startPlayerVoice(VOICE_ROPE_CATCH, 0);
}

kmBranchDefCpp(0x801387F0, NULL, void, dAcPy_c *this_, mVec3_c &pos) {
    if (!this_->isStatus(dAcPy_c::STATUS_53)) {
        return;
    }
    this_->onStatus(dAcPy_c::STATUS_54);
    static const float scBalloonOffsetY[] = {
        0.0f, -4.0f, -4.0f, 4.0f, -6.0f, -2.0f, -4.0f, -4.0f
    };
    pos.y += scBalloonOffsetY[this_->mPowerup];
    this_->mPos = this_->mLastPos = pos;
}

kmBranchDefCpp(0x80146840, NULL, void, dAcPy_c *this_) {
    static const int scSpeedHioData[] = { 1, 1, 1, 0, 1, 1, 1, 1 };
    u8 index = scSpeedHioData[this_->mPowerup];
    this_->mSpeedDataNormal = &dAcPy_c::m_speed_hio[index].mDataNormal;
    this_->mSpeedDataStar = &dAcPy_c::m_speed_hio[index].mDataStar;
}

// Powerup scale fixes, see d_player_model_manager.cpp and d_a_player_base.cpp for the rest

extern float PowerupScales[NEW_POWERUP_COUNT+2];

kmCallDefAsm(0x801293D0) {
    lis r6, PowerupScales@h
	ori r6, r6, PowerupScales@l
	add r3, r6, r0
	blr
}

kmCallDefAsm(0x80137CDC) {
    lis r6, PowerupScales@h
	ori r6, r6, PowerupScales@l
	add r3, r6, r0
	blr
}

kmBranchDefAsm(0x80141FF4, 0x80141FF8) {
    lis r5, PowerupScales@h
	ori r5, r5, PowerupScales@l
	add r4, r5, r4
	blr
}

kmCallDefAsm(0x8014378C) {
    lis r6, PowerupScales@h
	ori r6, r6, PowerupScales@l
	add r3, r6, r0
	blr
}

kmCallDefAsm(0x80143884) {
    lis r8, PowerupScales@h
	ori r8, r8, PowerupScales@l
	add r7, r8, r0
	blr
}

// Fix "Powerup get" sound
kmBranchDefCpp(0x80141280, NULL, void, dAcPy_c *this_) {
    this_->mChangeTimer = 30;
    int prevTallType = this_->getTallType(this_->mPowerupCopy);
    int currTallType = this_->getTallType(-1);
    if (this_->isStatus(dAcPy_c::STATUS_HANG)) {
        static const float offsets[3] = { 21.0f, 11.0f, 0.0f };
        this_->mPos.y += offsets[currTallType] - offsets[prevTallType];
    }
    static const int tallTypeChange[3][3] = {
        {7, 7, 1},
        {2, 7, 3},
        {4, 5, 6}
    };
    this_->m_67 = tallTypeChange[prevTallType][currTallType];
    switch (this_->m_67) {
        case 0:
        case 1:
        case 3:
            if (this_->mPowerup == POWERUP_PROPELLER_SHROOM) {
                this_->startSound(SE_PLY_CHANGE_PRPL, false);
            } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT) {
                this_->startSound(SE_PLY_CHANGE_PNGN, false);
            } else {
                this_->startSound(SE_PLY_CHANGE_BIG, false);
            }
            break;
        case 2:
        case 4:
            this_->startSound(SE_PLY_CHANGE_MAME, false);
            break;
        case 5:
        case 6:
            if (this_->mPowerup == POWERUP_PROPELLER_SHROOM) {
                this_->startSound(SE_PLY_CHANGE_PRPL, false);
            } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT) {
                this_->startSound(SE_PLY_CHANGE_PNGN, false);
            } else if (this_->mPowerup == POWERUP_FIRE_FLOWER || this_->mPowerup == POWERUP_ICE_FLOWER) {
                this_->startSound(SE_PLY_CHANGE_BIG, false);
            } else if (this_->mPowerup == POWERUP_HAMMER_SUIT) {
                this_->startSound(SE_PLY_CHANGE_BIG, false);
            } else {
                this_->startSound(SE_PLY_CHANGE_SMALL, false);
            }
            break;
    }
}