#include <kamek.h>
#include <game/bases/d_game_com.hpp>
#include <game/bases/d_stock_item_shadow.hpp>
#include <constants/game_constants.h>

// dStockItemShadow_c_classInit()
kmBranchDefCpp(0x807B2030, NULL, dStockItemShadow_c *) {
    return new dStockItemShadow_c;
}

// dStockItemShadow_c::createLayout()
kmBranchDefCpp(0x807B21B0, NULL, bool, dStockItemShadow_c *this_) {
    if (!this_->mLayout.ReadResource("stockItemShadow/stockItemShadow.arc", false)) {
        return false;
    }

    this_->mLayout.build("stockItemShadow_00.brlyt", nullptr);

    this_->mpRootPane = this_->mLayout.getRootPane();

    static const char *T_PANE_NAME_TBL[] = {
        "T_itemRest_00", "T_itemRest_01", "T_itemRest_02", "T_itemRest_06",
        "T_itemRest_04", "T_itemRest_05", "T_itemRest_03", "T_itemRest_07",
        "T_itemX_00", "T_itemX_01", "T_itemX_02", "T_itemX_06",
        "T_itemX_04", "T_itemX_05", "T_itemX_03", "T_itemX_07"
    };

    static const char *P_PANE_NAME_TBL[] = {
        "P_buttonBaseS_00", "P_buttonBaseS_01", "P_buttonBaseS_02", "P_buttonBaseS_03",
        "P_buttonBaseS_04", "P_buttonBaseS_05", "P_buttonBaseS_06", "P_buttonBaseS_07"
    };

    static const char *T_PANE_FIXED_NAME_TBL[] = {
        "T_itemX_00", "T_itemX_01", "T_itemX_02", "T_itemX_06",
        "T_itemX_04", "T_itemX_05", "T_itemX_03", "T_itemX_07"
    };
    static const int MESSAGE_DATA_TBL[] = {
        0x2E, 0x2E, 0x2E, 0x2E,
        0x2E, 0x2E, 0x2E, 0x2E
    };

    this_->mLayout.TPaneRegister(T_PANE_NAME_TBL, &this_->mpNewTextBoxes[dStockItemShadow_c::T_itemRest_00], ARRAY_SIZE(T_PANE_NAME_TBL));
    this_->mLayout.TPaneNameRegister(T_PANE_FIXED_NAME_TBL, MESSAGE_DATA_TBL, 0x2, ARRAY_SIZE(T_PANE_FIXED_NAME_TBL));
    this_->mLayout.PPaneRegister(P_PANE_NAME_TBL, &this_->mpNewPicturePanes[dStockItemShadow_c::P_buttonBaseS_00], ARRAY_SIZE(P_PANE_NAME_TBL));
    return true;
}

// dStockItemShadow_c::setInfo()
kmBranchDefCpp(0x807B2370, NULL, void, dStockItemShadow_c *this_) {
    if (this_->mDoSetInfo) {
        this_->mDoSetInfo = false;

        static const GXColor UP_COLOR_DATA_TBL[] = {
            {0xFF, 0xFF, 0xFF, 0xFF},
            {0xFF, 0xFF, 0xFF, 0xFF}
        };

        static const GXColor DOWN_COLOR_DATA_TBL[] = {
            {0x46, 0x46, 0x46, 0xFF},
            {0xFF, 0xFF, 0xFF, 0xFF}
        };

        // Update item displays
        for (int i = 0; i < NEW_ITEM_COUNT; i++) {
            int count = this_->mItemCounts[i];
            bool isOffBtn = (count == 0);

            this_->mpNewPicturePanes[dStockItemShadow_c::P_buttonBaseS_00+i]->SetVisible(isOffBtn);

            GXColor upColor = UP_COLOR_DATA_TBL[!isOffBtn];
            GXColor downColor = DOWN_COLOR_DATA_TBL[!isOffBtn];

            this_->mpNewTextBoxes[dStockItemShadow_c::T_itemRest_00+i]->SetVtxColor(0, upColor);
            this_->mpNewTextBoxes[dStockItemShadow_c::T_itemRest_00+i]->SetVtxColor(2, downColor);

            this_->mpNewTextBoxes[dStockItemShadow_c::T_itemX_00+i]->SetVtxColor(0, upColor);
            this_->mpNewTextBoxes[dStockItemShadow_c::T_itemX_00+i]->SetVtxColor(2, downColor);

            dGameCom::LayoutDispNumber(count, 2, this_->mpNewTextBoxes[dStockItemShadow_c::T_itemRest_00+i], true);
        }
    }
}
