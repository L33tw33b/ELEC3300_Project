/*
 * synth.h
 *
 *  Created on: Jun 24, 2021
 *      Author: rallen
 */

#ifndef INC_SYNTH_H_
#define INC_SYNTH_H_


#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../Drivers/BSP/STM32F4-Discovery/stm32f4_discovery_audio.h"
#include "wavetable.h"
/*
#include "adsr.h"
#include "biquad.h"
#include "reverb.h"
*/

//Maximum notes played simultaneously Use <= 2 in debug mode, 10 in release mode
#define MAX_VOICES 2

typedef struct {
  uint8_t wave;          // 0: sine, 1: saw, 2: square, 3: tri
  uint8_t voices;        // max number of simultaneous voices FIXME use this


/*
  float attack;          // attack in seconds (0 -> scale)
  float decay;           // decay in seconds  (scale -> scale*sustain)
  float sustain;         // sustain level (0.0-1.0)
  float release;         // release in seconds (scale*sustain -> 0.0)
  float scale;           // max of any one voice (0.0-1.0)
*/

/*
  //                     rlpf - resonant lowpass filter
  uint8_t enable_rlpf;   // 0=disable FIXME use this
  float cutoff;          // cutoff frequency
  float resonance;       // size of peak at cutoff
  //                     reverb

  uint8_t enable_reverb; // 0=disable FIXME use this
  float wet;             // all dry(original) signal=0.0, all wet(reverb)=1.0
  float delay;           // scale reverb delay 1.0=Schroder defaults  (max = 2.0!)
*/

  // synthesis blocks
  wavetable_state_t  wavetables[MAX_VOICES];
/*
  adsr_state_t       envelopes[MAX_VOICES];
  sf_biquad_state_st rlpf;
  reverb_state_t     *reverb; // state too large to put on the stack
*/
  // time
  float synth_time;
} synth_state_t;

#define DEFAULT_VOICES    MAX_VOICES
#define DEFAULT_WAVE      1
#define DEFAULT_SCALE     0.3
/*
#define DEFAULT_ATTACK    0.1
#define DEFAULT_DECAY     0.1
#define DEFAULT_SUSTAIN   0.8
#define DEFAULT_RELEASE   0.1

#define DEFAULT_CUTOFF    900.0
#define DEFAULT_RESONANCE 3.0
#define DEFAULT_WET       0.75
#define DEFAULT_DELAY     1.80
*/

void synth_init(void);
void synth_all_notes_off(void);
void note_off(uint8_t midi_cmd, uint8_t midi_param0, uint8_t midi_param1);
void note_on(uint8_t midi_cmd, uint8_t midi_param0, uint8_t midi_param1);
void audio_init(void);
void update_audio_buffer(uint32_t start_frame, uint32_t num_frames);

void set_wave(uint8_t v);
void set_voices(uint8_t v);
void set_attack(float v);
void set_decay(float v);
void set_sustain(float v);
void set_release(float v);
void set_scale(float v);
void set_cutoff(float v);
void set_resonance(float v);
void set_wet(float v);
void set_delay(float v);

#endif /* INC_SYNTH_H_ */
