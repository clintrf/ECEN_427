#include "score_board.h"
#include "../sprites/sprites.h"
#include "../hdmi/hdmi.h"

/********************************** macros ***********************************/
#define TEN_THOUSAND_SCALE 10000                                                // scaler for 10000 digit place
#define THOUSAND_SCALE 1000                                                     // scaler for 1000 digit place
#define HUNDRED_SCALE 100                                                       // scaler for 100 digit place
#define TEN_SCALE 10                                                            // scaler for 10 digit place
#define ONE_SCALE 1                                                             // scaler for 1 digit place
#define BYTES_PER_PIXEL 3                                                       // number of bytes in one pixel
#define DEFAULT_CHAR_SCALE 3                                                    // number to scale the sprites by
#define SCALE_NEXT_LINE_ 640*SPRITES_ONE_PIXEL*10                               // number to determine pixel spacing between letters on the screen
#define CHARACTER_ONE_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of first letter in player name
#define CHARACTER_TWO_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of second letter in player name
#define CHARACTER_THREE_POSITION (SPRITES_SCORE_BOARD_START_POS+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of third letter in player name
#define NUMBER_ONE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(0*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of 10000 digit of player score
#define NUMBER_TWO_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(1*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of 1000 digit of player score
#define NUMBER_THREE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(2*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of 100 digit of player score
#define NUMBER_FOUR_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(3*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of 10 digit of player score
#define NUMBER_FIVE_POSITION ((SPRITES_SCORE_BOARD_START_POS+50*SPRITES_ONE_PIXEL*DEFAULT_CHAR_SCALE)+(SCALE_NEXT_LINE_*i*DEFAULT_CHAR_SCALE)+(4*DEFAULT_CHAR_SCALE*SPRITES_ONE_PIXEL*10)) // position of 1 digit of player score
#define POSTION_0 0                                                             // index number 0
#define POSTION_1 1                                                             // index number 1
#define POSTION_2 2                                                             // index number 2
#define POSTION_3 3                                                             // index number 3
#define POSTION_4 4                                                             // index number 4
#define POSTION_5 5                                                             // index number 5
#define POSTION_6 6                                                             // index number 6
#define POSTION_7 7                                                             // index number 7
#define POSTION_8 8                                                             // index number 8
#define POSTION_9 9                                                             // index number 9
#define POSTION_10 10                                                           // index number 10
#define POSTION_11 11                                                           // index number 11
#define POSTION_12 12                                                           // index number 12
#define POSTION_13 13                                                           // index number 13
#define POSTION_14 14                                                           // index number 14
#define POSTION_15 15                                                           // index number 15
#define POSTION_16 16                                                           // index number 16
#define POSTION_17 17                                                           // index number 17
#define POSTION_18 18                                                           // index number 18
#define POSTION_19 19                                                           // index number 19
#define POSTION_20 20                                                           // index number 20
#define POSTION_21 21                                                           // index number 21
#define POSTION_22 22                                                           // index number 22
#define POSTION_23 23                                                           // index number 23
#define POSTION_24 24                                                           // index number 24
#define POSTION_25 25                                                           // index number 25
#define POSTION_26 26                                                           // index number 26
#define POSTION_27 27                                                           // index number 27
#define POSTION_28 28                                                           // index number 28
#define POSTION_29 29                                                           // index number 29
#define POSTION_30 30                                                           // index number 30
#define POSTION_31 31                                                           // index number 31
#define POSTION_32 32                                                           // index number 32
#define POSTION_33 33                                                           // index number 33
#define POSTION_34 34                                                           // index number 34
#define POSTION_35 35                                                           // index number 35
#define POSTION_36 36                                                           // index number 36
#define TOP_TEN_SCORES 10                                                       // indicate how many scores are in the top ten
#define CHAR_OFFSET 48                                                          // offset for when converting asci to number


