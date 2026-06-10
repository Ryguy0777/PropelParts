#pragma once

#include <game/bases/d_base_actor.hpp>
#include <game/bases/d_heap_allocator.hpp>
#include <game/mLib/m_3d.hpp>

/// @unofficial
class daWmItem_c : public dBaseActor_c {
public:
    void calcMdl();

    dHeapAllocator_c mAllocator;
    m3d::mdl_c mModel;
    m3d::anmChr_c mAnmChr;
    nw4r::g3d::ResFile mResFile;
    nw4r::g3d::ResFile mResAnmFile;
    nw4r::g3d::ResAnmTexPat mResAnmTexPat;
    m3d::anmTexPat_c mAnmTexPat;

    mVec3_c mSizeFactor; // Actual size is this * 3.2f (or 2.08f for Mini Mushroom)
    int mCurrAnm;
    int mNextAnm;

    bool mIsCreated;
    bool mIsVisible;
    bool mDoDraw;
    bool mOverridePosZ; // If true, sets mPos.z to 200.0f
    int mItemType;

    static const char *sc_resNames[];
    static const char *sc_mdlNames[];
    static const char *sc_anmNames[];
};
