#include <kamek.h>

// Fix "Mushroom if small" behavior
kmWrite32(0x800221ac, 0x28000003);