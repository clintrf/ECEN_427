#include <stdint.h>
#include <stdio.h>


struct player_stats{
  uint32_t name_char[25][3];
  uint32_t score_char[25][3];
  uint32_t score;
};

void init_score_board();

void print_high_scores();

void update_stats();
