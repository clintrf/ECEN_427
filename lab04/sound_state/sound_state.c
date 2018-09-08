/*
* Sound States
*
* ECEn 427
* Clint Frandsen, Dax Eckles, Seth Becerra
* BYU 2019
*/

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <err.h>
#include "../audio_driver/audio_driver.h" // sgould this be in the header file?
#include "sound_state.h"
#include <stdbool.h>
#include "../globals/globals.h"// sgould this be in the header file?

/********************************** globals **********************************/

bool sound_done = true;

/********************************* functions *********************************/

void sound_state_init(){
  printf("Sound State Machine Initialized!\n")
  current_state = INIT_STATE;
}

void sound_state_machine() {
  switch(current_state) {
    case INIT_STATE:
      printf("INIT_STATE\n");
      config_audio_pll(0);
      config_audio_codec(0);
      audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH);
      transition = INIT_TRANS;
      break;
    case WALK01_STATE:
      printf("WALK01_STATE\n");
      if(sound_done && globals_get_alien_walk_flag()) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_WALK1_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_WALK1_AUDIO).num_samples);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      return_state = WALK01_STATE;
      transition = WALK01_TRANS;
      break;
    case WALK02_STATE:
      printf("WALK02_STATE\n");
      if(sound_done && globals_get_alien_walk_flag()) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_WALK2_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_WALK2_AUDIO).num_samples);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      return_state = WALK02_STATE;
      transition = WALK02_TRANS;
      break;
    case WALK03_STATE:
      printf("WALK03_STATE\n");
      if(sound_done && globals_get_alien_walk_flag()) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_WALK3_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_WALK3_AUDIO).num_samples);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      return_state = WALK03_STATE;
      transition = WALK03_TRANS;
      break;
    case WALK04_STATE:
      printf("WALK04_STATE\n");
      if(sound_done && globals_get_alien_walk_flag()) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_WALK4_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_WALK4_AUDIO).num_samples);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      return_state = WALK04_STATE;
      transition = WALK04_TRANS;
      break;
    case SAUCER_ZOOM_STATE:
      printf("SAUCER_ZOOM_STATE\n");
      if(sound_done) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_UFO_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_UFO_AUDIO).num_samples);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      return_state = SAUCER_ZOOM_STATE;
      transition = SAUCER_ZOOM_TRANS;
      break;
    case SAUCER_EX_STATE:
      printf("SAUCER_EX_STATE\n");
      if(sound_done) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_UFO_DIE_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_UFO_DIE_AUDIO).num_samples);
        globals_set_saucer_ex_flag(false);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      transition = SAUCER_EX_TRANS;
      break;
    case ALIEN_EX_STATE:
      printf("ALIEN_EX_STATE\n");
      if(sound_done) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_INVADER_DIE_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_INVADER_DIE_AUDIO).num_samples);
        globals_set_alien_ex_flag(false);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      transition = ALIEN_EX_TRANS;
      break;
    case TANK_SHOOT_STATE:
      printf("TANK_SHOOT_STATE\n");
      if(sound_done) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_LASER_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_LASER_AUDIO).num_samples);

        globals_set_shoot_flag(false);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      transition = TANK_SHOOT_TRANS;
      break;
    case TANK_EX_STATE:
      printf("TANK_EX_STATE\n");
      if(sound_done) {
        audio_driver_write(
          audio_driver_get_data_array(AUDIO_DRIVER_PLAYER_DIE_AUDIO).sound_data,
          audio_driver_get_data_array(AUDIO_DRIVER_PLAYER_DIE_AUDIO).num_samples);

        globals_set_tank_ex_flag(false);
        sound_done = false;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      transition = TANK_EX_TRANS;
      break;
    default:
      printf("Should not come here\n");
      break;
  }

/******************************** TRANSITIONS ********************************/
  switch(transition) {
    case INIT_TRANS:
      printf("INIT_TRANS\n");
      current_state = WALK01_STATE;
      break;
    case WALK01_TRANS:
      printf("WALK01_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else{ current_state = WALK02_STATE; }
      break;
    case WALK02_TRANS:
      printf("WALK02_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else{ current_state = WALK03_STATE; }
      break;
    case WALK03_TRANS:
      printf("WALK03_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else{ current_state = WALK04_STATE; }
      break;
    case WALK04_TRANS:
      printf("WALK04_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else{ current_state = WALK01_STATE; }
      break;
    case SAUCER_ZOOM_TRANS:
      printf("SAUCER_ZOOM_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else{ current_state = WALK01_STATE; }
      break;
    case SAUCER_EX_TRANS:
      printf("SAUCER_EX_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case ALIEN_EX_TRANS:
      printf("ALIEN_EX_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case TANK_SHOOT_TRANS:
      printf("TANK_SHOOT_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case TANK_EX_TRANS:
      printf("TANK_EX_TRANS\n");
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    default:
      printf("Error, should not reach default state.\n");
      break;
  }
}