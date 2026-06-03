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
    /* } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
        this_->startSound(SE_PLY_THROW_ICEBALL, false);
        pos = loopPos;
        u32 param = (i << 16) | (this_->mAmiLayer << 12) | (this_->mLayer << 8) | (this_->m_12f4 << 4) | this_->mPlayerNo;
        dActor_c::construct(fProfile::ICEBALL, param, &pos, nullptr, 0);
    } */
    } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
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
        /* } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
            if (daIceBall_c::CheckIceballLimit(this_->mPlayerNo, 0)) {
                return true;
            }
        } */
        } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
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
    /* } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
        if (daIceBall_c::CheckIceballLimit(this_->mPlayerNo, 1)) {
            this_->createFireBall(1);
        }
    } */
    } else if (this_->mPowerup == POWERUP_PENGUIN_SUIT || this_->mPowerup == POWERUP_ICE_FLOWER) {
        if (daHammer_c__CheckHammerLimit(this_->mPlayerNo, 1)) {
            this_->createFireBall(1);
        }
    }
}