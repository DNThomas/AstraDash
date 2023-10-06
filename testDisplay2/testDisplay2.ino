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
//  processPayload(0b01111000, 0b00111111, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b10011110, 0b01000001, 0b00100000, 0b00010000, 0b00000000, 0b00000000);
  processPayloadX("011110000011111100000000000000000000000000010000100111100100000100100000000100000000000000000000"); // bad string?
    // good string 000111101111110000000000000000000000000000001000011110011000001000000100000010000000000000000000

  delay(1000); // 1
}

void processPayloadX(String s1) {
int m = 0;
byte array[8] = {s1[0],s1[1],s1[2],s1[3],s1[4],s1[5],s1[6],s1[7]};
byte together = 0;
for (int i = 0; i < 8; i++){
     together += (array[i] << (7-i));
}
Serial.println(together);
/*
  int m = 0;
  Serial.println("Updating Dash");
  Serial.println(s1); // 011110000011111100000000000000000000000000010000100111100100000100100000000100000000000000000000
  while (m < 96){ // go through all the bits
    // Serial.print(allTheBits[m]);
    char value = s1[m];
    int someInt = value - '0';
    Serial.print(someInt);
    digitalWrite(MOSI, someInt);
    delayMicroseconds(1);
    digitalWrite(SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(SCK, HIGH);
    m++;
  }
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);
*/
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
  Serial.println();
}


byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
// Serial.println(_send);
    int foo = bitRead(_send, i);
    Serial.print(foo);
    digitalWrite(MOSI, bitRead(_send, i));
//    digitalWrite(MOSI, foo);
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
