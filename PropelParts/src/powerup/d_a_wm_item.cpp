#include <kamek.h>
#include <game/bases/d_a_wm_item.hpp>
#include <game/bases/d_game_com.hpp>
#include <game/bases/d_res_mng.hpp>
#include <constants/game_constants.h>

const char *daWmItem_c::sc_resNames[] = {
    "I_kinoko",
    "I_fireflower",
    "I_propeller",
    "I_iceflower",
    "I_penguin",
    "I_kinoko",
    "I_star",
    "I_hammer",
};

const char *daWmItem_c::sc_mdlNames[] = {
    "I_kinoko",
    "I_fireflower",
    "I_propeller_model",
    "I_iceflower",
    "I_penguin",
    "I_kinoko",
    "I_star",
    "I_hammer",
};

const char *daWmItem_c::sc_anmNames[] = {
    "SI_kinoko",
    "SI_fireflower",
    "SI_propeller",
    "SI_iceflower",
    "SI_penguin",
    "SI_kinoko",
    "SI_star",
    "SI_hammer",
};

// daWmItem_c::createMdl()
kmBranchDefCpp(0x808D4C20, NULL, void, daWmItem_c *this_) {
    this_->mAllocator.createFrmHeap(-1, mHeap::g_gameHeaps[0], nullptr, 0x20);
    // Make BRRES path
    char resName[100];
    memset(resName, 0, 100);
    strncat(resName, "g3d/", 99);
    strncat(resName, daWmItem_c::sc_resNames[this_->mItemType], 99);
    strncat(resName, ".brres", 99);

    this_->mResFile = dResMng_c::m_instance->getRes(daWmItem_c::sc_resNames[this_->mItemType], resName);
    nw4r::g3d::ResMdl mdl = this_->mResFile.GetResMdl(daWmItem_c::sc_mdlNames[this_->mItemType]);

    ulong buffOpt = 0x20;
    // Check if we need to allocate for TexPAT anims
    if (this_->mItemType == ITEM_MUSHROOM || this_->mItemType == ITEM_MINI_MUSHROOM) {
        buffOpt = 0x23;
    }

    this_->mModel.create(mdl, &this_->mAllocator, buffOpt, 1, 0);

    // Animations
    this_->mResAnmFile = dResMng_c::m_instance->getRes(daWmItem_c::sc_anmNames[this_->mItemType], "g3d/model.brres");
    nw4r::g3d::ResAnmChr resChr = this_->mResAnmFile.GetResAnmChr("stok_wait");

    this_->mAnmChr.create(mdl, resChr, &this_->mAllocator, 0);
    this_->mAnmChr.setAnm(this_->mModel, resChr, m3d::FORWARD_LOOP);
    this_->mModel.setAnm(this_->mAnmChr, 1.0f);

    // TexPAT anims for the mushrooms
    if (this_->mItemType == ITEM_MUSHROOM || this_->mItemType == ITEM_MINI_MUSHROOM) {
        this_->mResAnmTexPat = this_->mResFile.GetResAnmTexPat("I_kinoko_switch");

        this_->mAnmTexPat.create(mdl, this_->mResAnmTexPat, &this_->mAllocator, 0, 1);
        this_->mAnmTexPat.setAnm(this_->mModel, this_->mResAnmTexPat, 0, m3d::FORWARD_ONCE);
        this_->mModel.setAnm(this_->mAnmTexPat);

        float frame = (this_->mItemType == ITEM_MINI_MUSHROOM) ? 2.0f : 0.0f;
        this_->mAnmTexPat.setFrame(frame, 0);
        this_->mAnmTexPat.setRate(0.0f, 0);
    }

    dGameCom::SetSoftLight_Item(this_->mModel, 1);
    this_->mAllocator.adjustFrmHeap();
}
