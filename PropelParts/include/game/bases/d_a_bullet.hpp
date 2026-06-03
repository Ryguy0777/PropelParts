#pragma once

#include <game/bases/d_actor_state.hpp>
#include <game/bases/d_heap_allocator.hpp>

class daBullet_c : public dActorState_c {
public:
    virtual int create();
    virtual int doDelete();
    virtual int execute();
    virtual int preExecute();
    virtual int draw();
    virtual void deleteReady();

    virtual ~daBullet_c();

    virtual void setEatTongue(dActor_c *eatingActor);
    virtual bool setEatSpitOut(dActor_c *eatingActor);
    virtual void waterSplashEffect(const mVec3_c &pos, float size);

    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, EatIn);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, EatNow);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, SpiteMove);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, Reflect);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, HitReflect);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, HitStar);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, HitShell);
    STATE_VIRTUAL_FUNC_DECLARE(daBullet_c, HitYoshiBullet);

    virtual void createMdl();
    virtual void removeMdl();
    virtual void initialize();
    virtual void setSpitOutMove(dActor_c *);
    virtual void setDeadMove(const mVec3_c&, short);
    virtual bool cullingProc();
    virtual bool hitProc_Star(dCc_c*);
    virtual bool hitProc_Shell(dCc_c*);
    virtual bool hitProc_YoshiBullet(dCc_c*);
    virtual bool hitProc_Reflect(dCc_c*);
    virtual void revengeHitSE();
    virtual void spitRoll();
    virtual void moveSE();
    virtual void deadRoll();

    dHeapAllocator_c mAllocator;
    u32 m_04;
    u32 mThrownDirection;
    u32 m_0c;
    mAng3_c mDeadrollDelta;
    u16 mPad;
};
