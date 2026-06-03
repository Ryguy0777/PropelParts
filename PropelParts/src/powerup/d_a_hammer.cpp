#include <kamek.h>
#include <game/bases/d_a_hammer.hpp>
#include <game/bases/d_a_player_base.hpp>

// Modify thrown hammer trajectory
kmBranchDefCpp(0x807fd020, NULL, void, daHammer_c *this_, daPlBase_c *player) {
    // Hammer is thrown by player, not spat from yoshi
    bool isPlayerHammer = player->mProfName == fProfile::PLAYER;
    if (isPlayerHammer) {
        // This fixes 2 things:
        // The bug in Newer where you could throw a hammer "backwards"
        // Hammers thrown from a spinjump throwing in both directions
        this_->mDirection = (this_->mParam >> 4) & 1;
    }

    this_->mSpeed.x = player->mSpeed.x * 0.5f + daHammer_c::smc_THROW_SPEED[this_->mDirection];
    this_->mSpeed.y = 2.7f;
    this_->m_24 = 0;
    this_->mAccelY = -0.175f;
    this_->mSpeedMax.x = daHammer_c::smc_THROW_SPEED_MAX[this_->mDirection];
    this_->mMaxFallSpeed = -4.0f;
    this_->mSpeedMax.y = -4.0f;
    this_->mAccelF = 0.04f;
    this_->m_28 = 0;
    this_->mAngle.x = -0x2000;
    this_->mAngle.y = daHammer_c::smc_THROW_ANGLE[this_->mDirection];
    
    if ((this_->mParam >> 16 & 0xF) == 0) {
        // Do a higher throw if the player is holding up
        if (isPlayerHammer && player->mKey.buttonUp()) {
            this_->mSpeed.x = player->mSpeed.x * 0.5f + (daHammer_c::smc_THROW_SPEED[this_->mDirection] * 0.4f);
            this_->mSpeed.y = 4.0f;
            this_->mSpeedMax.x *= 0.4f;
        }
    }
}

bool daHammer_c__CheckHammerLimit(int playerNo, int limitMode) {
    int hammerCount = 0;
    daHammer_c *hammer = nullptr;
    do {
        hammer = (daHammer_c*)fManager_c::searchBaseByProfName(fProfile::HAMMER, hammer);
        // There are 3 requirements for a hammer to count to the hammer limit
        // 1. It is in the "Throw" state
        // 2. The player number in the parameters is equal to the player being checked
        // 3. The "spin jump" paramater equals limitMode
        if (hammer && hammer->mStateMgr.getStateID()->isEqual(daHammer_c::StateID_SpiteMove)) {
            if (playerNo == (hammer->mParam & 0xF)) {
                if ((hammer->mParam >> 16 & 0xF) == limitMode) {
                    hammerCount++;
                }
            }
        }
    } while (hammer);

    // If we're checking spin jump thrown hammers, increase the hammer limit to 4
    if (limitMode) {
        return hammerCount < 4;
    }
    return hammerCount < 2;
}