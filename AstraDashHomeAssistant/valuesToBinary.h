// Create a 96 length array with 43 and 48 being on
int allTheBits[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int allTheBitsSize = sizeof(allTheBits) / sizeof (allTheBits[0]);

// Define each bit that will need to be set to 1.  Increasing the amount of elements will increase the lit items on the gauge
int oilTemp [] = {63,62,61,60,59};
int oilWarning [] = {64};
int battery [] = {57, 72, 71, 70, 69, 68};
int batteryWarning [] = {58};
int coolantTemp [] = {66, 65, 80, 79, 78, 77};
int coolantTempWarning [] = {67};
int fuelLevel [] = {74, 73, 89, 88, 87, 86, 85, 84, 83, 82, 81, 96, 95, 94, 93, 92, 91, 90};
int fuelLevelWarning [] = {76};
int rpm [] = {11,9,24,23,22,21,20,19,18,17,32,31,30,29,28,27,26,25,33,34,35,36,37,38,39,40,41,42,43};
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

void setBitsBasedOnInput(char const* valueIn, int ceiling, int floor, int bits[], int numberOfSegments) {
  String stringValue = String(valueIn);
  int value = stringValue.toInt();
  const int range = ceiling - floor;
  // Divide this Range by the number of segments.
  float percentage = constrain((float)value / range * 100, 0, 100);
  const int numberOfSegmentsToLight = (float)numberOfSegments / 100 * percentage;
  Serial.print("Number of segments to light up");
  Serial.println(numberOfSegmentsToLight);
  // Serial.println(bitsLit);
  int i = 0;
  int bitsLit = 0;
  while (i < numberOfSegments) {
    Serial.println(i);
    if(bitsLit < numberOfSegmentsToLight) {
      Serial.print("lighting a segment: ");
      Serial.println(bits[i-1]);
      allTheBits[bits[i-1]] = 1;
      bitsLit++;
    } else {
      Serial.print("not lighting a segment: ");
      Serial.println(bits[i-1]); // something is fucking weird here
      allTheBits[bits[i-1]] = 0; // error here
    }
    i++;
  }
}
