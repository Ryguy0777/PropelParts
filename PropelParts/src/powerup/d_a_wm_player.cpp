#include <kamek.h>
#include <game/bases/d_a_wm_player.hpp>

const mVec3_c sc_wmplayer_shadow_scales[] = {
    mVec3_c(0.6f, 1.0f, 0.6f), // Small
    mVec3_c(0.8f, 1.0f, 0.8f), // Super
    mVec3_c(0.8f, 1.0f, 0.8f), // Fire
    mVec3_c(0.4f, 1.0f, 0.4f), // Mini
    mVec3_c(0.8f, 1.0f, 0.8f), // Propeller
    mVec3_c(0.8f, 1.0f, 0.8f), // Penguin
    mVec3_c(0.8f, 1.0f, 0.8f), // Ice
    mVec3_c(0.8f, 1.0f, 0.8f), // Hammer
};

kmBranchDefCpp(0x809032c0, NULL, void, daWmPlayer_c *this_) {
    u32 powerup = this_->mPyMdlMng.mpMdl->mPowerupID;
    this_->CalcShadow(0.5f,
        sc_wmplayer_shadow_scales[powerup].x, sc_wmplayer_shadow_scales[powerup].y, sc_wmplayer_shadow_scales[powerup].z
    );
    this_->mPyMdlMng.calc(this_->mPos, this_->mAngle, this_->mScale);
    this_->mPyMdlMng.play();
}