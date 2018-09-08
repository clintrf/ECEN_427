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
#include "../image_render/image_render.h"

/********************************** macros **********************************/
#define DEFAULT_PLL 0
#define DEFAULT_CODEC 0

/********************************** globals **********************************/
bool sound_done = true;
audio_data_header current_data;

/********************************* functions *********************************/

void sound_state_init(){
  printf("Sound State Machine Initialized!\n");
  current_state = INIT_STATE;
}

void sound_state_machine() {
  switch(current_state) {
    case INIT_STATE:
      config_audio_pll(DEFAULT_PLL);
      config_audio_codec(DEFAULT_CODEC);
      audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH);
      transition = INIT_TRANS;
      break;
    case WALK01_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK1_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_alien_walk_flag(false);
        sound_done = false;
        return_state = WALK01_STATE;
        transition = WALK01_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case WALK02_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK2_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_alien_walk_flag(false);
        sound_done = false;
        return_state = WALK02_STATE;
        transition = WALK02_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case WALK03_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK3_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_alien_walk_flag(false);
        sound_done = false;
        return_state = WALK03_STATE;
        transition = WALK03_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case WALK04_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK4_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_alien_walk_flag(false);
        sound_done = false;
        return_state = WALK04_STATE;
        transition = WALK04_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case SAUCER_ZOOM_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_UFO_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_saucer_zoom_flag(false);
        sound_done = false;
        return_state = SAUCER_ZOOM_STATE;
        transition = SAUCER_ZOOM_TRANS;

      }
      else if (!audio_driver_read()) {
        sound_done = true;
      }
      break;
    case SAUCER_EX_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_UFO_DIE_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_saucer_ex_flag(false);
        sound_done = false;
        transition = SAUCER_EX_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case ALIEN_EX_STATE:
     if(sound_done) {
       current_data = audio_driver_get_data_array(AUDIO_DRIVER_INVADER_DIE_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_alien_ex_flag(false);
        sound_done = false;
        transition = ALIEN_EX_TRANS;
      }
      if (!audio_driver_read()) { sound_done = true; }
      break;
    case TANK_SHOOT_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_LASER_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_shoot_flag(false);
        sound_done = false;
        transition = TANK_SHOOT_TRANS;
      }
      else if (!audio_driver_read()) { sound_done = true; }
      break;
    case TANK_EX_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_PLAYER_DIE_AUDIO);
        audio_driver_write(current_data.sound_data, current_data.num_samples);
        globals_set_tank_ex_flag(false);
        globals_set_tank_dead(false);
        sound_done = false;
        transition = TANK_EX_TRANS;
      }
      else if (!audio_driver_read()) { 
      	image_render_tank(0,0);
      	sound_done = true; 
      }
      break;
    default:
      printf("Should not come here\n");
      break;
  }

/******************************** TRANSITIONS ********************************/
  switch(transition) {
    case INIT_TRANS:
      current_state = WALK01_STATE;
      break;
    case WALK01_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else { current_state = WALK02_STATE; }
      break;
    case WALK02_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else { current_state = WALK03_STATE; }
      break;
    case WALK03_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else { current_state = WALK04_STATE; }
      break;
    case WALK04_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else { current_state = WALK01_STATE; }
      break;
    case SAUCER_ZOOM_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; }
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; }
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; }
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; }
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; }
      else { current_state = WALK01_STATE; }
      break;
    case SAUCER_EX_TRANS:
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case ALIEN_EX_TRANS:
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case TANK_SHOOT_TRANS:
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    case TANK_EX_TRANS:
      if (!sound_done) { current_state = current_state; }
      else { current_state = return_state; }
      break;
    default:
      printf("Error, should not reach default state.\n");
      break;
  }
}
