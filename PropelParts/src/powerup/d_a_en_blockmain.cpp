#include <kamek.h>

// TEMP item block ID changes
kmWrite8(0x802ee62b, 0x06);
kmWrite8(0x802EE667, 0xAA);
kmWrite8(0x802EE6AF, 0xAA);

// Allow item ID of 6
kmWrite32(0x80022884, 0x281F00AA);
kmWrite32(0x80022B88, 0x280500AA);