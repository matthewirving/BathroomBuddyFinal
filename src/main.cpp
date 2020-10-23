#include <Arduino.h>
#include <Time.cpp>

#define DELAY_FACTOR  (100)
#define NUM_OF_DIGITS (4)

// 4 display on/off pin (for the common anode/cathode)
const int control_pins[NUM_OF_DIGITS] = {5, 18, 19, 21};
// pin 11 of 74HC595 (SHCP)
const int bit_clock_pin = 4;
// pin 12 of 74HC595 (STCP) Latch Pin
const int digit_clock_pin = 16;
// pin 14 of 74HC595 (DS)
const int data_pin = 17;

// digit pattern for a 7-segment display
const byte digit_pattern[16] =
{
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B00000111,  // 7
  B01111111,  // 8
  B01101111,  // 9
  B01110111,  // A
  B01111100,  // b
  B00111001,  // C
  B01011110,  // d
  B01111001,  // E
  B01110001   // F
};

int digit_data[NUM_OF_DIGITS] = {0};
int scan_position = 0;


void update_one_digit()
{
  int i;
  byte pattern;
  
  // turn off all digit

  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    digitalWrite(control_pins[i], HIGH);
  }

  //digitalWrite(control_pins[scan_position], LOW);
  // get the digit pattern of the position to be updated
  pattern = digit_pattern[digit_data[scan_position]];

  // turn off the output of 74HC595
  digitalWrite(digit_clock_pin, LOW);
  
  // update data pattern to be outputed from 74HC595
  // because it's a common anode LED, the pattern needs to be inverted
  shiftOut(data_pin, bit_clock_pin, MSBFIRST, pattern);
  
  // turn on the output of 74HC595
  digitalWrite(digit_clock_pin, HIGH);

  // turn on the digit to be updated in this round
  digitalWrite(control_pins[scan_position], LOW);

  // go to next update position
  scan_position++;
  
  if (scan_position >= NUM_OF_DIGITS)
  {
    scan_position = 0; 
  }

}

int switchPins[] = {13, 12, 14, 27, 26, 25, 33};

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);

  int i;

  // set related pins as output pins
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    pinMode(control_pins[i], OUTPUT);
  }

  pinMode(data_pin, OUTPUT);
  pinMode(bit_clock_pin, OUTPUT);
  pinMode(digit_clock_pin, OUTPUT);  
  
  for (int j = 0; j < 7; j++)
  {
    pinMode(switchPins[j], INPUT_PULLDOWN);
  }

  
}
Time myTime;

int switchCheck = 0;

bool firstRun = true;
bool switchFlipped = false;

bool switchChecker()
{
  switchCheck = 0;
  
  for (int k = 0; k < 7; k++)
  {
  switchCheck += digitalRead(switchPins[k]);
  }
  
  return (switchCheck >= 1 ? true : false);
}

void ledFunction()
{
  int i;
  unsigned long number;
  unsigned long digit_base;


  if(switchFlipped)
  {
    number = myTime.LEDTime();
  }
  else
  {
    number = 0;
    myTime.reset();
  }
 
  digit_base = 10;
  for (i = 0; i < NUM_OF_DIGITS; i++)
  {
    if(digit_base == 10)
    {
      digit_data[i] = number % digit_base;
    } 
    else 
    {
      digit_data[i] = (number % digit_base - (number % (digit_base / 10))) / (digit_base / 10);
    }
    
    digit_base = digit_base * 10;
  }

}

void loop() {
  
  
  
  
  switchFlipped = switchChecker();
  

  if(firstRun)
  {
    myTime.initalizeTime();
    firstRun = false;
  }
  
  ledFunction();
  update_one_digit();
}



