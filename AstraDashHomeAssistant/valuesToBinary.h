byte bytes[] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};

// Define each bit that will need to be set to 1.  Increasing the amount of elements will increase the lit items on the gauge
int oilTemp [] = {63,62,61,60,59};
int oilWarning [] = {64};
int battery [] = {57, 72, 71, 70, 69, 68};
int batteryWarning [] = {58};
int coolantTemp [] = {66, 65, 80, 79, 78, 77};
int coolantTempWarning [] = {67};
int fuelLevel [] = {75, 74, 73, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 96, 95, 94, 93, 92, 91, 90};
int fuelLevelWarning [] = {76};
int rpm [] = {11,9,24,23,22,21,20,19,18,17,32,31,30,29,28,27,26,25,33,34,35,36,37,38,39,40,41,42,43};
int digitthree[3][10] = { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, // 1 TODO
                       { 1, 3, 4, 5, 15, 16 }, // 2 TODO
                       { 1,3,4,5,16 } }; // 3
/*
int digitthree[9] = {
  {1,2},
  {1,2},
  {1,3,4,5},
  {1,2},
  {1,2},
  {1,2},
  {1,2},
  {1,2},
  {1,2}
};
*/
// TODO SPEEDO

// Define floor and ceiling, we will need this to figure out how many sections of a gauge to light up
const int oilTempFloor = -5;
const int oilTempCeiling = 40;
const int batteryFloor = 0;
const int batteryCeiling = 100;
const int coolantTempFloor = -5;
const int coolantTempCeiling = 40;
const int fuelLevelFloor = 0;
const int fuelLevelCeiling = 100;
const int rpmFloor = 0;
const int rpmCeiling = 16000;

void integerToByteWrite (int inputNumber, int value) {
  // Divide the number by 8 and round down.  This gives us the byte to Write to
  int byteToWrite = inputNumber/8;

  // Figure out the bit place by taking the input number and offsetting the byte
  int bitPlace = inputNumber - (byteToWrite*8);

  // The second parameter of bitWrite is reversed IE 0 is the last bit, 1 is the second to last, 7 is the first..
  // To reverse this we just remove the number we're looking for from 8.
  int bitToWrite = 8-bitPlace;

  // Write the bit to the byte!
  bitWrite(bytes[byteToWrite], bitToWrite, value);
}

void setBitsBasedOnInput(char const* valueIn, int ceiling, int floor, int bits[], int numberOfSegments) {
  String stringValue = String(valueIn);
  int value = stringValue.toInt();
  const int range = ceiling - floor;
  // Divide this Range by the number of segments.
  float percentage = constrain((float)value / range * 100, 0, 100);
  const int numberOfSegmentsToLight = (float)numberOfSegments / 100 * percentage;
//  Serial.print("Number of segments to light up: ");
//  Serial.println(numberOfSegmentsToLight);

  int i = 0;
  int bitsLit = 0;
  while (i < numberOfSegments) {
    if(bitsLit < numberOfSegmentsToLight) {
//      Serial.print("lighting a segment: ");
//      Serial.println(bits[i]);
      integerToByteWrite(bits[i], 1);
      bitsLit++;
    } else {
      integerToByteWrite(bits[i], 0);
    }
    i++;
  }
}