/*********************************** globals ***********************************/
char white_test[BYTES_PER_PIXEL] = {0xFF, 0xFF, 0xFF};                          // White pixel color when doing testing
char green_test[BYTES_PER_PIXEL] = {0x00,0x80,0x00};                            // green pixel color when doing testing
struct player_stats high_scores[TOP_TEN_SCORES];                                // tracks the player stats

/*********************************** functions ***********************************/
// converts an integer or char passed into the function out as its array form for printing to the screen
// chrt : the char we wish to convert to an array form
// returns : the array form of the passed in character
const uint32_t *convert_to_array(int chrt){
  char temp = chrt;
  switch(temp){                                                                 // simple switch statement that maps to the correct index containing pixelized version of the letters
    case '0':
      return char_array[POSTION_0];                                             // maping for 0 index
    case '1':
      return char_array[POSTION_1];                                             // maping for 1 index
    case '2':
      return char_array[POSTION_2];                                             // maping for 2 index
    case '3':
      return char_array[POSTION_3];                                             // maping for 3 index
    case '4':
      return char_array[POSTION_4];                                             // maping for 4 index
    case '5':
      return char_array[POSTION_5];                                             // maping for 5 index
    case '6':
      return char_array[POSTION_6];                                             // maping for 6 index
    case '7':
      return char_array[POSTION_7];                                             // maping for 7 index
    case '8':
      return char_array[POSTION_8];                                             // maping for 8 index
    case '9':
      return char_array[POSTION_9];                                             // maping for 9 index
    case ' ':
      return char_array[POSTION_10];                                            // maping for 10 index
    case 'A':
      return char_array[POSTION_11];                                            // maping for 11 index
    case 'B':
      return char_array[POSTION_12];                                            // maping for 12 index
    case 'C':
      return char_array[POSTION_13];                                            // maping for 13 index
    case 'D':
      return char_array[POSTION_14];                                            // maping for 14 index
    case 'E':
      return char_array[POSTION_15];                                            // maping for 15 index
    case 'F':
      return char_array[POSTION_16];                                            // maping for 16 index
    case 'G':
      return char_array[POSTION_17];                                            // maping for 17 index
    case 'H':
      return char_array[POSTION_18];                                            // maping for 18 index
    case 'I':
      return char_array[POSTION_19];                                            // maping for 19 index
    case 'J':
      return char_array[POSTION_20];                                            // maping for 20 index
    case 'K':
      return char_array[POSTION_21];                                            // maping for 21 index
    case 'L':
      return char_array[POSTION_22];                                            // maping for 22 index
    case 'M':
      return char_array[POSTION_23];                                            // maping for 23 index
    case 'N':
      return char_array[POSTION_24];                                            // maping for 24 index
    case 'O':
      return char_array[POSTION_25];                                            // maping for 25 index
    case 'P':
      return char_array[POSTION_26];                                            // maping for 26 index
    case 'Q':
      return char_array[POSTION_27];                                            // maping for 27 index
    case 'R':
      return char_array[POSTION_28];                                            // maping for 28 index
    case 'S':
      return char_array[POSTION_29];                                            // maping for 29 index
    case 'T':
      return char_array[POSTION_30];                                            // maping for 30 index
    case 'U':
      return char_array[POSTION_31];                                            // maping for 31 index
    case 'V':
      return char_array[POSTION_32];                                            // maping for 32 index
    case 'W':
      return char_array[POSTION_33];                                            // maping for 33 index
    case 'X':
      return char_array[POSTION_34];                                            // maping for 34 index
    case 'Y':
      return char_array[POSTION_35];                                            // maping for 35 index
    case 'Z':
      return char_array[POSTION_36];                                            // maping for 36 index
  }
};

