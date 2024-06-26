
#include "synth.h"

#include "synthutil.h"
#include "main.h"
#include "../../Drivers/BSP/STM32F4-Discovery/stm32f4_discovery_audio.h"


#define AUDIO_BUFFER_FRAMES   256
#define AUDIO_BUFFER_CHANNELS 2
#define AUDIO_BUFFER_SAMPLES  AUDIO_BUFFER_FRAMES * AUDIO_BUFFER_CHANNELS
#define AUDIO_BUFFER_BYTES    sizeof(int16_t)*AUDIO_BUFFER_SAMPLES

// volume of hardware DAC.  86 is the max before distortion occurs
#define HARDWARE_VOLUME 86

// ======================================================================
// private vars

// only one synth at a time
synth_state_t the_synth;

// sent to I2S -> DAC
uint16_t audio_buffer[AUDIO_BUFFER_SAMPLES];


//void audio_init(void);
//void update_audio_buffer(uint32_t start_frame, uint32_t num_frames);


void synth_init()
{

  the_synth.voices = DEFAULT_VOICES;
  the_synth.wave = DEFAULT_WAVE;

 the_synth.attack = DEFAULT_ATTACK;
  the_synth.decay = DEFAULT_DECAY;
  the_synth.sustain = DEFAULT_SUSTAIN;
  the_synth.release = DEFAULT_RELEASE;
  the_synth.scale = DEFAULT_SCALE;
  for(int i=0; i < MAX_VOICES; i++) {
    wavetable_init( &(the_synth.wavetables[i]), the_synth.wave );
    adsr_init( &(the_synth.envelopes[i]), the_synth.attack, the_synth.decay, the_synth.sustain, the_synth.release, the_synth.scale);
  }


  the_synth.cutoff = DEFAULT_CUTOFF;
  the_synth.resonance = DEFAULT_RESONANCE;
  the_synth.enable_rlpf = 1;
  sf_lowpass(&(the_synth.rlpf), FRAME_RATE, the_synth.cutoff, the_synth.resonance);
  /*
  the_synth.wet = DEFAULT_WET;
  the_synth.delay = DEFAULT_DELAY;
  the_synth.reverb = (reverb_state_t *)malloc(sizeof(reverb_state_t));
  if(the_synth.reverb == 0) {
    Error_Handler();
  }
  reverb_init(the_synth.reverb, the_synth.wet, the_synth.delay);
*/

  the_synth.synth_time = 0.0;

  audio_init();

}


void set_wave(uint8_t v)
{

  the_synth.wave = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.wavetables[i].wave = v;
  }
}
void set_voices(uint8_t v)
{

  the_synth.voices = v;  // FIXME use this
}
void set_attack(float v)
{

  the_synth.attack = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.envelopes[i].attack = v;
  }
}
void set_decay(float v)
{

  the_synth.decay = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.envelopes[i].decay = v;
  }
}
void set_sustain(float v)
{

  the_synth.sustain = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.envelopes[i].sustain = v;
  }
}
void set_release(float v)
{

  the_synth.release = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.envelopes[i].release = v;
  }
}
void set_scale(float v)
{

  the_synth.scale = v;
  for(int i=0; i < MAX_VOICES; i++) {
    the_synth.envelopes[i].scale = v;
  }
}
/*void set_cutoff(float v)
{

  the_synth.cutoff = v;
  sf_lowpass(&(the_synth.rlpf), FRAME_RATE, the_synth.cutoff, the_synth.resonance);
}
void set_resonance(float v)
{

  the_synth.resonance = v;
  sf_lowpass(&(the_synth.rlpf), FRAME_RATE, the_synth.cutoff, the_synth.resonance);
}
void set_wet(float v)
{

  the_synth.wet = v;
  reverb_init(the_synth.reverb, the_synth.wet, the_synth.delay);
}
void set_delay(float v)
{

  the_synth.delay = v;
  reverb_init(the_synth.reverb, the_synth.wet, the_synth.delay);
}*/

// ======================================================================
// Call this after synthesizer has been initialized
void audio_init(void)
{
  update_audio_buffer(0, AUDIO_BUFFER_FRAMES);

  if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, HARDWARE_VOLUME, FRAME_RATE) != AUDIO_OK) {
    Error_Handler();
  }

  // tell the chip to start DMA from audio_buffer
  BSP_AUDIO_OUT_Play(&(audio_buffer[0]), AUDIO_BUFFER_BYTES);

}

//This can be used as a panic button also
void synth_all_notes_off(void)
{
  for(int i = 0; i < MAX_VOICES; i++) {
    wavetable_note_off( &(the_synth.wavetables[i]) );
    adsr_reset(&(the_synth.envelopes[i]));
  }
}

void note_on(uint8_t midi_cmd, uint8_t midi_param0, uint8_t midi_param1)
{


  int8_t cur_idx = MAX_VOICES;
  for(int8_t j = 0; j < MAX_VOICES; j++) {
    if(0 == adsr_active(&(the_synth.envelopes[j]), the_synth.synth_time)) {
	//if(the_synth.wavetables[j].pitch == 0) {
      // Find a space to store the note
      cur_idx = j;
      break;
    }

  }
  if(cur_idx < MAX_VOICES) {
    wavetable_note_on(&(the_synth.wavetables[cur_idx]), midi_param0, midi_param1);
    adsr_note_on(&(the_synth.envelopes[cur_idx]), midi_param1, the_synth.synth_time);
  }
/*  else {
    printf("Note on:  [NOPE] %d %d\r\n", midi_param0, midi_param1);
  }*/
}

