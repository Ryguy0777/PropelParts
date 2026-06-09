#include <kamek.h>
#include <game/bases/d_a_en_blockmain.hpp>

// Allow item ID of 6
kmWriteNop(0x80022888);
kmWriteNop(0x8002288c);

kmWriteNop(0x80022b8c);
kmWriteNop(0x80022b90);

// Allow use of alternative item table

/*
    ITEM TABLE VALUES AND THEIR MEANING
    0x0 = Fire Flower
    0x1 = Star
    0x2 = Coin
    0x4 = Coin (used for 10-coin)
    0x5 = Mushroom (used by 10-coin to spawn the reward coins)
    0x6 = Mushroom (unused?)
    0x7 = 1-UP Mushroom
    0x8 = Mushroom (used by mushroom-if-small)
    0xA = Mushroom (used by vine)
    0xC = Mushroom (used by yoshi egg)
    0xD = Mushroom (used by spring)
    0xE = Ice Flower
    0xF = Empty
    0x11 = Penguin
    0x15 = Propeller
    0x19 = Mini Mushroom
    0x1B = Mushroom (used by continuous star)

    NEW ITEMS
    0x6 = Hammer Suit

    All other values will give mushrooms
*/
const u32 l_new_item_values[] = {
    0xF,    // Empty
    0x2,    // Coin (do not replace)
    0x6,    // Hammer Suit (replaced, originally Fire Flower)
    0x0,    // Fire Flower
    0x15,   // Propeller
    0x11,   // Penguin
    0x19,   // Mini Mushroom
    0x1,    // Star
    0x1B,   // Continous Star (do not replace)
    0xC,    // Yoshi Egg
    0x4,    // 10-coin (do not replace)
    0x7,    // 1-UP Mushroom
    0xA,    // Vine
    0xD,    // Spring
    0x8,    // Mushroom-if-Small (do not replace)
    0xE,    // Ice Flower
    0x5,    // 10-coin reward (do not replace)
    0x6,    // Unused
};

// Both of the following patch the daEnBlockMain_c function for spawning items
// If a block actor doesn't use this (I only think EN_BIG_RENGA_BLOCK and the tile blocks) then they won't spawn a hammer suit
// There's a second table in use here too and I'm not entirely sure what it's for, if something breaks then that needs patching too

kmBranchDefAsm(0x80022854, 0x80022858) {
    lwz r0, 0x4(r27) // Load mParam into r0
    rlwinm r0, r0, 25, 31, 31 // Get bit 7
    cmpwi r0, 0
    beq useOriginalTable

    // Load our new item table
    lis r3, l_new_item_values@h
    ori r3, r3, l_new_item_values@l
    b ret

    // Load the original table
    useOriginalTable:
    lis r3, l_item_values@h
    ori r3, r3, l_item_values@l

    ret:
    blr
}

kmBranchDefAsm(0x80022b58, 0x80022b5c) {
    lwz r12, 0x4(r27) // Load mParam into r12
    rlwinm r12, r12, 25, 31, 31 // Get bit 7
    cmpwi r12, 0
    beq useOriginalTable

    // Load our new item table
    lis r5, l_new_item_values@h
    ori r5, r5, l_new_item_values@l
    b ret

    // Load the original table
    useOriginalTable:
    lis r5, l_item_values@h
    ori r5, r5, l_item_values@l

    ret:
    blr
}