#include <Arduino.h>
#include <Wire.h>

byte address = 0x1D;

volatile bool isMoving;

enum MMA8452Q_Register
{
	STATUS_MMA8452Q = 0x00,
	OUT_X_MSB = 0x01,
	OUT_X_LSB = 0x02,
	OUT_Y_MSB = 0x03,
	OUT_Y_LSB = 0x04,
	OUT_Z_MSB = 0x05,
	OUT_Z_LSB = 0x06,
	SYSMOD = 0x0B,
	INT_SOURCE = 0x0C,
	WHO_AM_I = 0x0D,
	XYZ_DATA_CFG = 0x0E,
	HP_FILTER_CUTOFF = 0x0F,
	PL_STATUS = 0x10,
	PL_CFG = 0x11,
	PL_COUNT = 0x12,
	PL_BF_ZCOMP = 0x13,
	P_L_THS_REG = 0x14,
	FF_MT_CFG = 0x15,
	FF_MT_SRC = 0x16,
	FF_MT_THS = 0x17,
	FF_MT_COUNT = 0x18,
	TRANSIENT_CFG = 0x1D,
	TRANSIENT_SRC = 0x1E,
	TRANSIENT_THS = 0x1F,
	TRANSIENT_COUNT = 0x20,
	PULSE_CFG = 0x21,
	PULSE_SRC = 0x22,
	PULSE_THSX = 0x23,
	PULSE_THSY = 0x24,
	PULSE_THSZ = 0x25,
	PULSE_TMLT = 0x26,
	PULSE_LTCY = 0x27,
	PULSE_WIND = 0x28,
	ASLP_COUNT = 0x29,
	CTRL_REG1 = 0x2A,
	CTRL_REG2 = 0x2B,
	CTRL_REG3 = 0x2C,
	CTRL_REG4 = 0x2D,
	CTRL_REG5 = 0x2E,
	OFF_X = 0x2F,
	OFF_Y = 0x30,
	OFF_Z = 0x31
};

byte readRegister(byte reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(address, (byte) 1);

  while(!Wire.available());
  return Wire.read();
}

void writeRegister(byte reg, byte data) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void ISR_isMoving() {
  isMoving = digitalRead(2);
  digitalWrite(PB4, !isMoving);
};

void setup() {
  Wire.begin();
  Serial.begin(9600);

  pinMode(PB4, OUTPUT);
  pinMode(2, INPUT);

  byte whoAmI = readRegister(0x0D);
  Serial.printf("Who Am I Byte: 0x%04x\n", whoAmI);

  // Put device into standby mode 0x18
  Serial.printf("CTRL_REG1 initial: 0x%04x\n", readRegister(CTRL_REG1));

  writeRegister(CTRL_REG1, readRegister(CTRL_REG1) & ~(0x01));

  Serial.printf("CTRL_REG1: 0x%04x\n", readRegister(CTRL_REG1));


  // Set configuration register for motion detection
  Serial.printf("FF_MT_CFG initial: 0x%04x\n", readRegister(FF_MT_CFG));

  byte cfg = 0x00; //latch not enabled
  cfg |= 0x40;
  cfg |= 0x08;
  cfg |= 0x10;
  cfg |= 0x20;


  writeRegister(FF_MT_CFG, cfg);

  Serial.printf("FF_MT_CFG: 0x%04x\n", readRegister(FF_MT_CFG));


  //Set threshold of > 3g
  Serial.printf("FF_MT_THS initial: 0x%04x\n", readRegister(FF_MT_THS));
  
  writeRegister(FF_MT_THS, 0x10);

  Serial.printf("FF_MT_THS: 0x%04x\n", readRegister(FF_MT_THS));

  //Set the debounce counter to eliminate false readings for 100 Hz sample rate with requirement of 100 ms timer
  Serial.printf("FF_MT_COUNT initial: 0x%04x\n", readRegister(FF_MT_COUNT));
  
  writeRegister(FF_MT_COUNT, 0x0A);

  Serial.printf("FF_MT_THS: 0x%04x\n", readRegister(FF_MT_COUNT));

  //Enable motion detection Interrupt function in CRTL_REG4
  Serial.printf("CTRL_REG4 initial: 0x%04x\n", readRegister(CTRL_REG4));
  
  writeRegister(CTRL_REG4, 0x04);

  Serial.printf("CTRL_REG4: 0x%04x\n", readRegister(CTRL_REG4));

  //Router Interrupt function to INT1 hardware interrupt pin
  Serial.printf("CTRL_REG5 initial: 0x%04x\n", readRegister(CTRL_REG5));
  
  writeRegister(CTRL_REG5, 0x04);

  Serial.printf("CTRL_REG5: 0x%04x\n", readRegister(CTRL_REG5));

  // Put device in Active mode
  byte ctrlReg1Data = readRegister(CTRL_REG1);
  ctrlReg1Data |= 0x01;
  writeRegister(CTRL_REG1, ctrlReg1Data | 0x01);
  Serial.printf("CTRL_REG1: 0x%04x\n", readRegister(CTRL_REG1));

  attachInterrupt(2, ISR_isMoving, CHANGE);

}

void loop() {
  
}