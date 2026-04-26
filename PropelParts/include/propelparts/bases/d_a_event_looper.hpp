#pragma once
#include <game/bases/d_actor.hpp>

class daEventLooper_c : public dActor_c {
public:
    daEventLooper_c();
    ~daEventLooper_c();

    virtual int create();
    virtual int execute();
    virtual int draw();
    virtual int doDelete();

    enum State {
        STATE_WAITING,
        STATE_RUNNING,
        STATE_FINISHED
    };

    u8 mTriggeringEvent;
    u8 mStartEvent;
    u8 mTargetEventStart;
    u8 mTargetEventEnd;
    u8 mDelay;
    State mState;
    u8 mCurrentEvent;
    u8 mLastEvent;
    u8 mDelayCounter;
};