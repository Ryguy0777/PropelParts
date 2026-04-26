#include <kamek.h>
#include <propelparts/bases/d_a_event_looper.hpp>
#include <propelparts/bases/d_custom_profile.hpp>
#include <game/bases/d_switch_flag_mng.hpp>

CUSTOM_ACTOR_PROFILE(AC_EVENT_LOOPER, daEventLooper_c, fProfile::AC_FLAGON, fProfile::DRAW_ORDER::AC_FLAGON, 0);

const dActorData_c c_EVENT_LOOPER_actor_data = {
    fProfile::AC_EVENT_LOOPER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ACTOR_CREATE_GLOBAL
};
dCustomProfile_c l_EVENT_LOOPER_profile(
    &g_profile_AC_EVENT_LOOPER, "AC_EVENT_LOOPER",
    CourseActor::AC_EVENT_LOOPER, &c_EVENT_LOOPER_actor_data, NULL
);

daEventLooper_c::daEventLooper_c() : mState(STATE_WAITING), mCurrentEvent(0), mDelayCounter(0) {}
daEventLooper_c::~daEventLooper_c() {}

int daEventLooper_c::create() {
    mDelay = (u8)((mParam >> 8) & 0xFF);
    if (mDelay == 0)
        mDelay = 30;

    mState = STATE_WAITING;
    mCurrentEvent = 0;
    mLastEvent = 0;
    mDelayCounter = 0;

    return 1;
}

int daEventLooper_c::execute() {
    u8 triggeringEvent = (mEventNums[0] > 0) ? mEventNums[0] - 1 : 0;
    u8 startEvent = (mEventNums[1] > 0) ? mEventNums[1] - 1 : 0;
    u8 targetEventEnd = (mEventNums[2] > 0) ? mEventNums[2] - 1 : 0;

    bool triggeringActive = dSwitchFlagMng_c::checkEvent(triggeringEvent);

    switch (mState) {
        case STATE_WAITING: {
            if (triggeringActive) {
                mCurrentEvent = startEvent;
                dSwitchFlagMng_c::m_instance->set(mCurrentEvent, 0, true, false, false, false);
                mLastEvent = mCurrentEvent;
                mDelayCounter = 0;
                mState = STATE_RUNNING;
            }
            break;
        }

        case STATE_RUNNING: {
            if (!triggeringActive) {
                dSwitchFlagMng_c::m_instance->set(mLastEvent, 0, false, false, false, false);
                mState = STATE_WAITING;
                break;
            }

            mDelayCounter++;
            if (mDelayCounter >= mDelay) {
                dSwitchFlagMng_c::m_instance->set(mLastEvent, 0, false, false, false, false);

                if (mCurrentEvent >= targetEventEnd) {
                    mState = STATE_FINISHED;
                    mDelayCounter = 0;
                } else {
                    mCurrentEvent++;
                    dSwitchFlagMng_c::m_instance->set(mCurrentEvent, 0, true, false, false, false);
                    mLastEvent = mCurrentEvent;
                    mDelayCounter = 0;
                }
            }
            break;
        }

        case STATE_FINISHED: {
            if (!triggeringActive) {
                dSwitchFlagMng_c::m_instance->set(mLastEvent, 0, false, false, false, false);
                mState = STATE_WAITING;
                break;
            }

            mDelayCounter++;
            if (mDelayCounter >= mDelay) {
                dSwitchFlagMng_c::m_instance->set(mLastEvent, 0, false, false, false, false);
                mCurrentEvent = startEvent;
                dSwitchFlagMng_c::m_instance->set(mCurrentEvent, 0, true, false, false, false);
                mLastEvent = mCurrentEvent;
                mDelayCounter = 0;
                mState = STATE_RUNNING;
            }
            break;
        }
    }

    return 1;
}

int daEventLooper_c::draw() {
    return 1;
}

int daEventLooper_c::doDelete() {
    dSwitchFlagMng_c::m_instance->set(mLastEvent, 0, false, false, false, false);
    return 1;
}