void note_off(uint8_t midi_cmd, uint8_t midi_param0, uint8_t midi_param1)
{

  int8_t cur_idx = MAX_VOICES;
  for(int8_t j = 0; j < MAX_VOICES; j++) {
    if(the_synth.wavetables[j].pitch == midi_param0) {
      if(1 == adsr_releasing(&(the_synth.envelopes[j]), the_synth.synth_time)) {
        // Find which key is being released
        continue;
      }
      cur_idx = j;
      break;
    }
  }
  if(cur_idx < MAX_VOICES) {
    adsr_note_off(&(the_synth.envelopes[cur_idx]), the_synth.synth_time);

	  //wavetable_note_off(&(the_synth.wavetables[cur_idx]));
  }
/*  else {
    printf("Note off: [NOPE] %d %d\r\n", midi_param0, midi_param1);
  }*/
}





void update_audio_buffer(uint32_t start_frame, uint32_t num_frames)
{
  //HAL_GPIO_WritePin(LED_Port, RED_LED, GPIO_PIN_SET);
  static float sample_buffer[3][AUDIO_BUFFER_SAMPLES];

  memset(&(sample_buffer[0][0]), 0, sizeof(float)*AUDIO_BUFFER_SAMPLES);
  memset(&(sample_buffer[1][0]), 0, sizeof(float)*AUDIO_BUFFER_SAMPLES);
  // Osc + Env -> buf1
  for(int note = 0; note < MAX_VOICES; note++) {
    wavetable_get_samples(&(the_synth.wavetables[note]), &(sample_buffer[0][0]), num_frames);
    adsr_get_samples(&(the_synth.envelopes[note]), &(sample_buffer[0][0]), num_frames, the_synth.synth_time);
    for(int i = 0; i < num_frames; i++) {
      sample_buffer[1][2*i] += sample_buffer[0][2*i];
      sample_buffer[1][2*i+1] += sample_buffer[0][2*i+1];
    }
  }

  sf_biquad_process(&(the_synth.rlpf), num_frames, (sf_sample_st *)&(sample_buffer[1][0]), (sf_sample_st *)&(sample_buffer[2][0]));
  // Reverb buf1 -> buf2
  //reverb_get_samples(the_synth.reverb, &(sample_buffer[1][0]), &(sample_buffer[2][0]), num_frames);

  // RLPF buf2 -> buf1
  //sf_biquad_process(&(the_synth.rlpf), num_frames, (sf_sample_st *)&(sample_buffer[2][0]), (sf_sample_st *)&(sample_buffer[1][0]));

  // convert buf1 -> uint16 output buffer
/*  int i = 0;
  int outidx = 1;
  for(int frame = start_frame; frame < start_frame+num_frames; frame++) {
    float sample0_f = sample_buffer[outidx][2*i];
    float sample1_f = sample_buffer[outidx][2*i+1];
    sample0_f = (sample0_f > 1.0) ? sample0_f = 1.0 : (sample0_f < -1.0) ? -1.0 : sample0_f;
    sample1_f = (sample1_f > 1.0) ? sample1_f = 1.0 : (sample1_f < -1.0) ? -1.0 : sample1_f;
    audio_buffer[2*frame] = float2uint16(sample0_f);
    audio_buffer[2*frame+1] = float2uint16(sample1_f);
    i++;
  }*/
  int i = 0;
  for(int frame = start_frame; frame < start_frame+num_frames; frame++) {
    float sample0_f = sample_buffer[2][2*i];
    float sample1_f = sample_buffer[2][2*i+1];
    sample0_f = (sample0_f > 1.0) ? sample0_f = 1.0 : (sample0_f < -1.0) ? -1.0 : sample0_f;
    sample1_f = (sample1_f > 1.0) ? sample1_f = 1.0 : (sample1_f < -1.0) ? -1.0 : sample1_f;
    audio_buffer[2*frame] = float2uint16(sample0_f);
    audio_buffer[2*frame+1] = float2uint16(sample1_f);
    i++;
  }
  //HAL_GPIO_WritePin(LED_Port, RED_LED, GPIO_PIN_RESET);
}

// ======================================================================
// after the first half of the audio_buffer has been transferred, fill
// that portion with new samples while the second half is being played.
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
  update_audio_buffer(0, AUDIO_BUFFER_FRAMES/2);
  the_synth.synth_time += (float)(AUDIO_BUFFER_FRAMES/2)/FRAME_RATE;
}

// ======================================================================
// now the second half has been transferred. fill in the second half
// with new samples.
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
  update_audio_buffer(AUDIO_BUFFER_FRAMES/2, AUDIO_BUFFER_FRAMES/2);
  the_synth.synth_time += (float)(AUDIO_BUFFER_FRAMES/2)/FRAME_RATE;
}


void BSP_AUDIO_OUT_Error_CallBack(void)
{
  printf("AUDIO OUT Error\r\n");
}

