#pragma once
#include <game/bases/d_base_actor.hpp>
#include <game/bases/d_player_model_manager.hpp>

/// @unofficial
class da2DPlayer_c : public dBaseActor_c {
public:
    void setPlayerMode(int);

    dPyMdlMng_c *mpMdlMng;
    u32 mPad[0x3D];
    mVec3_c mSizeFactor; // Actual size is this * 4.1f
    u32 mPad2[0xC];
    float mPosOffsY;
    u8 mPad3[9];
    bool mDoReset;
};