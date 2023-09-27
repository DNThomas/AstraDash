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
int fuelLevel [] = {74, 73, 88, 87, 86, 85, 84, 83, 82, 81, 96, 95, 94, 93, 92, 91, 90};
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

void setBitsBasedOnInput(char const* valueIn, int ceiling, int floor, int bits[], int segs) {
  String stringValue = String(valueIn);
  int value = stringValue.toInt();
  const int range = ceiling - floor;
  // Divide this Range by the number of segments.
  const int numberOfSegments = sizeof(bits) / sizeof (bits[0]);
  // Serial.println(numberOfSegments);
  // Where 20 sits in 45 as a percentage
  float percentage = (float)value / range * 100;
  const int numberOfSegmentsToLight = (float)segs / 100 * percentage;

  int i = 0;
  while (i < numberOfSegmentsToLight) {
    // Serial.print(oilTemp[i]); // 63 & 62
    allTheBits[bits[i-1]] = 1; // sets the bits to 1
    i++;
  }
}
/*
void setup() {
  Serial.begin(115200);

  // Oil Temp
  const int incomingOilTemp = 40; // temporary example value that Home Assistant sent us.
  int numberOfSegments = sizeof(oilTemp) / sizeof (oilTemp[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingOilTemp, oilTempCeiling, oilTempFloor, oilTemp, numberOfSegments); // set the oil temp bits
  
  // Battery Level
  const int incomingBattery = 50;
  numberOfSegments = sizeof(battery) / sizeof (battery[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingBattery, batteryCeiling, batteryFloor, battery, numberOfSegments); // set the battery level
  
  // Coolant Temp
  const int incomingCoolantTemp = 30;
  numberOfSegments = sizeof(coolantTemp) / sizeof (coolantTemp[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingCoolantTemp, coolantTempCeiling, coolantTempFloor, coolantTemp, numberOfSegments); // set the coolant temp level
  
  // Fuel Level
  const int incomingFuelLevel = 90;
  numberOfSegments = sizeof(fuelLevel) / sizeof (fuelLevel[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingFuelLevel, fuelLevelCeiling, fuelLevelFloor, fuelLevel, numberOfSegments); // set the coolant temp level
  
  // RPM
  const int incomingRPM = 15000;
  numberOfSegments = sizeof(rpm) / sizeof (rpm[0]); // Get size, can't do this in function
  setBitsBasedOnInput(incomingRPM, rpmCeiling, rpmFloor, rpm, numberOfSegments); // set the coolant temp level
  
  int m = 0;
  Serial.println();
  while (m < allTheBitsSize) {
    Serial.print(m); // 0,1,2, etc.
    Serial.print(allTheBits[m]); // 0,1 etc.
    Serial.println();
    m++;
  }
}
*/
