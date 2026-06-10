#pragma once

#include <game/bases/d_base.hpp>
#include <game/bases/d_lytbase.hpp>
#include <constants/game_constants.h>

/// @unofficial
class dStockItemShadow_c : public dBase_c {
public:
    enum T_PANE_e {
        T_itemRest_00, T_itemRest_01, T_itemRest_02, T_itemRest_06,
        T_itemRest_04, T_itemRest_05, T_itemRest_03, T_itemRest_07,
        T_itemX_00, T_itemX_01, T_itemX_02, T_itemX_06,
        T_itemX_04, T_itemX_05, T_itemX_03, T_itemX_07,
        T_COUNT,
    };

    enum P_PANE_e {
        P_buttonBaseS_00, P_buttonBaseS_01, P_buttonBaseS_02, P_buttonBaseS_03,
        P_buttonBaseS_04, P_buttonBaseS_05, P_buttonBaseS_06, P_buttonBaseS_07,
        P_COUNT
    };

    dStockItemShadow_c();

    LytBase_c mLayout;
    nw4r::lyt::Pane *mpRootPane;
    // Unused
    LytTextBox_c *mpTextBoxes[14];
    nw4r::lyt::Picture *mpPicturePanes[7];

    bool mHasLayoutLoaded;
    bool mIsVisible;
    bool mDoSetInfo;
    int mItemCounts[NEW_ITEM_COUNT];

    // New
    LytTextBox_c *mpNewTextBoxes[T_COUNT];
    nw4r::lyt::Picture *mpNewPicturePanes[P_COUNT];
};
