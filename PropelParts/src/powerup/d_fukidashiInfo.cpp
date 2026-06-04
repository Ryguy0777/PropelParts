#include <kamek.h>
#include <game/bases/d_fukidashi_manager.hpp>
#include <constants/game_constants.h>

// Thanks to Palapeli for decompiling this for me
kmBranchDefCpp(0x800B1A50, NULL, void, dfukidashiInfo_c *this_, int playerType, int powerupType) {
    float s_FUKIDASHI_Y_OFFSET[PLAYER_COUNT][NEW_POWERUP_COUNT] = {
      // Mario
      {
        20.0f, // Small
        32.0f, // Mushroom
        32.0f, // Fire Flower
        12.0f, // Mini Mushroom
        38.0f, // Propeller Shroom
        33.0f, // Penguin Suit
        32.0f, // Ice Flower
        32.0f, // Hammer Suit
      },

      // Luigi
      {
        20.0f, // Small
        34.0f, // Mushroom
        34.0f, // Fire Flower
        12.0f, // Mini Mushroom
        39.0f, // Propeller Shroom
        35.0f, // Penguin Suit
        34.0f, // Ice Flower
        34.0f, // Hammer Suit
      },

      // Blue Toad
      {
        20.0f, // Small
        29.0f, // Mushroom
        29.0f, // Fire Flower
        11.0f, // Mini Mushroom
        34.0f, // Propeller Shroom
        29.0f, // Penguin Suit
        29.0f, // Ice Flower
        29.0f, // Hammer Suit
      },

      // Yellow Toad
      {
        20.0f, // Small
        29.0f, // Mushroom
        29.0f, // Fire Flower
        11.0f, // Mini Mushroom
        34.0f, // Propeller Shroom
        29.0f, // Penguin Suit
        29.0f, // Ice Flower
        29.0f, // Hammer Suit
      }
    };

    this_->mPosOffset.x = 0.0;
    this_->mPosOffset.y = s_FUKIDASHI_Y_OFFSET[playerType][powerupType];
    this_->mPosOffset.z = 0.0;
}