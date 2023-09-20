#define SCK 18 //Clk
#define MISO 19
#define MOSI 23 //Data
#define SS 5 // Pulse seperating low and high speed packets


//70ms between packets

byte fuzz;


void setup()
{
  Serial.begin(115200);
  
  pinMode(MISO, INPUT);
  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

  digitalWrite(MOSI, HIGH);       // SS high again 
  digitalWrite(SCK, HIGH);       // SS high again 
  digitalWrite(SS, HIGH);       // SS high again
  delay(100); //Delay 4 seconds for reset.   

  digitalWrite(MOSI, LOW);       // SS high again 
  digitalWrite(SCK, LOW);       // SS high again 
  digitalWrite(SS, LOW);       // SS high again

}



void loop()
{

/*  
  bitBangData(flipByte(0xF7),34); // digit 3 and MPH/KMH
  bitBangData(flipByte(0x5A),34); // digit 2 and 1st 4 segs of revs
  bitBangData(flipByte(0x00),34); // next 8 segs of revs
  bitBangData(flipByte(0x00),34); // next 8 segs of revs
  bitBangData(flipByte(0x00),34); // last 8 segs of revs
  bitBangData(flipByte(0x08),34); // revs colour and red bits
  bitBangData(flipByte(0xFB),34); // digit 1 and 2. All 4 of left indicators
  bitBangData(flipByte(0x82),34); // Oil segments, battery alarm
  bitBangData(flipByte(0x04),34); // battery segments, temp alarm
  bitBangData(flipByte(0x0C),34); // temp segments, fuel alarm
  bitBangData(flipByte(0x00),34); // fuel segments
  bitBangData(flipByte(0xDF),34); // fuel segment end
*/
  bitBangData(flipByte(fuzz),34); // digit 3 and MPH/KMH
  // bitBangData(flipByte(0x00),34); // digit 3 and MPH/KMH
  bitBangData(flipByte(0xF2),34); // digit 2 and 1st 4 segs of revs
  bitBangData(flipByte(0x00),34); // next 8 segs of revs
  bitBangData(flipByte(0xFF),34); // next 8 segs of revs
  bitBangData(flipByte(0xFF),34); // last 8 segs of revs
  bitBangData(flipByte(0xFF),34); // revs colour and red bits
  bitBangData(flipByte(0xFF),34); // digit 1 and 2. All 4 of left indicators  
  bitBangData(flipByte(0xFF),34); // Oil segments, battery alarm
  bitBangData(flipByte(0xFF),34); // battery segments, temp alarm
  bitBangData(flipByte(0xFF),34); // temp segments, fuel alarm
  bitBangData(flipByte(0xFF),34); // fuel segments
  bitBangData(flipByte(0xFF),34); // fuel segment end
 
  digitalWrite(SS, HIGH);       // SS high again 
  delayMicroseconds(2);
  digitalWrite(SS, LOW);       // SS high again  

/*

  bitBangData(flipByte(0x18),10); // data transmission  
  bitBangData(flipByte(0x48),10); // data transmission
  bitBangData(flipByte(0x61),10); // data transmission
  bitBangData(flipByte(0xF7),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x16),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x98),10); // data transmission
  bitBangData(flipByte(0x4A),10); // data transmission
  bitBangData(flipByte(0x92),10); // data transmission
  bitBangData(flipByte(0xE9),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0xFF),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x12),10); // data transmission
  bitBangData(flipByte(0xE9),10); // data transmission
  bitBangData(flipByte(0xA8),10); // data transmission
  bitBangData(flipByte(0xCB),10); // data transmission
  bitBangData(flipByte(0xCE),10); // data transmission
  bitBangData(flipByte(0x54),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x02),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0xE1),10); // data transmission
  bitBangData(flipByte(0xE1),10); // data transmission
  bitBangData(flipByte(0x0A),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x18),10); // data transmission
  bitBangData(flipByte(0x6D),10); // data transmission
  bitBangData(flipByte(0x5B),10); // data transmission
  bitBangData(flipByte(0x06),10); // data transmission
  bitBangData(flipByte(0xFF),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x87),10); // data transmission
  bitBangData(flipByte(0xD7),10); // data transmission
  bitBangData(flipByte(0x89),10); // data transmission
  bitBangData(flipByte(0xC1),10); // data transmission
  bitBangData(flipByte(0x1E),10); // data transmission
  bitBangData(flipByte(0xF0),10); // data transmission
  bitBangData(flipByte(0x1D),10); // data transmission
  bitBangData(flipByte(0x1E),10); // data transmission
  bitBangData(flipByte(0xF0),10); // data transmission
  bitBangData(flipByte(0x1D),10); // data transmission
  bitBangData(flipByte(0x0A),10); // data transmission
  bitBangData(flipByte(0x58),10); // data transmission
  bitBangData(flipByte(0x08),10); // data transmission
  bitBangData(flipByte(0x05),10); // data transmission
  bitBangData(flipByte(0x30),10); // data transmission
  bitBangData(flipByte(0x03),10); // data transmission
  bitBangData(flipByte(0xF8),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
  bitBangData(flipByte(0x20),10); // data transmission
  bitBangData(flipByte(0x20),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x20),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x01),10); // data transmission
  bitBangData(flipByte(0x00),10); // data transmission
*/

fuzz++;

delay(70);
}



byte bitBangData(byte _send,int bit_time)  // This function transmit the data via bitbanging
{
  byte _receive = 0;

  for(int i=0; i<8; i++)  // 8 bits in a byte
  {
    digitalWrite(MOSI, bitRead(_send, i));    // Set MOSI
delayMicroseconds(1);
    digitalWrite(SCK, LOW);                  // SCK high
delayMicroseconds(1);
    digitalWrite(SCK, HIGH);                   // SCK low
delayMicroseconds(bit_time); // Pads this so each bit is approx 36uS
  } 
  digitalWrite(MOSI, LOW);       // 
  return _receive;        // Return the received data
}

byte flipByte(byte c)
     {
       c = ((c>>1)&0x55)|((c<<1)&0xAA);
       c = ((c>>2)&0x33)|((c<<2)&0xCC);
       c = (c>>4) | (c<<4) ;

       return c;
     }
