#include <kamek.h>
#include <game/bases/d_a_player.hpp>

// Fix "Mushroom if small" behavior
kmWrite32(0x80A2BE98, 0x28000003);
kmWrite32(0x80A28598, 0x28000003);

// Inject hammer item brres string
kmWritePointer(0x80AF0FA8, "g3d/I_hammer.brres");

// Inject hammer item arc name
kmWritePointer(0x80AF0ECC, "I_hammer");

// Inject hammer item model name
kmWritePointer(0x80AF1000, "I_hammer");

// Allow EN_ITEM to transform players into custom powers

// Return values matter here
// 0 = no custom powerup was checked
// 1 = the player has the powerup currently checked
// 2 = player was given powerup (failed)
// 3 = player was given powerup (success)
int PowerupCheck_Custom(int enItem_power, int player_power, dAcPy_c *player) {
    OSReport("%d %d\n", enItem_power, player_power);
    if (enItem_power == 5) { // Hammer suit
        if (player_power == POWERUP_HAMMER_SUIT) {
            // Player already has hammer suit
            return 1;
        }
        if (player->fn_80145c00(POWERUP_HAMMER_SUIT)) {
            return 3;
        } else {
            return 2;
        }
    }
    return 0;
}

extern "C" void PowerupCheck_Custom__FiiP7dAcPy_c(void);

kmCallDefAsm(0x80A285FC) {
    stwu sp, -0x10(sp)
	mflr r0
	stw r0, 0x14(sp)

    mr r5, r30
    bl PowerupCheck_Custom__FiiP7dAcPy_c

    cmpwi r3, 0
    beq NoCustomPowerup

    cmpwi r3, 1
    beq AlreadyHasPowerup

    cmpwi r3, 2
    beq CustomPowerupSet_return

    li r29, 1
    b CustomPowerupSet_return

    AlreadyHasPowerup:
    li r28, 0
    li r29, 2
    b CustomPowerupSet_return

    NoCustomPowerup:
    CustomPowerupSet_return:
    // End of function
	lwz r0, 0x14(sp)
	mtlr r0
	addi sp, sp, 0x10
	
	// Leftover instruction from what we replaced to add the bl
	cmpwi r29, 0
	blr
}

// Allow EN_ITEM to be a custom powerup
extern "C" void daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot(void);

static asm void setHammerToEnItemDCA() {
    bl daEnItem_c__GetWhetherPlayerCanGetPowerupOrNot
	cmpwi r3, 1
	bne DontSetHammer
	
	li r0, 5
	sth r0, 0xDCA(r31)
	
    DontSetHammer:
	lwz r0, 0x14(sp)
	lwz r31, 0xC(sp)
	mtlr r0
	addi sp, sp, 0x10
	blr
}

kmWritePointer(0x80AF117C, &setHammerToEnItemDCA);

kmBranchDefAsm(0x80A2C0B4, NULL) {
    cmplwi r4, 0x19
	bne _not19
	li r0, 0xD
	sth r0, 0xDCA(r3)
    _not19:
	cmplwi r4, 6
	bnelr
	li r0, 5
	sth r0, 0xDCA(r3)
	blr
}

// Load the "wait" animation for custom powerups
kmCallDefAsm(0x80A27CE4) {
    cmplwi r4, 5 // Hammer Suit
	beqlr
	cmplwi r4, 6 // 1-up
	beqlr
	// Neither of those succeeded
	crclr 4*cr0+eq
	blr
}