// saves the scores to the board data structures in the program
void save_score_board(){
  uint32_t nam1, nam2, nam3, num, num1, num2, num3, num4, num5, test;           // defines name and number variables
  FILE * fp;                                                                    // creates a new file
  fp = fopen("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt", "r"); // opens file
  for(int i = 0; i < TOP_TEN_SCORES; i++){                                      // reads the file and grabs the top scores from the file
    nam1 = getc(fp);                                                            // reads in a letter
    nam2 = getc(fp);                                                            // reads in a letter
    nam3 = getc(fp);                                                            // reads in a letter
    getc(fp);                                                                   // reads in a space
    num1 = getc(fp);                                                            // reads in a number
    num2 = getc(fp);                                                            // reads in a number
    num3 = getc(fp);                                                            // reads in a number
    num4 = getc(fp);                                                            // reads in a number
    num5 = getc(fp);                                                            // reads in a number
    while (test != '\n'){                                                       // goes until the end of file
      test = getc(fp);                                                          // reads in any extra characters in the file
    }

    int temp1 = (TEN_THOUSAND_SCALE*(num1-CHAR_OFFSET));                        // uses read in number to calculate 10000 digit of score
    int temp2 = (THOUSAND_SCALE*(num2-CHAR_OFFSET));                            // uses read in number to calculate 1000 digit of score
    int temp3 = (HUNDRED_SCALE*(num3-CHAR_OFFSET));                             // uses read in number to calculate 100 digit of score
    int temp4 = (TEN_SCALE*(num4-CHAR_OFFSET));                                 // uses read in number to calculate 10 digit of score
    int temp5 = (ONE_SCALE*(num5-CHAR_OFFSET));                                 // uses read in number to calculate 1 digit of score

    num = temp1+temp2+temp3+temp4+temp5;                                        // establihes the high score
    high_scores[i].score = num;                                                 // transfer all the data into the high scores data structure
    high_scores[i].letter1 = nam1;                                              // uses read in letter for name
    high_scores[i].letter2 = nam2;                                              // uses read in letter for name
    high_scores[i].letter3 = nam3;                                              // uses read in letter for name

    for(int j = 0; j < SCORE_BOARD_CHAR_ARRAY_SIZE;j++){                        // converts those numbers to the array version
      high_scores[i].name_char[POSTION_0][j] = convert_to_array(nam1)[j];       // converts name letter into pixels
      high_scores[i].name_char[POSTION_1][j] = convert_to_array(nam2)[j];       // converts name letter into pixels
      high_scores[i].name_char[POSTION_2][j] = convert_to_array(nam3)[j];       // converts name letter into pixels
      high_scores[i].score_char[POSTION_0][j] = convert_to_array(num1)[j];      // converts number letter into pixels
      high_scores[i].score_char[POSTION_1][j] = convert_to_array(num2)[j];      // converts number letter into pixels
      high_scores[i].score_char[POSTION_2][j] = convert_to_array(num3)[j];      // converts number letter into pixels
      high_scores[i].score_char[POSTION_3][j] = convert_to_array(num4)[j];      // converts number letter into pixels
      high_scores[i].score_char[POSTION_4][j] = convert_to_array(num5)[j];      // converts number letter into pixels
    }
  }
  fclose(fp);                                                                   // closes the file
}

