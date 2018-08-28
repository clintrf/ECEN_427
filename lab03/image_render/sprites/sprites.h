#include <stdint.h>
#include <stdio.h>

/********************************** macros ***********************************/
#define SAUCER_WIDTH 16
#define SAUCER_HEIGHT 7
#define ALIEN_EXPLOSION_WIDTH 12
#define ALIEN_EXPLOSION_HEIGHT 10
#define ALIEN_WIDTH 12
#define ALIEN_HEIGHT 8
#define TANK_WIDTH 15
#define TANK_HEIGHT 8
#define BUNKER_WIDTH 24
#define BUNKER_HEIGHT 18
#define BUNKER_DAMAGE_WIDTH 6
#define BUNKER_DAMAGE_HEIGHT 6
#define TANK_BULLET_WIDTH 1
#define ALIEN_BULLET_WIDTH 3
#define BULLET_HEIGHT 5
#define CHARACTER_WIDTH 5
#define CHARACTER_HEIGHT 5
#define SCALE_NEXT_LINE 640*3
#define ONE_PIXEL 3
#define GAME_OVER_SCALING 8
#define NORMAL_CHARACTER_SCALING 3
#define MIDPOINT_WIDTH 960
#define CHARACTER_SPACE_GO 24
#define G_START_LOCATION_GO 360
#define A_START_LOCATION_GO 504
#define M_START_LOCATION_GO 648
#define E_START_LOCATION_GO 792
#define O_START_LOCATION_GO 1008
#define V_START_LOCATION_GO 1152
#define E2_START_LOCATION_GO 1296
#define R_START_LOCATION_GO 1440
#define GAME_OVER_CHARACTER_HEIGHT GAME_OVER_SCALING*CHARACTER_HEIGHT
#define GAME_OVER_END_LINE GAME_OVER_CHARACTER_HEIGHT*SCALE_NEXT_LINE
#define EXTRA_INFORMATION_START_LINE GAME_OVER_END_LINE+(SCALE_NEXT_LINE*10)
#define E_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-60-(45*7)
#define N_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-54-(45*6)
#define T_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-48-(45*5)
#define E2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-42-(45*4)
#define R_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-36-(45*3)
#define Y_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-12-(45*2)
#define O_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960-6-(45*1)
#define U_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+6+(45*0)
#define R2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+12+(45*1)
#define N2_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+36+(45*2)
#define A_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+42+(45*3)
#define M_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+48+(45*4)
#define E3_START_LOCATION_YN EXTRA_INFORMATION_START_LINE+960+54+(45*5)
#define H_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-42-(45*4)
#define I_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-36-(45*3)
#define G_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-30-(45*2)
#define H2_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960-24-(45*1)
#define S_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+(45*0)
#define C_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+6+(45*1)
#define O_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+12+(45*2)
#define R_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+18+(45*3)
#define E_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+24+(45*4)
#define S2_START_LOCATION_HS EXTRA_INFORMATION_START_LINE+960+30+(45*5)

#define TOP_TEN_SCORES 10
#define PIXEL_SPACING 3
#define ONE_PIXEL 3
#define DEFAULT_CHAR_SCALE 3
#define SCORE_BOARD_START_POS 640*100*3+(150*3)
#define SCALE_NEXT_LINE_ 640*ONE_PIXEL*10
#define CHAR_OFFSET 48

static const uint32_t saucer_16x7[] =
{
	0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,0,0,1,1,0,0,1,1,1,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0
};

static const uint32_t alien_explosion_12x10[] =
{
	0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,1,0,0,1,0,0,0,1,0,
	1,0,0,1,0,0,0,0,0,1,0,0,
	0,1,0,0,1,0,0,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,1,
	1,1,0,0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,0,0,1,0,0,1,0,
	0,0,1,0,0,0,0,0,1,0,0,1,
	0,1,0,0,0,1,0,0,1,0,0,0,
	0,0,0,0,0,1,0,0,0,0,0,0
};

static const uint32_t alien_top_in_12x8[] =
{
	0,0,0,0,0,1,1,0,0,0,0,0,
  0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,0,1,0,1,1,0,1,0,0,0,
	0,0,1,0,0,0,0,0,0,1,0,0,
	0,0,0,1,0,0,0,0,1,0,0,0
};

static const uint32_t alien_top_out_12x8[] =
{
	0,0,0,0,0,1,1,0,0,0,0,0,
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,0,0,
	0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,1,0,1,1,0,1,0,0,0,
	0,0,1,0,1,0,0,1,0,1,0,0
};

static const uint32_t alien_middle_in_12x8[] =
{
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,0,1,0,0,0,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,0,0,
	0,0,1,1,0,1,1,1,0,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,1,0,1,0,0,0,0,0,1,0,1,
	0,0,0,0,1,1,0,1,1,0,0,0
};

static const uint32_t alien_middle_out_12x8[] =
{
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,1,0,0,1,0,0,0,1,0,0,1,
	0,1,0,1,1,1,1,1,1,1,0,1,
	0,1,1,1,0,1,1,1,0,1,1,1,
	0,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,1,1,1,1,1,1,0,
	0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,1,0,0,0,0,0,0,0,1,0
};

