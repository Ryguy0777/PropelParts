#include <kamek.h>
#include <game/bases/d_info.hpp>
#include <game/bases/d_save_mng.hpp>

// dInfo_c::addStockItem()
kmBranchDefCpp(0x800BB330, NULL, void, dInfo_c *this_, int item) {
    dMj2dGame_c *save = dSaveMng_c::m_instance->getSaveGame(-1);
    save->setNewStockItem(item, save->getNewStockItem(item)+1);
}

// dInfo_c::subStockItem()
kmBranchDefCpp(0x800BB380, NULL, void, dInfo_c *this_, int item) {
    dMj2dGame_c *save = dSaveMng_c::m_instance->getSaveGame(-1);
    save->setNewStockItem(item, save->getNewStockItem(item)-1);
}

// dInfo_c::getStockItem()
kmBranchDefCpp(0x800BB3D0, NULL, int, dInfo_c *this_, int item) {
    dMj2dGame_c *save = dSaveMng_c::m_instance->getSaveGame(-1);
    return save->getNewStockItem(item);
}

// dInfo_c::clsStockItem()
kmBranchDefCpp(0x800BB410, NULL, void, dInfo_c *this_, int item) {
    dMj2dGame_c *save = dSaveMng_c::m_instance->getSaveGame(-1);
    save->setNewStockItem(item, 0);
}
