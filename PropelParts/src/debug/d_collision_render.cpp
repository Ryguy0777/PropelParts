// Source: https://github.com/CLF78/NSMASR-v2/blob/master/game/debug/collisionRender.cpp
#include <kamek.h>
#include <propelparts/bases/d_collision_render.hpp>
#include <game/bases/d_actor.hpp>
#include <game/bases/d_rc.hpp>
#include <game/bases/d_bg_ctr.hpp>
#include <nw4r/g3d/g3d_camera.h>
#include <nw4r/math/math_triangular.h>
#include <revolution/GX.h>
#include <game_versions_nsmbw.h>
#include <propelparts/bases/d_debug_config.hpp>

// Patches to allow tracking instances of dBc_c
static dBc_c* firstBc = NULL;
static dBc_c* lastBc = NULL;

extern "C" {
void addBcToList(dBc_c* self);
void removeBcFromList(dBc_c* self);
}

// Drawing helper functions
void DrawPoint(float x, float y, float z, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_POINTS, GX_VTXFMT0, 1);

    GXPosition3f32(x, y, z);
    GXColor4u8(r,g,b,a);

    GXEnd();
}

void DrawLine(float x1, float y1, float x2, float y2, float z, u8 r, u8 g, u8 b, u8 a) {
    GXBegin(GX_LINES, GX_VTXFMT0, 2);

    GXPosition3f32(x1, y1, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(x2, y2, z);
    GXColor4u8(r,g,b,a);

    GXEnd();
}

void DrawQuad(float tlX, float tlY, float trX, float trY, float blX, float blY, float brX, float brY, float z, u8 r, u8 g, u8 b, u8 a, bool addDiagonal) {

    // Setup drawing
    GXBegin(GX_LINES, GX_VTXFMT0, 8 + addDiagonal * 2);

    // Draw top
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);

    // Draw left
    GXPosition3f32(tlX, tlY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);

    // Draw right
    GXPosition3f32(trX, trY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    // Draw bottom
    GXPosition3f32(blX, blY, z);
    GXColor4u8(r,g,b,a);
    GXPosition3f32(brX, brY, z);
    GXColor4u8(r,g,b,a);

    // Draw diagonal if enabled
    if (addDiagonal) {
        GXPosition3f32(trX, trY, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(blX, blY, z);
        GXColor4u8(r,g,b,a);
    }

    GXEnd();
}

// Credits to Ismy for helping me with this one
void DrawCircle(float centreX, float centreY, float radiusX, float radiusY, float z, u8 r, u8 g, u8 b, u8 a) {

    // Define a few variables
    const int numVert = 64;
    const float step = 256.0f/numVert;
    float sin, cos, xDist, yDist;

    // Initialize the prev variables
    float prevXDist = radiusX;
    float prevYDist = 0.0f;

    // Begin drawing
    GXBegin(GX_LINES, GX_VTXFMT0, numVert * 2);

    // Draw each line
    for (int i = 1; i <= numVert / 4; i++) {

        // Calculate sin and cos for the current angle
        nw4r::math::SinCosFIdx(&sin, &cos, step * i);

        // Calculate the distances from the center
        xDist = radiusX * cos;
        yDist = radiusY * sin;

        // Draw on the first quadrant
        GXPosition3f32(centreX + prevXDist, centreY + prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + xDist, centreY + yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the second quadrant
        GXPosition3f32(centreX - prevYDist, centreY + prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - yDist, centreY + xDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the third quadrant
        GXPosition3f32(centreX - prevXDist, centreY - prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX - xDist, centreY - yDist, z);
        GXColor4u8(r,g,b,a);

        // Draw on the fourth quadrant
        GXPosition3f32(centreX + prevYDist, centreY - prevXDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + yDist, centreY - xDist, z);
        GXColor4u8(r,g,b,a);

        // Override the "previous" values
        prevXDist = xDist;
        prevYDist = yDist;
    }

    // End drawing
    GXEnd();
}

void DrawPartialCircle(float centreX, float centreY, float radius, float z, u16 minAngle, u16 maxAngle, u8 r, u8 g, u8 b, u8 a) {

    // Initialize variables
    const int maxVerts = 64;
    const float step = 256.0f/maxVerts;
    float sin, cos, xDist, yDist;

    // Calculate the number of required vertices and the starting angle
    u16 totalAngle = (maxAngle - minAngle) & 0xFFFF;
    int numVert = (totalAngle * maxVerts / 0x10000);
    float startAngle = minAngle / 256.0f;

    // Set up the prev variables
    nw4r::math::SinCosFIdx(&sin, &cos, startAngle);
    float prevXDist = radius * cos;
    float prevYDist = radius * sin;

    // Begin drawing
    GXBegin(GX_LINES, GX_VTXFMT0, numVert * 2);

    // Draw each line
    for (int i = 1; i <= numVert; i++) {

        // Calculate sin and cos for the current angle
        nw4r::math::SinCosFIdx(&sin, &cos, startAngle + step * i);

        // Calculate the distances from the center
        xDist = radius * cos;
        yDist = radius * sin;

        // Draw the line
        GXPosition3f32(centreX + prevXDist, centreY + prevYDist, z);
        GXColor4u8(r,g,b,a);
        GXPosition3f32(centreX + xDist, centreY + yDist, z);
        GXColor4u8(r,g,b,a);

        // Override the "previous" values
        prevXDist = xDist;
        prevYDist = yDist;
    }
}

dCollisionRender_c dCollisionRender_c::m_instance;

dCollisionRender_c::dCollisionRender_c() {
    create(nullptr, nullptr);
}

// Dummy function
void dCollisionRender_c::drawOpa() {}

// Actual drawing
void dCollisionRender_c::drawXlu() {

    // Setup copied from 0x801717C0
    // Set current vertex descriptor to enable position and color0, both provided directly
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

    // Position has 3 elements (x,y,z), each of type f32
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);

    // Color 0 has 4 components (r, g, b, a), each of type u8
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

    // Disable culling
    GXSetCullMode(GX_CULL_NONE);

    // Enable color channel 0 and set vertex color as source
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);

    // Enable one TEV stage
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);

    // Set blend mode
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);

    // Load camera matrix
    nw4r::math::MTX34 mtx;
    m3d::getCurrentCamera().GetCameraMtx(&mtx);
    GXLoadPosMtxImm(mtx, 0);

    // Set line width to 3 pixels and point size to 6 pixels
    GXSetLineWidth(18, 0);
    GXSetPointSize(36, 0);

    // Load debug flags from config
    u32 flags = dDebugConfig_c::m_instance->mCollisionDebugFlags;

    // Draw all instances of dCc_c
    if (flags & (1 << ColliderDisplayFlags::Hitboxes)) {
        dCc_c* currCc = dCc_c::mEntryN;
        while (currCc) {

            // Make sure the actor isn't dead and that its owner exists
            if (currCc->mInfo != CC_NO_HIT) {

                u32 uptr = (u32)currCc;
                u8 r = (uptr >> 16) & 0xFF;
                u8 g = (uptr >> 8) & 0xFF;
                u8 b = uptr & 0xFF;
                u8 a = 0xFF;

                float centreX = currCc->getCenterPosX();
                float centreY = currCc->getCenterPosY();
                float edgeDistX = currCc->mCcData.mBase.mSize.x;
                float edgeDistY = currCc->mCcData.mBase.mSize.y;
                u8 collType = currCc->mShape;

                // Call DrawCircle for circles
                if (collType == CC_SHAPE_CIRCLE) {
                    DrawCircle(centreX, centreY, edgeDistX, edgeDistY, 9000.0f, r, g, b, a);
                } else { // Else call DrawQuad
                    float tlX, tlY, trX, trY, blX, blY, brX, brY;
                    bool addDiagonal = true;

                    // Use trapezoidDist for Y coordinates if collType is 2
                    // Else edge distance
                    if (collType == CC_SHAPE_DAIKEI_UD) {
                        tlY = centreY + currCc->mTrpOffsets[0];
                        trY = centreY + currCc->mTrpOffsets[2];
                        blY = centreY + currCc->mTrpOffsets[1];
                        brY = centreY + currCc->mTrpOffsets[3];
                        addDiagonal = false;
                    } else {
                        tlY = centreY + edgeDistY;
                        trY = centreY + edgeDistY;
                        blY = centreY - edgeDistY;
                        brY = centreY - edgeDistY;
                    }

                    // Use trapezoidDist for X coordinates if collType is 3
                    // Else edge distance
                    if (collType == CC_SHAPE_DAIKEI_LR) {
                        tlX = centreX + currCc->mTrpOffsets[0];
                        trX = centreX + currCc->mTrpOffsets[1];
                        blX = centreX + currCc->mTrpOffsets[2];
                        brX = centreX + currCc->mTrpOffsets[3];
                        addDiagonal = false;
                    } else {
                        tlX = centreX - edgeDistX;
                        trX = centreX + edgeDistX;
                        blX = centreX - edgeDistX;
                        brX = centreX + edgeDistX;
                    }

                    // Draw the quad
                    DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, 9000.0f, r, g, b, a, addDiagonal);
                }
            }

            currCc = currCc->mpNext;
        }
    }

    // Draw all instances of dBg_ctr_c
    if (flags & (1 << ColliderDisplayFlags::Colliders)) {
        dBg_ctr_c* currBgCtr = dBg_ctr_c::mEntryN;
        while (currBgCtr) {

            u32 uptr = (u32)currBgCtr;
            u8 r = (uptr >> 16) & 0xFF;
            u8 g = (uptr >> 8) & 0xFF;
            u8 b = uptr & 0xFF;
            u8 a = 0xFF;

            // If round, draw a circle
            if (currBgCtr->mIsRound) {
                DrawCircle(currBgCtr->mLastPos.x, currBgCtr->mLastPos.y, currBgCtr->mRadius, currBgCtr->mRadius, 9000.0f, r, g, b, a);
            } else { // Else draw a quad
                float tlX = currBgCtr->mCalcedPos[0].x;
                float tlY = currBgCtr->mCalcedPos[0].y;
                float trX = currBgCtr->mCalcedPos[3].x;
                float trY = currBgCtr->mCalcedPos[3].y;
                float blX = currBgCtr->mCalcedPos[1].x;
                float blY = currBgCtr->mCalcedPos[1].y;
                float brX = currBgCtr->mCalcedPos[2].x;
                float brY = currBgCtr->mCalcedPos[2].y;
                DrawQuad(tlX, tlY, trX, trY, blX, blY, brX, brY, 9000.0f, r, g, b, a, false);
            }

            currBgCtr = currBgCtr->mpNext;
        }
    }

    // Draw all instances of dBc_c
    if (flags & (1 << ColliderDisplayFlags::Sensors)) {
        dBc_c* currBc = firstBc;
        while (currBc) {

            // Get the color
            u32 uptr = (u32)currBc;
            u8 r = (uptr >> 16) + 0x20;
            u8 g = (uptr >> 8) + 0x30;
            u8 b = (uptr & 0xFF) + 0x80;
            u8 a = 0xFF;

            // Get the actor's position
            float ownerPosX = currBc->mpOwnerPos->x;
            float ownerPosY = currBc->mpOwnerPos->y;

            // Make an array of sensors
            sBcSensorIf_c *sensors[4] = {currBc->mpSensorFoot, currBc->mpSensorHead, currBc->mpSensorWall, currBc->mpSensorWall};

            // Draw the sensors
            for (int i = 0; i < 4; i++) {

                // Check if the sensor exists
                sBcSensorIf_c *sensor = sensors[i];
                if (sensor == nullptr) {
                    continue;
                }

                // Multiplier for the adjacent sensors
                int mult = (i == 3) ? -1 : 1;
                float x1, y1, x2, y2;

                // sBcPointData is probably the wrong thing to typecast to but I don't really care
                bool isLine = ((sBcPointData *)sensor)->mFlags & 1;

                if (isLine == false) {
                    sBcSensorPoint *pointSensor = (sBcSensorPoint *)sensor;

                    x1 = ownerPosX + (float)(mult * pointSensor->mX / 4096);
                    y1 = ownerPosY + (float)(pointSensor->mY / 4096);

                    DrawPoint(x1, y1, 8005.0f, r, g, b, a);

                } else {
                    sBcSensorLine *lineSensor = (sBcSensorLine *)sensor;

                    if (i < 2) {
                        x1 = ownerPosX + (float)(lineSensor->mLineA / 4096);
                        x2 = ownerPosX + (float)(lineSensor->mLineB / 4096);
                        y1 = ownerPosY + (float)(lineSensor->mDistanceFromCenter / 4096);
                        y2 = ownerPosY + (float)(lineSensor->mDistanceFromCenter / 4096);
                    } else {
                        x1 = ownerPosX + (float)(mult * lineSensor->mDistanceFromCenter / 4096);
                        x2 = ownerPosX + (float)(mult * lineSensor->mDistanceFromCenter / 4096);
                        y1 = ownerPosY + (float)(lineSensor->mLineA / 4096);
                        y2 = ownerPosY + (float)(lineSensor->mLineB / 4096);
                    }

                    DrawLine(x1, y1, x2, y2, 8005.0f, r, g, b, a);
                }
            }

            // Repurpose the (otherwise unused) vtable pointer as a pointer to the next dBc_c instance
            currBc = *(dBc_c **)currBc;
        }
    }

    // Draw all instances of dRide_ctr_c
    if (flags & (1 << ColliderDisplayFlags::RideableColliders)) {
        dRide_ctr_c* currRide = dRide_ctr_c::mEntryN;
        while (currRide) {

            u32 uptr = (u32)currRide;
            u8 r = (uptr >> 16) & 0xFF;
            u8 g = (uptr >> 8) & 0xFF;
            u8 b = uptr & 0xFF;
            u8 a = 0xFF;

            // For dRide2Point and dRideRoll, draw a simple line
            if (currRide->mType <= 2) {
                DrawLine(currRide->mLeft.x, currRide->mLeft.y, currRide->mRight.x, currRide->mRight.y, 9000.0f, r, g, b, a);
            } else { // dRideCircle is a little bit more complex
                dRideCircle_c* currCircle = (dRideCircle_c*)currRide;

                // Get centre and radius
                float centreX = currCircle->mpOwner->mPos.x + currCircle->mCenterOffset.x;
                float centreY = currCircle->mpOwner->mPos.y + currCircle->mCenterOffset.y;
                float radius = currCircle->mRadius;

                // If the circle is full, use the regular circle method
                if (currCircle->mLeftAngleOffset + currCircle->mRightAngleOffset == 0x10000) {
                    DrawCircle(centreX, centreY, radius, radius, 9000.0f, r, g, b, a);
                } else { // Else draw a partial circle
                    u16 minAngle = currCircle->mRotation - currCircle->mRightAngleOffset;
                    u16 maxAngle = minAngle + currCircle->mLeftAngleOffset;
                    DrawPartialCircle(centreX, centreY, radius, 9000.0f, minAngle, maxAngle, r, g, b, a);
                }
            }

            currRide = currRide->mpNext;
        }
    }
}