static const uint32_t alien_bottom_in_12x8[] =
{
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,0,0,1,1,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,1,1,1,0,0,1,1,1,0,0,
	0,1,1,0,0,1,1,0,0,1,1,0,
	0,0,1,1,0,0,0,0,1,1,0,0
};

static const uint32_t alien_bottom_out_12x8[] =
{
	0,0,0,0,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,0,0,1,1,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,
	0,0,0,1,1,0,0,1,1,0,0,0,
	0,0,1,1,0,1,1,0,1,1,0,0,
	1,1,0,0,0,0,0,0,0,0,1,1
};

static const uint32_t tank_15x8[] =
{
	0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};


// Shape of the entire bunker.
static const uint32_t bunker_24x18[] =
{
	0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
	1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1
};

// These are the blocks that comprise the bunker and each time a bullet
// strikes one of these blocks, you erode the block as you sequence through
// these patterns.
static const uint32_t bunkerDamage0_6x6[] = {
	0,1,1,0,0,0,
	0,0,0,0,0,1,
	1,1,0,1,0,0,
	1,0,0,0,0,0,
	0,0,1,1,0,0,
	0,0,0,0,1,0
};

static const uint32_t bunkerDamage1_6x6[] = {
	1,1,1,0,1,0,
	1,0,1,0,0,1,
	1,1,0,1,1,1,
	1,0,0,0,0,0,
	0,1,1,1,0,1,
	0,1,1,0,1,0
};

static const uint32_t bunkerDamage2_6x6[] = {
	1,1,1,1,1,1,
	1,0,1,1,0,1,
	1,1,0,1,1,1,
	1,1,0,1,1,0,
	0,1,1,1,0,1,
	1,1,1,1,1,1
};

