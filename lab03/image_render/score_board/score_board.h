#include <stdint.h>
#include <stdio.h>


struct player_stats{
  uint32_t name_char[3][25];
  uint32_t score_char[5][25];
  char letter1;
  char letter2;
  char letter3;
  uint32_t score;
};

struct text_map{
  uint32_t index;
  char value;
};

const uint32_t *convert_to_array();


void save_score_board();

void print_high_scores();

//uint32_t * convert_num(uint32_t final_score);

void update_stats(uint32_t name1,uint32_t name2 ,uint32_t name3, uint32_t final_score);
