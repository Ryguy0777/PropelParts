#include <kamek.h>
#include <constants/game_constants.h>

// Powerup scale fixes, see d_a_player.cpp and d_a_player_base.cpp for the rest

float PowerupScales[NEW_POWERUP_COUNT+2] = {
    1.0f, 1.0f, // Padding values
    1.0f, // Small
    1.0f, // Super
    1.0f, // Fire
    0.6f, // Mini
    1.0f, // Propeller
    1.0f, // Penguin
    1.0f, // Ice
    1.0f, // Hammer
};

kmCallDefAsm(0x800d6fdc) {
    lis r6, PowerupScales@h
	ori r6, r6, PowerupScales@l
	add r4, r6, r0
	blr
}

kmCallDefAsm(0x800d7064) {
    lis r8, PowerupScales@h
	ori r8, r8, PowerupScales@l
	add r7, r8, r0
	blr
}