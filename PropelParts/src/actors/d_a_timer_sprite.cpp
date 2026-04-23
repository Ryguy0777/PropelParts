#include <kamek.h>
#include <propelparts/bases/d_a_timer_sprite.hpp>
#include <propelparts/bases/d_custom_profile.hpp>
#include <game/bases/d_switch_flag_mng.hpp>

CUSTOM_ACTOR_PROFILE(EN_TIMER_SPRITE, daTimerSprite_c, fProfile::EN_STAR_COIN, fProfile::DRAW_ORDER::EN_STAR_COIN, 0);

const dActorData_c c_TIMER_SPRITE_actor_data = {fProfile::EN_TIMER_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ACTOR_CREATE_GLOBAL};
dCustomProfile_c l_TIMER_SPRITE_profile(&g_profile_EN_TIMER_SPRITE, "EN_TIMER_SPRITE", CourseActor::EN_TIMER_SPRITE, &c_TIMER_SPRITE_actor_data, NULL);

int daTimerSprite_c::create() {
    return 1;
}

void daTimerSprite_c::doAddTime() {
    u32 *timer = (u32*)(*(u32*)0x8042A350);
    if (timer != nullptr) {
        u32 *time = (u32*)((u32)timer + 4);
        *time += (50 << 12);
    }
}

int daTimerSprite_c::execute() {
    u8 eventID = mEventNums[0];
    
    if (eventID == 0) {
        doAddTime();
        deleteActor(1);
        return 1;
    }

    if (dSwitchFlagMng_c::checkEvent(eventID - 1)) {
        doAddTime();
        deleteActor(1);
    }
    
    return 1;
}