// Patches to allow tracking instances of dBc_c
void addBcToList(dBc_c* self) {

    // If the bc was already initialized, bail
    if (*(dBc_c **)self || lastBc == self) {
        return;
    }

    // If first is NULL, set this bc as both first and last
    if (firstBc == NULL) {
        firstBc = self;
        lastBc = self;

    // Else set the next field of the previous one, and set this one as last
    } else {
        *(dBc_c **)lastBc = self;
        lastBc = self;
    }
}

void removeBcFromList(dBc_c* self) {

    // If no bcs are initialized, bail
    if (!firstBc) {
        return;
    }

    // If this bc has no "next" and isn't the last bc it was never initialized, so bail
    if (!*(dBc_c **)self && lastBc != self) {
        return;
    }

    // If this bc is the only initialized one, set both first and last to NULL
    if (firstBc == lastBc) {
        firstBc = NULL;
        lastBc = NULL;
        return;
    }

    // If this bc is the first one, set the next one as the first one
    if (firstBc == self) {
        firstBc = *(dBc_c **)self;
        return;
    }

    // Find the previous bc
    dBc_c* prevBc = firstBc;
    while (*(dBc_c **)prevBc != self) {
        prevBc = *(dBc_c **)prevBc;
    }

    // Remove this bc from the list
    *(dBc_c **)prevBc = *(dBc_c **)self;

    // If this was the last bc, set the previous one as the new last
    if (lastBc == self) {
        lastBc = prevBc;
    }
}

// Initialize "next" field to NULL
kmWrite32(0x8006CF50, 0x38800000);

// Add instance to the linked list once initialized
kmCallDefAsm(0x8006D104) {

    // No stack saving needed
    nofralloc

    // Original instruction
    stw r0, 0xE8(r30)

    // Call CPP function
    mr r3, r30
    b addBcToList
}

// Remove instance from the linked list when destroyed
kmCallDefAsm(0x8006CFBC) {

    // Push stack manually
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r4, 0xC(r1)

    // Call CPP function
    bl removeBcFromList

    // Pop stack manually
    lwz r4, 0xC(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10

    // Restore registers
    mr r3, r31
    cmpwi r4, 0
    blr
}