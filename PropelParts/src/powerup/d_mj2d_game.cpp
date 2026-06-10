#include <kamek.h>
#include <game/bases/d_mj2d_data.hpp>

// dMj2dGame_c::setStockItem()
kmBranchDefCpp(0x800CE420, NULL, void, dMj2dGame_c *this_, int item, s8 count) {
    if (count > 99) {
        count = 99;
    }
    this_->setNewStockItem(item, count);
}
