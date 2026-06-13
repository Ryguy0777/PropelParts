#pragma once

#include <game/bases/d_wm_demo_actor.hpp>
#include <game/bases/d_wm_enemy.hpp>
#include <game/bases/d_player_model_manager.hpp>

class daWmPlayer_c : public dWmDemoActor_c {
public:
    void setEnemyDieByStar(dWmEnemy_c *);

    static bool checkWalkPlayers();
    static bool isPlayerStarMode();

    static void startStarSound();

    u8 mPad1[0x8];
    bool m_18c;
    u8 mPad2[0x3f];
    dPyMdlMng_c mPyMdlMng;
    u8 mPad3[0x54];
    int m_22c;
    int m_230;
    int m_234;
    u8 mPad4[0xc4];
    bool m_2fc;

    static daWmPlayer_c *ms_instance;
};