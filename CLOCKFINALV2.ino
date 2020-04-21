
#include <Wire.h>
#define CLOCK_ADDRESS 0x68
byte year, month, date, hour, dow, minute, second;
float celsius;
char digit[13] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x73, 0x00};
int TD, T0, T1, Y0, Y1, MO0, MO1, D0, D1, PM, S0, S1, M0, M1, H0, H1;
void setup() {
  Wire.begin();
  DDRD = DDRD | B11111100;
  DDRB = DDRB | B00111111;
}

void loop() {
  getTime();
}

void getTime() {
  byte temp;
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(uint8_t(0x11));
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 2);
  temp = Wire.read(); // Here's the MSB
  celsius = float(temp) + 0.25 * (Wire.read() >> 6);
  Wire.beginTransmission(CLOCK_ADDRESS);
  Wire.write(uint8_t(0x00));
  Wire.endTransmission();
  Wire.requestFrom(CLOCK_ADDRESS, 7);
  second = bcdToDec(Wire.read() & 0x7f);
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0x3f);
  dow = bcdToDec(Wire.read());
  date = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
  if (hour > 12) {
    hour = hour - 12;
    PM = 11;
  } else {
    PM = 10;
    }
  H0 = hour % 10;
  H1 = hour / 10;
  TD = (celsius - ((int) celsius)) * 10;
  T0 = (int) celsius % 10;
  T1 = ((int) celsius) / 10;
  D0 = date % 10;
  D1 = date / 10;
  MO0 = month % 10;
  MO1 = month / 10;
  Y0 = year % 10;
  Y1 = year / 10;
  M0 = minute % 10;
  M1 = minute / 10;
  S0 = second % 10;
  S1 = second / 10;
  writeToDisplay(0x00, TD);
  delay(1);
  writeToDisplay(0x06, T0);
  delay(1);
  writeToDisplay(0x08, T1);
  delay(1);
  writeToDisplay(0x0E, Y0);
  delay(1);
  writeToDisplay(0x10, Y1);
  delay(1);
  writeToDisplay(0x16, MO0);
  delay(1);
  writeToDisplay(0x18, MO1);
  delay(1);
  writeToDisplay(0x1E, D0);
  delay(1);
  writeToDisplay(0x20, D1);
  delay(1);
  writeToDisplay(0x26, PM);
  delay(1);
  writeToDisplay(0x2A, S0);
  delay(1);
  writeToDisplay(0x2C, S1);
  delay(1);
  writeToDisplay(0x32, M0);
  delay(1);
  writeToDisplay(0x34, M1);
  delay(1);
  writeToDisplay(0x3A, H0);
  delay(1);
  writeToDisplay(0x3C, H1);
  delay(1);
}

byte bcdToDec(byte val) {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

void writeToDisplay (int dNo, int toWrite) {
  PORTC = dNo;
  PORTD = digit[toWrite] << 2;
  PORTB = dNo | ((B01000000 & digit[toWrite]) >> 6);
}
