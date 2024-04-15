#ifndef INC_WAVETABLE_H_
#define INC_WAVETABLE_H_

#include <stdint.h>

// defines for wave table
#define WAVE_LENGTH 256

// wave table that is used to update the audio_buffer
float sine_wave_table[WAVE_LENGTH];
float saw_wave_table[WAVE_LENGTH];
//Implement Square and triangle wave

typedef struct {
  uint8_t wave;

  float   phase;
  float   phase_inc;

  //Frequency
  int8_t  pitch;
  float   pitch_hz;
} wavetable_state_t;

void wavetable_init(wavetable_state_t *self, uint8_t wave);
void wavetable_note_on(wavetable_state_t *self, int8_t pitch, int8_t velocity);
void wavetable_note_off(wavetable_state_t *self);
void wavetable_get_samples(wavetable_state_t *self, float *out_samples, int frame_count);

#endif /* INC_WAVETABLE_H_ */
