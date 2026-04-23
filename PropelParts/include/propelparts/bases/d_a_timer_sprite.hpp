#pragma once
#include <game/bases/d_actor.hpp>

class daTimerSprite_c : public dActor_c {
public:
    daTimerSprite_c() {}
    ~daTimerSprite_c() {}

    virtual int create();
    virtual int execute();
    virtual int draw() { return 1; }
    virtual int doDelete() { return 1; }

    void doAddTime();

    u8 mEventID;
};