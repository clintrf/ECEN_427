#include <stdint.h>
#include <stdio.h>
/********************************** macros ***********************************/
#define SCORE_BOARD_NUMBER_OF_CHARS_IN_NAME 3
#define SCORE_BOARD_NUMBER_OF_NUMBERS_IN_SCORE 5
#define SCORE_BOARD_CHAR_ARRAY_SIZE 25
/*********************************** globals ***********************************/
// the players stats as he plays the game
struct player_stats{
  uint32_t name_char[SCORE_BOARD_NUMBER_OF_CHARS_IN_NAME][SCORE_BOARD_CHAR_ARRAY_SIZE];
  uint32_t score_char[SCORE_BOARD_NUMBER_OF_NUMBERS_IN_SCORE][SCORE_BOARD_CHAR_ARRAY_SIZE];
  char letter1;
  char letter2;
  char letter3;
  uint32_t score;
};

// a map that goes from a value to the index of the array that the value represents
struct text_map{
  uint32_t index;
  char value;
};

/*********************************** functions ***********************************/
// converts an integer or char passed into the function out as its array form for printing to the screen
// chrt : the char we wish to convert to an array form
// returns : the array form of the passed in character
const uint32_t *convert_to_array(int chrt);

// saves the scores to the board data structures in the program
void save_score_board();

// function to print the top 10 high scores
void print_high_scores();

// updates the scores on the board
// name1 : first character of the players name
// name2 : second character of the players name
// name3 : third character of the players name
// final_score : thescore the player received in the game
void update_stats(uint32_t name1,uint32_t name2 ,uint32_t name3, uint32_t final_score);
