#include <kamek.h>
#include <game/bases/d_enemy.hpp>
#include <game/bases/d_a_hammer.hpp>
#include <game/bases/d_a_player_manager.hpp>
#include <game/bases/d_actor_manager.hpp>

// EN_BUBBLE
bool daEnBubble_c__hitCallback_YoshiBullet(dEn_c *podoboo, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	hammer->hitProc_Reflect(self);
	hammer->revengeHitSE();

	mVec3_c hitPos(self->mCollPos.x, self->mCollPos.y, 5500.0f);
	podoboo->hitdamageEffect(hitPos);
	podoboo->setDeathSound_Fire();
	mVec3_c podoboo_center = podoboo->getCenterPos();
	bool dir = dActor_c::getTrgToSrcDir_Main(podoboo->mPos.x + podoboo->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
	dActorMng_c::m_instance->createUpCoin(podoboo_center, dir, 1, 0);

    return podoboo->hitCallback_Ice(self, other);
}
kmWrite32(0x80ACE8E4, 0x0008820C);
kmWritePointer(0x80ADF160, &daEnBubble_c__hitCallback_YoshiBullet);

// EN_DOSUN
kmWrite32(0x80ACF9D0, 0x0008820e);

// EN_BIGDOSUN
bool daEnDosun_c__hitCallback_YoshiBullet(dEn_c *thwomp, dCc_c *self, dCc_c *other) {
	if (thwomp->mProfName == fProfile::EN_DOSUN) {
		return thwomp->dEn_c::hitCallback_YoshiBullet(self, other);
	}
	return false;
}

kmWrite32(0x80ACF9F4, 0x0008820e);
kmWritePointer(0x80AE8360, &daEnDosun_c__hitCallback_YoshiBullet);

// EN_BIG_TERESA
bool daEnTeresa_c__hitCallback_YoshiBullet(dEn_c *boo, dCc_c *self, dCc_c *other) {
	if (boo->mProfName == fProfile::EN_TERESA || boo->mProfName == fProfile::EN_ROTATION_GHOST) {
		daHammer_c *hammer = (daHammer_c*)other->mpOwner;
		mVec3_c hitPos(self->mCollPos.x, self->mCollPos.y, 5500.0f);
		boo->hitdamageEffect(hitPos);
		mVec3_c boo_center = boo->getCenterPos();
		bool dir = dActor_c::getTrgToSrcDir_Main(boo->mPos.x + boo->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
		dActorMng_c::m_instance->createUpCoin(boo_center, dir, 1, 0);

        int playerNo = hammer->mParam & 0xF;
        dAcPy_c *hammerPlayer = daPyMng_c::getPlayer(playerNo);
		return boo->hitCallback_Star(self, &hammerPlayer->mCc);
	}
	return false;
}

kmWrite32(0x80AD4AA0, 0x0008a80e);
kmWrite32(0x80AD4AA4, 0x20020000);
kmWritePointer(0x80B0AE20, &daEnTeresa_c__hitCallback_YoshiBullet);

// EN_BIRIKYU
kmWrite32(0x80ACE17C, 0x00080a04);

// EN_TOGETEKKYU
kmWrite32(0x80AD4D88, 0x00080006);

// EN_BIG_TOGETEKKYU
kmWrite32(0x80ACDD50, 0x00080006);

// EN_UNIRA
bool daEnUnira_c__hitCallback_YoshiBullet(dEn_c *urchin, dCc_c *self, dCc_c *other) {
	return false;
}

kmWrite32(0x80AD4F48, 0x00088a1e);
kmWritePointer(0x80B0CCC0, &daEnUnira_c__hitCallback_YoshiBullet);

// EN_WANWAN
kmWrite32(0x80AD5214, 0x00088206);

// EN_FIRESNAKE
kmWrite32(0x80ACFBC4, 0x00088206);

// EN_KOPONE
bool daEnKopone_c__hitCallback_YoshiBullet(dEn_c *fishbones, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	mVec3_c hitPos(self->mCollPos.x, self->mCollPos.y, 5500.0f);
	fishbones->hitdamageEffect(hitPos);
	mVec3_c fishbones_center = fishbones->getCenterPos();
	bool dir = dActor_c::getTrgToSrcDir_Main(fishbones->mPos.x + fishbones->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
	dActorMng_c::m_instance->createUpCoin(fishbones_center, dir, 1, 0);
	return fishbones->hitCallback_Shell(self, other);
}

kmWrite32(0x80AD2748, 0x0008881e);
kmWritePointer(0x80af7aa0, &daEnKopone_c__hitCallback_YoshiBullet);

// EN_BEANS_KURIBO
kmWrite32(0x80acd988, 0x0008A2BE); // What?
kmWrite32(0x80ACD9A0, 0x0008A2BE);

// EN_PUKUCOIN
kmWrite32(0x80AD3C60, 0x00088a0e);
kmWrite32(0x80AD3C3C, 0x00088a0e);

// EN_CHOCHIN_ANKON
kmWrite32(0x80ACECF8, 0x0008828e);
kmWrite32(0x80ACED1C, 0x0008828e);

// EN_ROTATION_GHOST
kmWrite32(0x80AD4158, 0x0008880E);
kmWritePointer(0x80b06e48, &daEnTeresa_c__hitCallback_YoshiBullet);

// EN_WATERPAKKUN
kmWrite32(0x80AD5538, 0x003AFFFE);

bool hitCallback_YoshiBullet_False() {
    return false;
}

// EN_BIG_HANACHAN
kmWritePointer(0x80AD7FC8, &hitCallback_YoshiBullet_False);

// EN_BIG_PATAMET
kmWritePointer(0x80B028D8, &hitCallback_YoshiBullet_False);

// EN_BIG_MET
kmWritePointer(0x80AD8A10, &hitCallback_YoshiBullet_False);
kmWrite32(0x80ACDC8C, 0x0000004F);
kmWrite32(0x80ACDC90, 0xFFBAFFBC);

// EN_KING_KILLER
kmWrite32(0x80AD2618, 0x0000004F);
kmWrite32(0x80AD261C, 0x00080806);
kmWritePointer(0x80AF72A8, &hitCallback_YoshiBullet_False);
kmWritePointer(0x80af72c8, &hitCallback_YoshiBullet_False);
kmWritePointer(0x80af72e4, &hitCallback_YoshiBullet_False);

// EN_KILLER
kmWrite32(0x80ad22dc, 0x03000000);
kmWrite32(0x80a49fd8, 0xc044005c);
kmWrite32(0x80a49fdc, 0xc064007c);
//kmWritePointer(0x80af684c, &hitCallback_YoshiBullet_False);

// EN_MAGNUM_KILLER
kmWrite32(0x80AD22B8, 0x03000000);

// EN_POLTER
kmWrite32(0x80AD3ADC, 0x00000047);
kmWrite32(0x80AD3B00, 0x00000047);

// GABON_ROCK
extern int GabonRock_CrushState;
kmBranchDefAsm(0x807F9694, NULL) {
    // r27 = apOther
	// r28 = apThis
	// r29 = objOther
	// r31 = objThis
    lbz r0, 0x2D(r27)
	cmpwi r0, 19
	bne hgrExit

	addi r3, r31, 0x394
	lis r4, GabonRock_CrushState@h
	ori r4, r4, GabonRock_CrushState@l
	lwz r12, 0(r3)
	lwz r12, 0x18(r12)
	mtctr r12
	bctrl

	lbz r0, 0xA1(r28)
	ori r0, r0, 2
	stb r0, 0xA1(r28)
hgrExit:
	lwz r27, 0x2C(r1)
	lwz r28, 0x30(r1)
	lwz r29, 0x34(r1)
	lwz r30, 0x38(r1)
	lwz r31, 0x3C(r1)
	lwz r0, 0x44(r1)
	mtlr r0
	addi r1, r1, 0x40
	blr
}

kmWrite32(0x8095C668, 0x00000010);

// EN_ICICLE
kmWrite32(0x80AD0EB4, 0x0000004F);
kmWrite32(0x80AD0ED8, 0x0000004F);
kmWritePointer(0x80AEFFFC, &hitCallback_YoshiBullet_False);
kmWritePointer(0x80AEFFF8, &hitCallback_YoshiBullet_False);

// FIXATION_ICICLE
kmWrite32(0x80ACFD08, 0x0000004F);
kmWrite32(0x80ACFD2C, 0x0000004F);
kmWritePointer(0x80AE9EEC, &hitCallback_YoshiBullet_False);
kmWritePointer(0x80AE9EE8, &hitCallback_YoshiBullet_False);

// EN_SANBO
bool daEnSanbo_c__hitCallback_YoshiBullet(dEn_c *pokey, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	mVec3_c hitPos(self->getCenterVec().x, self->getCenterVec().y, pokey->mPos.z);
	bool dir = dActor_c::getTrgToSrcDir_Main(pokey->mPos.x + pokey->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
	dActorMng_c::m_instance->createUpCoin(hitPos, dir, 1, 0);
	return pokey->hitCallback_Shell(self, other);
}

kmWritePointer(0x80b07a90, &daEnSanbo_c__hitCallback_YoshiBullet);

bool Pakkun_hitCallback_YoshiBullet(dEn_c *piranha, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	mVec3_c hitPos = piranha->getCenterPos();
	bool dir = dActor_c::getTrgToSrcDir_Main(piranha->mPos.x + piranha->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
	dActorMng_c::m_instance->createUpCoin(hitPos, dir, 1, 0);
	return piranha->hitCallback_Shell(self, other);
}

// EN_UP_DOKAN_PAKKUN
kmWritePointer(0x80ae7bc4, &Pakkun_hitCallback_YoshiBullet);

// EN_DOWN_DOKAN_PAKKUN
kmWritePointer(0x80ae703c, &Pakkun_hitCallback_YoshiBullet);

// EN_RIGHT_DOKAN_PAKKUN
kmWritePointer(0x80ae77ec, &Pakkun_hitCallback_YoshiBullet);

// EN_LEFT_DOKAN_PAKKUN
kmWritePointer(0x80ae7414, &Pakkun_hitCallback_YoshiBullet);

// EN_UP_DOKAN_FPAKKUN
kmWritePointer(0x80ae6c8c, &Pakkun_hitCallback_YoshiBullet);

// EN_DOWN_DOKAN_FPAKKUN
kmWritePointer(0x80ae6164, &Pakkun_hitCallback_YoshiBullet);

// EN_RIGHT_DOKAN_FPAKKUN
kmWritePointer(0x80ae68d4, &Pakkun_hitCallback_YoshiBullet);

// EN_LEFT_DOKAN_FPAKKUN
kmWritePointer(0x80ae651c, &Pakkun_hitCallback_YoshiBullet);

// EN_JIMEN_PAKKUN
kmWritePointer(0x80af32d0, &Pakkun_hitCallback_YoshiBullet);

// EN_JIMEN_BIG_PAKKUN
kmWritePointer(0x80af2bc8, &Pakkun_hitCallback_YoshiBullet);

// EN_JIMEN_FPAKKUN
kmWritePointer(0x80af2ec8, &Pakkun_hitCallback_YoshiBullet);

// EN_JIMEN_BIG_FPAKKUN
kmWritePointer(0x80af28f0, &Pakkun_hitCallback_YoshiBullet);

// EN_MISTMAN
bool daEnMistman_c__hitCallback_YoshiBullet(dEn_c *foo, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	mVec3_c hitPos(self->mCollPos.x, self->mCollPos.y, 5500.0f);
	foo->hitdamageEffect(hitPos);
	mVec3_c foo_center = foo->getCenterPos();
	bool dir = dActor_c::getTrgToSrcDir_Main(foo->mPos.x + foo->mCenterOffs.x, hammer->mPos.x + hammer->mCenterOffs.x);
	dActorMng_c::m_instance->createUpCoin(foo_center, dir, 1, 0);
	return foo->hitCallback_Shell(self, other);
}

kmWritePointer(0x80afcb60, &daEnMistman_c__hitCallback_YoshiBullet);

// EN_CHOROBON
// This one is so stupid... hammers don't reflect after hitting fuzzies, making it possible to kill lines of fuzzies with hammers
// This dumb hack makes hammers reflect after hitting fuzzies
bool daEnChorobon_c__hitCallback_YoshiBullet(dEn_c *fuzzy, dCc_c *self, dCc_c *other) {
	daHammer_c *hammer = (daHammer_c*)other->mpOwner;
	hammer->hitProc_Reflect(self);
	hammer->revengeHitSE();
	return fuzzy->dEn_c::hitCallback_YoshiBullet(self, other);
}

kmWritePointer(0x80ae0770, &daEnChorobon_c__hitCallback_YoshiBullet);

bool Boss_hitCallback_YoshiBullet(dEn_c *boss, dCc_c *self, dCc_c *other) {
    return boss->hitCallback_Shell(self, other);
}

bool Boss_Kokoopa_hitCallback_YoshiBullet(dEn_c *boss, dCc_c *self, dCc_c *other) {
	// Stupid hack
	if (*(u32 *)(((u32)boss) + 0x794) & 2) {
		return false;
	} else {
		return boss->hitCallback_Shell(self, other);
	}
}

// EN_BOSS_CASTLE_LARRY
kmWritePointer(0x80B76CD8, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_LARRY
kmWritePointer(0x80B81F8C, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_LEMMY
kmWritePointer(0x80B777F0, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_LEMMY
kmWritePointer(0x80B82E38, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_WENDY
kmWritePointer(0x80B7ADB8, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_WENDY
kmWritePointer(0x80B8768C, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_LUDWIG
kmWritePointer(0x80B783C8, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_LUDWIG
kmWritePointer(0x80B84120, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_IGGY
kmWritePointer(0x80B75FC8, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_IGGY
kmWritePointer(0x80B7BC70, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_MORTON
kmWritePointer(0x80B790A8, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_MORTON
kmWritePointer(0x80B853F0, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_CASTLE_ROY
kmWritePointer(0x80B79E10, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_ROY
kmWritePointer(0x80B86458, &Boss_Kokoopa_hitCallback_YoshiBullet);

// EN_BOSS_KAMECK
kmWritePointer(0x80B7CC24, &Boss_hitCallback_YoshiBullet);

// EN_BOSS_KOOPA
kmWritePointer(0x80B7E848, &Boss_hitCallback_YoshiBullet);

// EN_BOSS_JR_A
kmWritePointer(0x80B804FC, &Boss_hitCallback_YoshiBullet);

// EN_BOSS_JR_B
kmWritePointer(0x80B80C04, &Boss_hitCallback_YoshiBullet);

// EN_BOSS_JR_C
kmWritePointer(0x80B814B0, &Boss_hitCallback_YoshiBullet);