static const uint32_t bunkerDamage3_6x6[] = {
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_upper_left_gone_6x6[] = {
	0,0,0,1,1,1,
	0,0,1,1,1,1,
	0,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_upper_right_gone_6x6[] = {
	1,1,1,0,0,0,
	1,1,1,1,0,0,
	1,1,1,1,1,0,
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,1,1
};

static const uint32_t bunker_lower_left_gone_6x6[] = {
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	0,0,1,1,1,1,
	0,0,0,1,1,1,
	0,0,0,0,1,1,
	0,0,0,0,0,1
};

static const uint32_t bunker_lower_right_gone_6x6[] = {
	1,1,1,1,1,1,
	1,1,1,1,1,1,
	1,1,1,1,0,0,
	1,1,1,0,0,0,
	1,1,0,0,0,0,
	1,0,0,0,0,0
};

static const uint32_t tank_explosion1_15x8[] = {
	0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,
	0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,
	0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,
	1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
};

static const uint32_t tank_explosion2_15x8[] = {
	1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
	0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,
	0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,
	1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,
	0,0,0,1,1,1,1,1,1,1,0,0,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
};

static const uint32_t tank_gone_15x8[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static const uint32_t tankbullet_1x5[] = {
	1,
	1,
	1,
	1,
	1
};

static const uint32_t tankbullet_gone_1x5[] = {
	0,
	0,
	0,
	0,
	0
};

static const uint32_t alienbullet1_down_3x5[] = {
	0,1,0,
	0,1,0,
	0,1,0,
	1,1,1,
	0,1,0
};

static const uint32_t alienbullet1_up_3x5[] = {
	0,1,0,
	1,1,1,
	0,1,0,
	0,1,0,
	0,1,0
};

static const uint32_t alienbullet2_up_3x5[] = {
	0,1,0,
	1,0,0,
	0,1,0,
	0,0,1,
	0,1,0
};

static const uint32_t alienbullet2_down_3x5[] = {
	0,1,0,
	0,0,1,
	0,1,0,
	1,0,0,
	0,1,0
};


static const uint32_t alienbullet2_gone_3x5[] = {
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
	0,0,0,
};

static const uint32_t letterA_5x5[] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterB_5x5[] = {
	1,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterC_5x5[] = {
	0,1,1,1,1,
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	0,1,1,1,1
};

static const uint32_t letterD_5x5[] = {
	1,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterE_5x5[] = {
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,0,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t letterF_5x5[] = {
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,0,
	1,0,0,0,0,
	1,0,0,0,0
};

static const uint32_t letterG_5x5[] = {
	0,1,1,1,1,
	1,0,0,0,0,
	1,0,0,1,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterH_5x5[] = {
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterI_5x5[] = {
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterJ_5x5[] = {
	1,1,1,1,1,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	1,1,1,0,0
};

static const uint32_t letterK_5x5[] = {
	1,0,0,0,1,
	1,0,0,1,0,
	1,1,1,0,0,
	1,0,0,1,0,
	1,0,0,0,1
};

static const uint32_t letterL_5x5[] = {
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t letterM_5x5[] = {
	1,0,0,0,1,
	1,1,0,1,1,
	1,0,1,0,1,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterN_5x5[] = {
	1,0,0,0,1,
	1,1,0,0,1,
	1,0,1,0,1,
	1,0,0,1,1,
	1,0,0,0,1
};

static const uint32_t letterO_5x5[] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterP_5x5[] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,0,
	1,0,0,0,0
};

static const uint32_t letterQ_5x5[] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,1,0,
	0,1,1,0,1
};

static const uint32_t letterR_5x5[] = {
	1,1,1,1,0,
	1,0,0,0,1,
	1,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1
};

static const uint32_t letterS_5x5[] = {
	0,1,1,1,1,
	1,0,0,0,0,
	0,1,1,1,0,
	0,0,0,0,1,
	1,1,1,1,0
};

static const uint32_t letterT_5x5[] = {
	1,1,1,1,1,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterU_5x5[] = {
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t letterV_5x5[] = {
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0
};

static const uint32_t letterW_5x5[] = {
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,1,0,1,
	1,0,1,0,1,
	0,1,0,1,0
};

static const uint32_t letterX_5x5[] = {
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0,
	0,1,0,1,0,
	1,0,0,0,1
};

static const uint32_t letterY_5x5[] = {
	1,0,0,0,1,
	0,1,0,1,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0
};

static const uint32_t letterZ_5x5[] = {
	1,1,1,1,1,
	0,0,0,1,0,
	0,0,1,0,0,
	0,1,0,0,0,
	1,1,1,1,1
};

static const uint32_t number0_5x5[] = {
	0,1,1,1,0,
	1,0,0,0,1,
	1,0,0,0,1,
	1,0,0,0,1,
	0,1,1,1,0
};

static const uint32_t number1_5x5[] = {
	1,1,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	0,0,1,0,0,
	1,1,1,1,1
};

static const uint32_t number2_5x5[] = {
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1
};

static const uint32_t number3_5x5[] = {
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number4_5x5[] = {
	1,0,0,0,1,
	1,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	0,0,0,0,1
};

static const uint32_t number5_5x5[] = {
	1,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number6_5x5[] = {
	0,1,1,1,1,
	1,0,0,0,0,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number7_5x5[] = {
	1,1,1,1,1,
	0,0,0,0,1,
	0,0,0,0,1,
	0,0,0,0,1,
	0,0,0,0,1
};

static const uint32_t number8_5x5[] = {
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t number9_5x5[] = {
	1,1,1,1,1,
	1,0,0,0,1,
	1,1,1,1,1,
	0,0,0,0,1,
	1,1,1,1,1
};

static const uint32_t letterBLANK_5x5[] = {
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0,
	0,0,0,0,0
};

static const uint32_t char_array[37][25] = {
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, // 0			0
  {	1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1}, // 1			1
  {	1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1}, // 2			2
  {	1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, // 3			3
  {	1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1}, // 4			4
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, // 5			5
  {	0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1}, // 6			6
  {	1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, // 7			7
  {	1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1}, // 8			8
  {	1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1}, // 9			9
  {	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // BLANK	10
  {	0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1}, // A			11
  {	1,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0}, // B			12
  {	0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1}, // C			13
  {	1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0}, // D			14
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,1,1,1,1}, // E			15
  {	1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0}, // F			16
  {	0,1,1,1,1,1,0,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0}, // G			17
  {	1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1}, // H			18
  {	0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, // I			19
  {	1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,0}, // J			20
  {	1,0,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,1}, // K			21
  {	1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1}, // L			22
  {	1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1}, // M			23
  {	1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1}, // N			24
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, // O			25
  {	0,1,1,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0}, // P			26
  {	0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,1}, // Q			27
  {	1,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1}, // R			28
  {	0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0}, // S			29
  {	1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, // T			30
  {	1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0}, // U			31
  {	1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0}, // V			32
  {	1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0}, // W			33
  {	1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1}, // X			34
  {	1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0}, // Y			35
  {	1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1}  // Z			36
};

static const uint32_t letter_array[37] = {
  0, // 0			0
  1, // 1			1
  2, // 2			2
  3, // 3			3
  4, // 4			4
  5, // 5			5
  6, // 6			6
  7, // 7			7
  8, // 8			8
  9, // 9			9
  ' ', // BLANK	10
  'A', // A			11
  'B', // B			12
  'C', // C			13
  'D', // D			14
  'E', // E			15
  'F', // F			16
  'G', // G			17
  'H', // H			18
  'I', // I			19
  'J', // J			20
  'K', // K			21
  'L', // L			22
  'M', // M			23
  'N', // N			24
  'O', // O			25
  'P', // P			26
  'Q', // Q			27
  'R', // R			28
  'S', // S			29
  'T', // T			30
  'U', // U			31
  'V', // V			32
  'W', // W			33
  'X', // X			34
  'Y', // Y			35
  'Z'  // Z			36
};

// prints an image to the screen
// image[] : the image to be printed
// current_location : top left pixel of the character
// scaling_factor : scale of the figure. must be int. size >= 1. place 1 for normal image
void sprites_render_image(const uint32_t image[], uint32_t width, uint32_t height, uint32_t starting_location, uint32_t scaling_factor, const void *color);
