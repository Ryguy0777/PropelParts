#include <kamek.h>
#include <game/bases/d_res_mng.hpp>
#include <constants/game_constants.h>

// Load/unload additional resources for the World Maps
// TODO: Once WorldMap branch is merged, move this stuff into the global reslist

static const char *sc_itemMdls[] = {
    "I_hammer",
};

static const char *sc_stockAnms[] = {
    "SI_hammer",
};

// Resource loading sPhase method for WORLD_MAP
kmBranchDefCpp(0x80926D80, NULL, bool) {
    dResMng_c::m_instance->setRes("WorldMap", "cobHatenaSwitch", nullptr);

    dResMng_c::m_instance->setRes("Object", sc_itemMdls, ARRAY_SIZE(sc_itemMdls), nullptr);
    dResMng_c::m_instance->setRes("WorldMap", sc_stockAnms, ARRAY_SIZE(sc_stockAnms), nullptr);
    return true;
}

// After dScWMap_c::doDelete()
kmBranchDefCpp(0x80927AB4, NULL, bool, bool ret) {
    if (ret) {
        dResMng_c::m_instance->getResP()->deleteRes("cobHatenaSwitch");

        for (int i = 0; i < ARRAY_SIZE(sc_itemMdls); i++) {
            dResMng_c::m_instance->getResP()->deleteRes(sc_itemMdls[i]);
        }

        for (int i = 0; i < ARRAY_SIZE(sc_stockAnms); i++) {
            dResMng_c::m_instance->getResP()->deleteRes(sc_stockAnms[i]);
        }
    }
    return ret;
}

// Patch number of WM_ITEM actors to make
kmWrite16(0x809272D2, NEW_ITEM_COUNT);
