#pragma once

#include <game/bases/d_a_bullet.hpp>

class daHammer_c : public daBullet_c {
public:
    virtual int draw();

    virtual ~daHammer_c();

    virtual void finalUpdate();

    virtual void createMdl();

    virtual void initialize();
    virtual void setSpitOutMove();
    virtual bool cullingProc();
    virtual void revengeHitSE();
    virtual void spitRoll();
    virtual void moveSE();

    nw4r::g3d::ResFile mRes;
    m3d::smdl_c mModel;
    mVec3_c m_10;
    u32 m_1c;
    u32 m_20;
    u32 m_24;
    u16 m_28;
    sCcDatNewF mCollInfo;
    u32 m_50;
    u32 mPad2;

    static const float smc_THROW_SPEED_MAX[2];
    static const short smc_THROW_ANGLE[2];
    static const float smc_THROW_SPEED[2];
};