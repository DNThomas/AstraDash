void processPayload(byte displayOne, byte displayTwo, byte displayThree, byte displayFour, byte displayFive, byte displaySix, byte displaySeven, byte displayEight, byte displayNine, byte displayTen, byte displayEleven, byte displayTwelve) {
  Serial.print("Processing payload..  ");
  bitBangData(displayOne); // digit 3 and MPH/KMH
  bitBangData(displayTwo); // digit 2 and 1st 4 segs of revs and bits digit 2 of MPH/KPH
  bitBangData(displayThree); // next 8 segs of revs
  bitBangData(displayFour); // next 8 segs of revs
  bitBangData(displayFive); // last 8 segs of revs
  bitBangData(displaySix); // revs colour and red bits (0XFF for Red bits on)
  bitBangData(displaySeven); // digit 1 and 2(max value of 2 cause speedo) of MPH/KPH & All 4 of left indicators -
  bitBangData(displayEight); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on) -- first bit of battery
  bitBangData(displayNine); // battery segments (6), temp segments(2), temp alarm (1) - 
  bitBangData(displayTen); // temp segments, fuel alarm -- I'm not sure about this, I feel like it's fuel...
  bitBangData(displayEleven); // fuel segments
  bitBangData(displayTwelve); // fuel segment end
}


byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    digitalWrite(SCK, LOW);
    digitalWrite(SCK, HIGH);
  }
  return false;
}
