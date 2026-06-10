#include <kamek.h>
#include <constants/game_constants.h>

// Clear all stocked item fields after a game over
kmWrite16(0x80789042, NEW_ITEM_COUNT);
