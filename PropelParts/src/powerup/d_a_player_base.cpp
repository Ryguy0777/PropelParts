#include <kamek.h>
#include <constants/game_constants.h>

// Powerup scale fixes, see d_player_model_manager.cpp and d_a_player.cpp for the rest

extern float PowerupScales[NEW_POWERUP_COUNT+2];

kmCallDefAsm(0x8004cc80) {
    lis r6, PowerupScales@h
	ori r6, r6, PowerupScales@l
	add r3, r6, r0
	blr
}