// Writes increasing values to the Dials to test each segment.

#define SCK 18 //Clk
#define MOSI 23 //Data

void setup() {
  Serial.begin(115200);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);
  digitalWrite(SCK, HIGH);
  delay(100);

  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
}

void loop () {
  // 0...        // 1        // 2        // 3        // 4        // 5        // 6        // 7        // 8        // 9        // 10       // 11       // 12
  processPayload(0b01111000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b10011110, 0b01000001, 0b00100000, 0b00010000, 0b00000000, 0b00000000);
  delay(1000); // 1
  processPayload(0b00011000, 0b11001100, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b10011000, 0b01000001, 0b00100000, 0b00010000, 0b00000000, 0b00000000);
  delay(1000); // 2
  processPayload(0b10110000, 0b11011011, 0b00111111, 0b00000000, 0b00000000, 0b00010000, 0b10110111, 0b10000010, 0b01000000, 0b01100000, 0b00000000, 0b00000000);
  delay(1000); // 3
  processPayload(0b10111100, 0b11011110, 0b11111111, 0b00000001, 0b00000000, 0b00010000, 0b10110101, 0b10000110, 0b11000001, 0b11100000, 0b00000001, 0b00000000);
  delay(1000); // 4
  processPayload(0b11011000, 0b11101100, 0b11111111, 0b00000111, 0b00000000, 0b00010000, 0b10110001, 0b10001110, 0b11000011, 0b11100001, 0b00000011, 0b00000000);
  delay(1000); // 5
  processPayload(0b11101000, 0b11110110, 0b11111111, 0b00111111, 0b00000000, 0b00010000, 0b10110101, 0b10011110, 0b11000111, 0b11100011, 0b00000111, 0b00010000);
  delay(1000); // 6
  processPayload(0b11101000, 0b11110111, 0b11111111, 0b11111111, 0b00000001, 0b00010000, 0b10110111, 0b10111110, 0b11001111, 0b11100111, 0b00011111, 0b10100000);
  delay(1000); // 7 
  processPayload(0b00111000, 0b11011100, 0b11111111, 0b11111111, 0b00000111, 0b00010000, 0b10110000, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000); // 8
  processPayload(0b11111000, 0b11111111, 0b11111111, 0b11111111, 0b00011111, 0b00010000, 0b10110111, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000); // 9
  processPayload(0b11111000, 0b11111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10110101, 0b10111110, 0b11011111, 0b11101111, 0b111111111, 0b11100001);
  delay(1000);
}


void processPayload(byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7, byte b8, byte b9, byte b10, byte b11, byte b12) {
  Serial.println("Processing payload");
  // bitBangDatflipByte(fuzz)); // digit 3 and MPH/KMH
  bitBangData(b1); // digit 3 and MPH/KMH
  bitBangData(b2); // digit 2 and 1st 4 segs of revs -- also something on speedo?
  bitBangData(b3); // next 8 segs of revs
  bitBangData(b4); // next 8 segs of revs
  bitBangData(b5); // last 8 segs of revs
  bitBangData(b6); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(b7); // digit 1 and 2 of MPH/KPH. All 4 of left indicators -
  bitBangData(b8); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on)
  bitBangData(b9); // battery segments, temp alarm
  bitBangData(b10); // temp segments, fuel alarm
  bitBangData(b11); // fuel segments
  bitBangData(b12); // fuel segment end
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
}

byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    delayMicroseconds(1);
    digitalWrite(SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(SCK, HIGH);
    // Pads this so each bit is approx 34ms
    delayMicroseconds(30); 
  }
  digitalWrite(MOSI, LOW);
  return false;
}
