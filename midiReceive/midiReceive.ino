  #include "MIDIUSB.h"
  #include "pitchToFrequency.h"
  #include "Tone.h"
// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

Tone notePlayer[4];
uint16_t freq[4];

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  Serial.begin(115200);
  notePlayer[0].begin(2);
  notePlayer[1].begin(3);
  notePlayer[2].begin(4);
  notePlayer[3].begin(5);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      Serial.print("Received: ");
      Serial.print(rx.header, HEX);
      Serial.print("-");
      Serial.print(rx.byte1, HEX);//on off
      Serial.print("-");
      Serial.print(pitchFrequency[rx.byte2]);//note
      Serial.print("-");
      Serial.println(rx.byte3, HEX);//velocity

      on_off_event(pitchFrequency[rx.byte2], rx.header);
    }
  } while (rx.header != 0);
}
void on_off_event(uint16_t frequency, uint8_t header){

  if (header == 0x9){// if on
    for(uint8_t i=0;i<4;i++){
      if (!notePlayer[i].isPlaying()){//if not playing
        Serial.print("Tone gen: ");
        Serial.println(i);
        notePlayer[i].play(frequency);
        freq[i] = frequency;
        break;
      }
    }
  }
  if (header == 0x8){// if off
    for(uint8_t i=0;i<4;i++){
      if (freq[i] == frequency){//if exists
        Serial.print("Tone gen stop: ");
        Serial.println(i);
        notePlayer[i].stop();
        break;
      }
    }
  }
}