// function to print the top 10 high scores
void print_high_scores(){
  for(int i = 0; i < TOP_TEN_SCORES; i++){                                      // prints the high scores onto the screen via for loop
    sprites_render_image(high_scores[i].name_char[POSTION_0],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_ONE_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[POSTION_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_TWO_POSITION,DEFAULT_CHAR_SCALE,white_test);
    sprites_render_image(high_scores[i].name_char[POSTION_2],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,CHARACTER_THREE_POSITION,DEFAULT_CHAR_SCALE,white_test);

    sprites_render_image(high_scores[i].score_char[POSTION_0],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_ONE_POSITION,DEFAULT_CHAR_SCALE,green_test);
    sprites_render_image(high_scores[i].score_char[POSTION_1],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_TWO_POSITION,DEFAULT_CHAR_SCALE,green_test);
    sprites_render_image(high_scores[i].score_char[POSTION_2],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_THREE_POSITION,DEFAULT_CHAR_SCALE,green_test);
    sprites_render_image(high_scores[i].score_char[POSTION_3],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_FOUR_POSITION,DEFAULT_CHAR_SCALE,green_test);
    sprites_render_image(high_scores[i].score_char[POSTION_4],SPRITES_CHARACTER_WIDTH,SPRITES_CHARACTER_HEIGHT,NUMBER_FIVE_POSITION,DEFAULT_CHAR_SCALE,green_test);
  }
}

// updates the scores on the board
// name1 : first character of the players name
// name2 : second character of the players name
// name3 : third character of the players name
// final_score : thescore the player received in the game
void update_stats(uint32_t name1,uint32_t name2 ,uint32_t name3, uint32_t final_score){
  uint16_t insert_flag = 1;
  uint32_t numb1,numb2,numb3,numb4,numb5;
  printf("%s %d\r\n" , "Final Score:", final_score);
  save_score_board();

  FILE * fpp;
  int i;
  /* open the file for writing*/
  fpp = fopen ("/home/xilinx/ECEN_427/lab03/image_render/score_board/high_scores.txt","r+");
  uint32_t whole_numb[SCORE_BOARD_NUMBER_OF_NUMBERS_IN_SCORE];
  for(int i = 0; i < TOP_TEN_SCORES; i++){
    numb1 = ((high_scores[i].score)/TEN_THOUSAND_SCALE);                        // calculates the 10000 digit from the high score
    numb2 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);   // calculates the 1000 digit from the high score
    numb3 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE)/HUNDRED_SCALE);// calculates the 100 digit from the high score
    numb4 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE)/TEN_SCALE);// calculates the 10 digit from the high score
    numb5 = ((high_scores[i].score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE-numb4*TEN_SCALE)/ONE_SCALE);// calculates the 1 digit from the high score

    if(high_scores[i].score > final_score) {                                    // if the player score is lower than a high score, write the high score to the file
      fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , high_scores[i].letter1,high_scores[i].letter2,high_scores[i].letter3,numb1,numb2,numb3,numb4,numb5); // save score to txt file
    }
    else {                                                                      // if the player's score is higher than the current high score position, then write the player score to the file
      if (insert_flag){                                                         // only do this if the insert flg is up
        numb1 = ((final_score)/TEN_THOUSAND_SCALE);                             // calculates the 10000 digit from the high score
        numb2 = ((final_score-numb1*TEN_THOUSAND_SCALE)/THOUSAND_SCALE);        // calculates the 1000 digit from the high score
        numb3 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE)/HUNDRED_SCALE);// calculates the 100 digit from the high score
        numb4 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE)/TEN_SCALE);// calculates the 10 digit from the high score
        numb5 = ((final_score-numb1*TEN_THOUSAND_SCALE-numb2*THOUSAND_SCALE-numb3*HUNDRED_SCALE-numb4*TEN_SCALE)/ONE_SCALE);// calculates the 1 digit from the high score
        fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , name1,name2,name3,numb1,numb2,numb3,numb4,numb5);// save score to the txt file
        insert_flag = 0;                                                        // lower the insert flag
        i--;                                                                    // decreament the index to know how many more high scores to save in
      }
      else{                                                                     // if the insert flag is down, then the high score has already been written
        fprintf (fpp, "%c%c%c %d%d%d%d%d\n" , high_scores[i].letter1,high_scores[i].letter2,high_scores[i].letter3,numb1,numb2,numb3,numb4,numb5); // save score to txt file
      }
    }
  }
  fprintf (fpp, "\n" );                                                         // prints a line return to end of text file
  fprintf (fpp, "\n" );                                                         // prints a line return to end of text file
  fprintf (fpp, "\n" );                                                         // prints a line return to end of text file
  save_score_board();                                                           // save this score board
  fclose(fpp);                                                                  // close the file
}
