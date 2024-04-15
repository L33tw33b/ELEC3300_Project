#include "midi.h"

// USB MIDI buffer : max received data 64 bytes
#define RX_BUFF_SIZE 64

extern USBH_HandleTypeDef hUsbHostFS;
uint8_t MIDI_RX_Buffer[RX_BUFF_SIZE]; // MIDI reception buffer

void start_midi(void)
{
  USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE);
}

void USBH_MIDI_ReceiveCallback(USBH_HandleTypeDef *phost) // from usbh.midi.c
{
  // each USB midi package is 4 bytes long
  uint16_t numberOfPackets = USBH_MIDI_GetLastReceivedDataSize(&hUsbHostFS) / 4;
  printf("midi received %d packets.\r\n", numberOfPackets);
  for(uint16_t i = 0; i < numberOfPackets; ++i) {
    uint8_t cin_cable   = MIDI_RX_Buffer[4*i+0];
    uint8_t midi_cmd    = MIDI_RX_Buffer[4*i+1];
    uint8_t midi_param0 = MIDI_RX_Buffer[4*i+2];
    uint8_t midi_param1 = MIDI_RX_Buffer[4*i+3];
    if(cin_cable == (uint8_t)0) { //?
      continue;
    }
    decode_midi(midi_cmd, midi_param0, midi_param1);
  }
  // start a new reception
  USBH_MIDI_Receive(&hUsbHostFS, MIDI_RX_Buffer, RX_BUFF_SIZE);
}

// decode midi input, react to note on/off commands
void decode_midi(uint8_t midi_cmd, uint8_t midi_param0, uint8_t midi_param1)
{

  switch(midi_cmd & 0xf0) {
  case 0x80: // Note off
    note_off(midi_cmd, midi_param0, midi_param1);
    break;
  case 0x90: // Note on
    note_on(midi_cmd, midi_param0, midi_param1);
    break;
    /*case 0xa0: // Aftertouch
  case 0xB0: // Continuous controller
  case 0xC0: // Patch change
  case 0xD0: // Channel Pressure
  case 0xE0: // Pitch bend
  case 0xF0: // (non-musical commands)
    printf("%d: %02x %02x %02x\r\n", i, midi_cmd, midi_param0, midi_param1);
    printf("command not handled\r\n");
    break;*/
  }
}
