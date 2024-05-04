#include "wavetable.h"
#include "synthutil.h"
#include <math.h>
#include <string.h>




static uint8_t wavetables_initialized = 0;



void wavetable_sine_init(void);
void wavetable_saw_init(void);
void wavetable_square_init(void);
void wavetable_triangle_init(void);
void wavetable_semi_sine_init(void);
void wavetable_sine_init(void)
{
  float phase_inc = (2.0f * (float)M_PI) / (float)WAVE_LENGTH;
  float phase = 0;
  for (int i = 0; i < WAVE_LENGTH; i++) {
    sine_wave_table[i] = sin(phase);
    phase += phase_inc;
  }
}

void wavetable_saw_init(void)
{
  memset(saw_wave_table, 0, sizeof(float) * WAVE_LENGTH);
  float num_octaves = (int)(FRAME_RATE / 2.0 / 440.0);
  for (int octave = 1; octave < num_octaves; octave++) {
    float phase_inc = (octave * 2.0 * (float)M_PI) / (float)WAVE_LENGTH;
    float phase = 0;
    float sign = (octave & 1) ? -1.0f : 1.0f;
    for (int i = 0; i < WAVE_LENGTH; i++) {
      saw_wave_table[i] += (sign * sin(phase) / octave) * (2.0f / (float)M_PI);
      phase += phase_inc;
    }
  }
}

//Implement Square and trinagle wave here
void wavetable_square_init(void)
{
    for (int i = 0; i < WAVE_LENGTH; i++) {
        // Set half of the wave to a high value and half to a low value
        if (i < WAVE_LENGTH / 2) {
            square_wave_table[i] = 1.0f;  // High amplitude
        } else {
            square_wave_table[i] = -1.0f; // Low amplitude
        }
    }
}

void wavetable_triangle_init(void)
{
    int half_wave_length = WAVE_LENGTH / 2;
    float increment = 2.0f / half_wave_length;  // Increment for the rising edge
    float decrement = -increment;               // Decrement for the falling edge
    float value = -1.0f;                        // Start from the lowest value

    // First half: rising edge
    for (int i = 0; i < half_wave_length; i++) {
        triangle_wave_table[i] = value;
        value += increment;
    }

    // Second half: falling edge
    for (int i = half_wave_length; i < WAVE_LENGTH; i++) {
        triangle_wave_table[i] = value;
        value += decrement;
    }
}

void wavetable_semi_sine_init(void)
{
    float phase_inc = (float)M_PI / (float)WAVE_LENGTH; // Increment to cover half a sine wave
    float phase = 0;
    for (int i = 0; i < WAVE_LENGTH; i++) {
        // Use the sine of the phase to generate wave values
        semi_sine_wave_table[i] = sin(phase);
        phase += phase_inc;

        // Reset the phase after reaching π to repeat the semi-sine wave
        if (phase > M_PI) {
            phase -= M_PI;
        }
    }
}

void wavetable_init(wavetable_state_t *self, uint8_t wave)
{
  if(!wavetables_initialized) {
    wavetable_sine_init();
    wavetable_saw_init();
    wavetable_square_init();
    wavetable_triangle_init();
    wavetable_semi_sine_init();
    wavetables_initialized = 1;
  }
  self->wave      = wave;
  self->phase     = 0;
  self->pitch     = 0;
  self->pitch_hz  = pitch_to_freq(32);
  self->phase_inc = (self->pitch_hz / FRAME_RATE) * WAVE_LENGTH;
}


void wavetable_note_on(wavetable_state_t *self, int8_t pitch, int8_t velocity)
{
  self->phase     = 0;
  self->pitch     = pitch;
  self->pitch_hz  = pitch_to_freq(pitch);
  self->phase_inc = (self->pitch_hz / FRAME_RATE) * WAVE_LENGTH;
}


void wavetable_note_off(wavetable_state_t *self)
{
  self->phase     = 0;
  self->pitch     = 0;
  self->pitch_hz  = pitch_to_freq(32);
  self->pitch_hz  = 0;
  self->phase_inc = (self->pitch_hz / FRAME_RATE) * WAVE_LENGTH;
}


void wavetable_get_samples(wavetable_state_t *self, float *out_samples, int frame_count)
{
  for(int frame = 0; frame < frame_count; frame++) {
    float sample_f;
    switch(self->wave) {
    case 0:
      sample_f = sine_wave_table[(uint32_t)self->phase];
      break;
    case 1:
      sample_f = saw_wave_table[(uint32_t)self->phase];
      break;
    case 2:
    	sample_f = square_wave_table[(uint32_t)self->phase];
    	break;
    case 3:
    	sample_f = triangle_wave_table[(uint32_t)self->phase];
    	break;
    case 4:
    	sample_f = semi_sine_wave_table[(uint32_t)self->phase];
    	break;
    default:
    	sample_f = saw_wave_table[(uint32_t)self->phase];
    	break;
    }
    out_samples[2*frame]   = sample_f;
    out_samples[2*frame+1] = sample_f;
    self->phase += self->phase_inc;
    if(self->phase > WAVE_LENGTH) {
      self->phase -= WAVE_LENGTH;
    }
  }
}
