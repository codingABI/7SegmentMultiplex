/*
 * Project: 7SegmentMultiplex
 * Description: Demonstration of segment multiplexing for a 7-segment display with an Arduino Uno/Nano
 *
 * This technic could be used to use a 7-segment display with a common anode or cathode (for example 4041AH)
 * - directly by an Arduino Uno/Nano and
 * - without a special display controller and 
 * - without too much current on microcontroller I/O-pins 
 *   (Max. 40mA per I/O-pin would be allowed for an ATMega328 and 200mA for all I/O-pins in sum.
 *   We will use only ~11mA per I/O-pin and in sum 11mA*"Number of digits" due segment muliplexing, for example 44mA for a 4 digit display)
 * - with only one current limiting resistors per digit and
 * - without bightness flickering depending on how many segments of digit are used
 *
 * Disadvantages:
 * - We use use only 1/8 of the maximum possible bightness of the display
 * - We have to update the display every ~2ms to avoid flickering (for example by timer2)
 *
 * License: CC0
 * Copyright (c) 2022 codingABI
 * 
 * created by codingABI https://github.com/codingABI/7SegmentMultiplex
 * 
 * History:
 * 17.09.2022, Initial version
 */

// 12 pins are needed to controll a 4 digit 7-segment display 
#define SEG_A_PIN 2
#define SEG_B_PIN 3
#define SEG_C_PIN 4
#define SEG_D_PIN 5
#define SEG_E_PIN 6
#define SEG_F_PIN 7
#define SEG_G_PIN 8
#define SEG_DP_PIN 9
#define SEG_DIG1_PIN 10 
#define SEG_DIG2_PIN 11
#define SEG_DIG3_PIN 12
#define SEG_DIG4_PIN 13

// We use 4 digits and each digit has eight segments (including the decimal point)
#define SEG_MAXDIGITS 4
#define SEG_MAXSEGMENTS 8

// Rotary encoder pins
#define ENCODER_CLK_PIN A0
#define ENCODER_DT_PIN  A1

// Global variables for the 7-segment display
byte g_SevenSegmentDigit[SEG_MAXDIGITS]; // Data to be shown on a digit
const byte g_SevenSegmentSelectPin[SEG_MAXDIGITS] = { SEG_DIG1_PIN,SEG_DIG2_PIN,SEG_DIG3_PIN,SEG_DIG4_PIN };
const byte g_SevenSegmentLEDPin[SEG_MAXSEGMENTS] = { SEG_A_PIN,SEG_B_PIN,SEG_C_PIN,SEG_D_PIN,SEG_E_PIN,SEG_F_PIN,SEG_G_PIN,SEG_DP_PIN }; 

// Initialize 7-segment-display variables and pin
void SeventSegmentDisplayInit() {
  for (int i=0;i<SEG_MAXSEGMENTS;i++) pinMode(g_SevenSegmentLEDPin[i],OUTPUT);
  for (int i=0;i<SEG_MAXDIGITS;i++) {
    pinMode(g_SevenSegmentSelectPin[i], OUTPUT);
    digitalWrite(g_SevenSegmentSelectPin[i], HIGH);
    g_SevenSegmentDigit[i]=0;
  }
}

// Refresh display (= Show next segment of 7-segment-display)
void SeventSegmentRefesh() {
  static byte segment = 0;

  // Change current segment on every run
  segment++;
  if (segment >= SEG_MAXSEGMENTS) segment=0;

  // Disable all other segments except the current
  for (int i=0;i<SEG_MAXSEGMENTS;i++) if (i != segment) digitalWrite(g_SevenSegmentLEDPin[i],LOW);

  // Enable all digits where the segment should be enabled. Disable all other digits
  for (int i=0;i<SEG_MAXDIGITS;i++) {
    if (((g_SevenSegmentDigit[i] >> segment) & 1) == 1) {
      // Enable digit
      digitalWrite(g_SevenSegmentSelectPin[i],LOW);
    } else {
      // Disable digit
      digitalWrite(g_SevenSegmentSelectPin[i],HIGH);
    }
  }

  // Enable segment
  digitalWrite(g_SevenSegmentLEDPin[segment],HIGH);
}

/* Set sign for a digit
 *
 * Segments of a digit:
 *    -      A
 *   | |    F B
 *    -      G
 *   | |    E C
 *    -      D
 *      .      DP (=H) 
 */     
void SeventSegmentSet(byte digit, char sign,bool decimalPoint=false) {
  byte segmentValue = 0;

  if (digit >= SEG_MAXDIGITS) return;
  switch (sign) {        //HGFEDCBA (H=DP)
    case ' ':segmentValue=B00000000;break;
    case '0':segmentValue=B00111111;break;
    case '1':segmentValue=B00000110;break;
    case '2':segmentValue=B01011011;break;
    case '3':segmentValue=B01001111;break;
    case '4':segmentValue=B01100110;break;
    case '5':segmentValue=B01101101;break;
    case '6':segmentValue=B01111101;break;
    case '7':segmentValue=B00000111;break;
    case '8':segmentValue=B01111111;break;
    case '9':segmentValue=B01101111;break;
  }
  if (decimalPoint) segmentValue += B10000000;
  g_SevenSegmentDigit[digit]=segmentValue;
}

// Setup
void setup() {
  Serial.begin(115200);
  SeventSegmentDisplayInit();

  // Set '4881' to be displayed
  SeventSegmentSet(0,'4');
  SeventSegmentSet(1,'8');
  SeventSegmentSet(2,'8');
  SeventSegmentSet(3,'1');

}

// Loop
void loop() {
  static int lastClkValue = HIGH;
  static unsigned long updateAfterMS = 1000, lastUpdateAfterMS = 0;
  static unsigned long lastMicros = 0;
  int dtValue, clkValue;

  if (micros()-lastMicros > updateAfterMS*1000) {
    // Refresh display
    SeventSegmentRefesh();

    lastMicros = micros();
  }

    // Read rotary encoder to allow user to change the updates interval 
  clkValue = digitalRead(ENCODER_CLK_PIN);
  if (clkValue != lastClkValue) {
    // There was a change on the CLK pin
    lastClkValue = clkValue;
    dtValue = digitalRead(ENCODER_DT_PIN);
    if (clkValue == LOW && dtValue == HIGH) {
      // clockwise ++
      if (updateAfterMS < 1000) {
        if (updateAfterMS >= 100) updateAfterMS+=100; else if (updateAfterMS >= 20) updateAfterMS+=10; else updateAfterMS ++;
      }
    }
    if (clkValue == LOW && dtValue == LOW) {
      // counter clockwise --
      if (updateAfterMS > 0) { 
        if (updateAfterMS > 100) updateAfterMS-=100; else if (updateAfterMS > 20) updateAfterMS-=10; else updateAfterMS--;
      }
    }
  }

  if (updateAfterMS != lastUpdateAfterMS) { // Send update interval to serial if value has changed
    Serial.print("Update display");
    if (updateAfterMS > 0) {
      Serial.print(" every ");
      Serial.print(updateAfterMS);    
      Serial.print(" ms");
    } else {
      Serial.print(" as often as possible");
    }
    Serial.println();
    lastUpdateAfterMS = updateAfterMS;
  }
}
