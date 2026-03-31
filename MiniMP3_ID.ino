/* MiniMP3_ID.ino
Author: Forrest Lee Erickson, with Chat GPT.
Date: 20260331
 
Program Requirements asked of Chat GPT:
ESP32 WROOM embedded system. 
We have as one of our preipherials a Mini MP3 player. 
We have two types we my use. 
One is the DFRobot player an another uses the TD5580A IC. 
We use the DFRobot library. I would like setup and loop code. 
I would like the setup code to test and set a flag for the presence of the Mini MP3 player. 
I would like the setup code to test and set a flag for which type of Mini MP3 player. 
The test should NOT depend on the presence of absence of an SD card with MP3 files. 
That test will be developed next. 

*/

//#include <Arduino.h>

#define MP3_RX 16
#define MP3_TX 17

bool mp3_present = false;

enum Mp3Type {
  MP3_NONE = 0,
  MP3_DFROBOT,
  MP3_TD5580A
};

Mp3Type mp3_type = MP3_NONE;

//Functions

//Send Command (DFPlayer Frame)
void sendDFCommand(uint8_t cmd, uint16_t param = 0) {
  uint8_t buf[10];

  buf[0] = 0x7E;
  buf[1] = 0xFF;
  buf[2] = 0x06;
  buf[3] = cmd;
  buf[4] = 0x00;
  buf[5] = (param >> 8) & 0xFF;
  buf[6] = param & 0xFF;

  uint16_t checksum = 0 - (buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]);
  buf[7] = (checksum >> 8) & 0xFF;
  buf[8] = checksum & 0xFF;

  buf[9] = 0xEF;

  Serial2.write(buf, 10);
}

// Read Frame (Arduino Style)
// This is intentionally simple and blocking for startup use.
bool readFrame(uint8_t *buf, uint32_t timeout_ms) {
  uint32_t start = millis();
  int idx = 0;

  while (millis() - start < timeout_ms) {
    if (Serial2.available()) {
      uint8_t b = Serial2.read();

      // Wait for start byte
      if (idx == 0) {
        if (b != 0x7E) continue;
      }

      buf[idx++] = b;

      if (idx == 10) {
        // Validate end byte
        if (buf[9] == 0xEF) {
          return true;
        } else {
          idx = 0; // restart
        }
      }
    }
  }
  return false;
}

//Detection Logic
void detectMP3Module() {
  uint8_t resp[10];

  mp3_present = false;
  mp3_type = MP3_NONE;

  // Flush any garbage
  while (Serial2.available()) Serial2.read();

  // Try a DFPlayer "query status" command
  sendDFCommand(0x3F);

  if (readFrame(resp, 200)) {
    mp3_present = true;

    // Check DFPlayer checksum
    uint16_t sum = 0;
    for (int i = 1; i < 7; i++) {
      sum += resp[i];
    }
    sum = 0 - sum;

    uint16_t received = (resp[7] << 8) | resp[8];

    if (sum == received) {
      mp3_type = MP3_DFROBOT;
    } else {
      mp3_type = MP3_TD5580A;
    }
  }
}


void setup() {
  Serial.begin(115200);

  // Arduino-style UART2 init
  Serial2.begin(9600, SERIAL_8N1, MP3_RX, MP3_TX);

  delay(500);  // Allow module to boot

  detectMP3Module();

  Serial.println();
  Serial.println("==== MP3 Module Detection ====");

  if (!mp3_present) {
    Serial.println("No MP3 module detected");
  } else {
    if (mp3_type == MP3_DFROBOT) {
      Serial.println("DFRobot DFPlayer Mini detected");
    } else if (mp3_type == MP3_TD5580A) {
      Serial.println("TD5580A-based module detected");
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
