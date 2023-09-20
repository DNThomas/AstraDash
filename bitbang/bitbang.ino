#define SCK 18 //Clk
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets

byte fuzz;

void setup() {
  Serial.begin(115200);
  
  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);
  digitalWrite(SCK, HIGH);
  digitalWrite(SS, HIGH);
  delay(100);

  digitalWrite(MOSI, LOW);
  digitalWrite(SCK, LOW);
  digitalWrite(SS, LOW);

}



void loop() {
  // bitBangData(flipByte(fuzz),34); // digit 3 and MPH/KMH
  bitBangData(flipByte(0xFF),34); // digit 3 and MPH/KMH
  bitBangData(flipByte(0xFF),34); // digit 2 and 1st 4 segs of revs -- also something on speedo?
  bitBangData(flipByte(0xFF),34); // next 8 segs of revs
  bitBangData(flipByte(0xFF),34); // next 8 segs of revs
  bitBangData(flipByte(0xFF),34); // last 8 segs of revs
  bitBangData(flipByte(0xFF),34); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(flipByte(0xFF),34); // digit 1 and 2 of MPH/KPH. All 4 of left indicators - 
  bitBangData(flipByte(0x15),34); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on)
  bitBangData(flipByte(0xFF),34); // battery segments, temp alarm
  bitBangData(flipByte(0xFF),34); // temp segments, fuel alarm
  bitBangData(flipByte(0xFF),34); // fuel segments
  bitBangData(flipByte(0xFF),34); // fuel segment end
 
  digitalWrite(SS, HIGH);
  delayMicroseconds(2);
  digitalWrite(SS, LOW);

  fuzz++;
  // Serial.write(fuzz);
  delay(100);
}



byte bitBangData(byte _send,int bit_time) {
  byte _receive = 0;
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    delayMicroseconds(1);
    digitalWrite(SCK, LOW);
    delayMicroseconds(1);
    digitalWrite(SCK, HIGH);

    // Pads this so each bit is approx 36uS
    delayMicroseconds(bit_time); 
  }
  digitalWrite(MOSI, LOW);

  // Return the received data
  return _receive;
}

byte flipByte(byte c) {
  c = ((c>>1)&0x55)|((c<<1)&0xAA);
  c = ((c>>2)&0x33)|((c<<2)&0xCC);
  c = (c>>4) | (c<<4) ;
  return c;
}
