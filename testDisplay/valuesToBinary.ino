/*
  Takes input values of:
    {
      speedo: 10,
      revs: 55,
      battery:6, 
      fuel: 10,
      oil:3,
      temp: 50
      // TODO: finmish this off from readme!
    }
  and returns a string of bits IE 11100001011010100101...  with a length of 12*8 = 96
  General problem being solved here is:
    data in: { outdoortemp: 2, battery: 80, indoortemp: 15, carchargelevel: 40, taskstodo: 3, solargen: 16000}
    data out: 11111111*8
  each number IE outdoortemp of 2 will impact the bit values according to some logic.
  firstly we need to know how many potential values can be in the gauage in this case the oil gauge = 5
  example 1: we need floor and ceiling values for each value IE -10 > 40 - then we do , 40--10 = 50 / 5(see above re potential values) = 10% -- 10% of 6 is 0.6 so 1 bar.
  example 2: if the outdoor temp was 30 the maths would be 40--10 = 50 / 5 = 10% -- 10% of 30 is 3 so 3 bars.  

  I think a better range is 0 > 30 fwiw :P

  example 1: now to convert this to the relevant bits in a string..   We have the 96 string stored in a global variable.  We will edit each bit individually.
    first we lookup the corresponding bit in a bit map which looks like this.
  temp1: 54 (1 bar only)
    then we would need to be set to 1 IE setBitsInString(temp, 1)
  example 2: 
  temp3: 54, 42,41 // yes it can be that weird..  so setBitsInString(temp, 3)
  
  TODO: I need to model the temp1, temp2 etc. bit IE
  temp = [54,42,41,40] then I know I onyl want to set the first 3 which I know from the variable I then iterate over with a for loop 
  for ( int j = 0; j < 3; ++j ) // output each array element's value {
      globalBitsVariable[n[j] = 1;
      Serial.print (n[j]) ;
      Serial.print (‘\r’) ;
   } 
*/
void processPayload(byte displayOne, byte displayTwo, byte displayThree, byte displayFour, byte displayFive, byte displaySix, byte displaySeven, byte displayEight, byte displayNine, byte displayTen, byte displayEleven, byte displayTwelve) {
  Serial.print("Processing payload..  ");
  bitBangData(displayOne); // digit 3 and MPH/KMH
/*
              || ignore
                | - middle middle
                 | - top left
                  | - top middle
                   | - top right
                    | - bottom right
                     | - TBD - not digit 3
                      | - TBD - not digit 3
                       | - TBD - not digit 3 

*/
  bitBangData(displayTwo); // digit 2 and 1st 4 segs of revs and bits digit 2 of MPH/KPH
/*
              || ignore
                | - 6kmph
                 | - 3kmph
                  | - Top left digit 2
                   | - Top middle digit 2
                    | - Top right digit 2
                     | - bottom right digit 2
                      | - bottom middle speedo digit 3
                       | - bottom left speedo digit 3

*/
  bitBangData(displayThree); // next 8 segs of revs
/*
              || ignore
                | - 30krpm
                 | - ...
                  | - ...
                   | - ...
                    | - ...
                     | - ...
                      | - ...
                       | - 10k rpm

*/
  bitBangData(displayFour); // next 8 segs of revs
/*
              || ignore
                | - 45k
                 | - ...
                  | - ...
                   | - ...
                    | - ...
                     | - ...
                      | - ...
                       | - 30k rpm

*/
  bitBangData(displayFive); // last 8 segs of revs
/*
              || ignore
                | - 45
                 | - ...
                  | - ...
                   | - ...
                    | - ...
                     | - ...
                      | - ...
                       | - 60k rpm

*/
  bitBangData(displaySix); // revs colour and red bits (0XFF for Red bits on)
/*
              || ignore
                | - 60
                 | - ...
                  | - 70
                   | - Speedo backlight
                    | - TBD
                     | - First Red line?
                      | - TBD
                       | - TBD - 

*/
  bitBangData(displaySeven); // digit 1 and 2(max value of 2 cause speedo) of MPH/KPH & All 4 of left indicators -
/*
              || ignore
                | - Left 4 icons
                 | - TBD
                  | - digit 1 > top middle, bottom left, middle middle, bottom middle
                   | - digit 1 > top right
                    | - digit 1 > bottom right
                     | - digit 2 > bottom middle
                      | - digit 2 > bottom left
                       | - digit 2 > middle section

*/
  bitBangData(displayEight); // Oil segments, battery alarm (0x15 == battery alarm off, 0x13 == battery alarm on) -- first bit of battery
/*
              || ignore
                | - 1st bit of battery
                 | - battery alarm
                  | - final section of oil
                   | - 4th section of oil
                    | - 3rd section of oil
                     | - 2nd section of oil
                      | - 1st section of oil
                       | - oil warning?

*/
  bitBangData(displayNine); // battery segments (6), temp segments(2), temp alarm (1) - 
/*
              || ignore
                | - 2nd bit of temp
                 | - 1st bit of temp
                  | - temp light
                   | - 6th bit of battery
                    | - 5th bit of battery
                     | - 4th bit of battery
                      | - 3rd bit of battery
                       | - 2nd bit of battery

*/
  bitBangData(displayTen); // temp segments, fuel alarm -- I'm not sure about this, I feel like it's fuel...
/*
              || ignore
                | - 2nd bit of fuel
                 | - 1st bit of fuel
                  | - TBD
                   | - fuel alarm
                    | - Final red bit of temp
                     | - 5th bit of temp
                      | - 4th bit of temp
                       | - 3rd bit of temp

*/
  bitBangData(displayEleven); // fuel segments
/*
              || ignore
                | - 9th
                 | - 8th
                  | - 7th
                   | - 6th bit of fuel
                    | - 5th bit of fuiel
                     | - 4th bit of Fuel?
                      | - 3rd bit of Fuel?  Hard to tell
                       | - 2nd bit of Fuel?  Hard to tell

*/
  bitBangData(displayTwelve); // fuel segment end
/*
              || ignore
                | - penultimate bit of fuel?
                 | - final bit of fuel
                  | - 7th bit of fuel
                   | - TBD
                    | - TBD
                     | - TBD?
                      | - TBD
                       | - final bit of fuel, maybe 8?

*/
}


byte bitBangData(byte _send) {
  for(int i=0; i<8; i++) {
    digitalWrite(MOSI, bitRead(_send, i));
    digitalWrite(SCK, LOW);
    digitalWrite(SCK, HIGH);
  }
  return false;
}

