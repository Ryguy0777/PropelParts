#include <kamek.h>

// Fix "Mushroom if small" behavior
kmWrite32(0x800221ac, 0x28000003);
kmWrite32(0x80A2BE98, 0x28000003);
kmWrite32(0x80A28598, 0x28000003);