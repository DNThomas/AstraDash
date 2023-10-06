// Writes increasing values to the Dials to test each segment.

byte bytes[] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};

void integerToByteWrite (int inputNumber) {
  // Divide the number by 8 and round down.  This gives us the byte to Write to
  int byteToWrite = inputNumber/8;

  Serial.print("Before: ");
  Serial.println(byteToWrite, BIN);

  // Figure out the bit place by taking the input number and offsetting the byte
  int bitPlace = inputNumber - (byteToWrite*8);
  Serial.println(bitPlace);

  // The second parameter of bitWrite is reversed IE 0 is the last bit, 1 is the second to last, 7 is the first..
  // To reverse this we just remove the number we're looking for from 8.
  int bitToWrite = 8-bitPlace;      

  // Write the bit to the byte!
  bitWrite(bytes[byteToWrite], bitToWrite, 1);
  Serial.print("After:  ");
  Serial.println(byteToWrite, BIN);
  delay(1000);
  bitWrite(byteToWrite, bitToWrite, 0);
}
