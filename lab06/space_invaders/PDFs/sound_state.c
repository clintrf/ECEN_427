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
#define DEFAULT_PLL 0        // the default value to pass into the PLL
#define DEFAULT_CODEC 0        // the default value to pass into the CODEC
#define RENDER_DEFAULT 0      // default render value

/********************************** globals **********************************/
bool sound_done = true;         // flag to say if the sound is finish or not
audio_data_header current_data; // stores the value of the audio to play

/********************************* functions *********************************/

void sound_state_init(){  // initialize the state machine
  printf("Sound State Machine Initialized!\n");
  current_state = INIT_STATE; // set the start state of the S.M. to INIT_STATE
}

void sound_state_machine() { // function for the state machine
  switch(current_state) {    // check the current state of the state in S.M.
    case INIT_STATE:
      config_audio_pll(DEFAULT_PLL); // initialize the pll
      config_audio_codec(DEFAULT_CODEC); // initialize the codec
      audio_driver_init(AUDIO_DRIVER_GPIO_FILE_PATH); // initialize the driver
      transition = INIT_TRANS; // set the transition state
      break;
    case WALK01_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK1_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_alien_walk_flag(false); // set walk flag to false
        sound_done = false; // set sound flag to false
        return_state = WALK01_STATE; // update return state
        transition = WALK01_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case WALK02_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK2_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_alien_walk_flag(false); // set walk flag to false
        sound_done = false; // set sound flag to false
        return_state = WALK02_STATE; // update return state
        transition = WALK02_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case WALK03_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK3_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_alien_walk_flag(false); // set walk flag to false
        sound_done = false; // set sound flag to false
        return_state = WALK03_STATE; // update return state
        transition = WALK03_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case WALK04_STATE:
      if(globals_get_alien_walk_flag()) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_WALK4_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_alien_walk_flag(false); // set walk flag to false
        sound_done = false; // set sound flag to false
        return_state = WALK04_STATE; // update return state
        transition = WALK04_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case SAUCER_ZOOM_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_UFO_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_saucer_zoom_flag(false); // set zoom flag to false
        sound_done = false; // set sound flag to false
        return_state = SAUCER_ZOOM_STATE; // update return state
        transition = SAUCER_ZOOM_TRANS; // set the transition state

      }
      else if (!audio_driver_read()) { sound_done = true; }  // set sound done flag high
      break;
    case SAUCER_EX_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_UFO_DIE_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_saucer_ex_flag(false); // set saucer explode flag to false
        sound_done = false; // set sound flag to false
        transition = SAUCER_EX_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case ALIEN_EX_STATE:
     if(sound_done) {
       current_data = audio_driver_get_data_array(AUDIO_DRIVER_INVADER_DIE_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_alien_ex_flag(false); // set alien explode flag to false
        sound_done = false; // set sound flag to false
        transition = ALIEN_EX_TRANS; // set the transition state
      }
      if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case TANK_SHOOT_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_LASER_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_shoot_flag(false); // set walk flag to false
        sound_done = false; // set sound flag to false
        transition = TANK_SHOOT_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { sound_done = true; } // set sound done flag high
      break;
    case TANK_EX_STATE:
      if(sound_done) {
        current_data = audio_driver_get_data_array(AUDIO_DRIVER_PLAYER_DIE_AUDIO); // get the sound data
        audio_driver_write(current_data.sound_data, current_data.num_samples); // play the sound
        globals_set_tank_ex_flag(false); // say the tank is exploding
        globals_set_tank_dead(false); // make the tank alive again
        sound_done = false; // set sound flag to false
        transition = TANK_EX_TRANS; // set the transition state
      }
      else if (!audio_driver_read()) { // set sound done flag high
      	image_render_tank(RENDER_DEFAULT,RENDER_DEFAULT); // re draw the tank alive after sound plays
      	sound_done = true; // set sound flag to true
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
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; } // if tank explode set state
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; } // if alien explode then set state
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; } // if shoot then set state
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; } // if saucer explode then set state
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; } // if saucer move then set state
      else { current_state = WALK02_STATE; } // set the State to got to on next tick
      break;
    case WALK02_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; } // if tank explode set state
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; } // if alien explode then set state
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; } // if shoot then set state
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; } // if saucer explode then set state
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; } // if saucer move then set state
      else { current_state = WALK03_STATE; } // set the State to got to on next tick
      break;
    case WALK03_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; } // if tank explode set state
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; } // if alien explode then set state
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; } // if shoot then set state
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; } // if saucer explode then set state
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; } // if saucer move then set state
      else { current_state = WALK04_STATE; } // set the State to got to on next tick
      break;
    case WALK04_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; } // if tank explode set state
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; } // if alien explode then set state
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; } // if shoot then set state
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; } // if saucer explode then set state
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; } // if saucer move then set state
      else { current_state = WALK01_STATE; } // set the State to got to on next tick
      break;
    case SAUCER_ZOOM_TRANS:
      if (!sound_done) { current_state = current_state; }
      else if(globals_get_tank_ex_flag()) { current_state = TANK_EX_STATE; } // if tank explode set state
      else if(globals_get_alien_ex_flag()) { current_state = ALIEN_EX_STATE; } // if alien explode then set state
      else if(globals_get_shoot_flag()) { current_state = TANK_SHOOT_STATE; } // if shoot then set state
      else if(globals_get_saucer_ex_flag()) { current_state = SAUCER_EX_STATE; } // if saucer explode then set state
      else if(globals_get_saucer_zoom_flag()) { current_state = SAUCER_ZOOM_STATE; } // if saucer move then set state
      else { current_state = WALK01_STATE; } // set the State to got to on next tick
      break;
    case SAUCER_EX_TRANS:
      if (!sound_done) { current_state = current_state; } // if sound is not done playing the stay in same state
      else { current_state = return_state; } // set the State to got to on next tick
      break;
    case ALIEN_EX_TRANS:
      if (!sound_done) { current_state = current_state; } // if sound is not done playing the stay in same state
      else { current_state = return_state; } // set the State to got to on next tick
      break;
    case TANK_SHOOT_TRANS:
      if (!sound_done) { current_state = current_state; } // if sound is not done playing the stay in same state
      else { current_state = return_state; } // set the State to got to on next tick
      break;
    case TANK_EX_TRANS:
      if (!sound_done) { current_state = current_state; } // if sound is not done playing the stay in same state
      else { current_state = return_state; } // set the State to got to on next tick
      break;
    default:
      printf("Error, should not reach default state.\n");
      break;
  }
}
