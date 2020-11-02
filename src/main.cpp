#include <Arduino.h>
#include <Time.cpp>
#include <Wifi.h>
#include <WebServer.h>
#include "../credentials/info.h"
#include "index.h"

#define DELAY_FACTOR  (100)
#define NUM_OF_DIGITS (4)

#define WIFI_TIMEOUT_MS 20000
const char *password = WIFI_PASSWORD;
const char *network = WIFI_NETWORK;



// sets web server port to 80
WebServer server(80);

// variable to store HTTP request
String header;

void connectToWifi()
{
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(network, password);

  unsigned long startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_TIMEOUT_MS)
  {
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed!");
    // take action
  }
  else
  {
    Serial.print("Connected!");
    Serial.println(WiFi.localIP());

    
  }
}

unsigned long pageCurrTime = millis();
unsigned long pagePrevTime = 0;

/*void webpageHandler()
{
  WiFiClient client = server.available();

  if(client) // if theres a new client connection
  {
    pageCurrTime = millis();
    pagePrevTime = pageCurrTime;
    Serial.println("New Client.");
    String currentLine = ""; // make string to hold incoming data from client
    while (client.connected() && pageCurrTime - pagePrevTime <= WIFI_TIMEOUT_MS) // loop while client is connected
    {

    }
  } 
}
*/

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

Time myTime;

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

void handle_root()
{
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handle_name()
{
  int switchNum = 0;
  bool switchFound = false;
  String name = "Noone";
  for(int j = 0; j < 7; j++)
  {
    if(digitalRead(switchPins[j]) == HIGH)
    {
      switchNum = j;
      switchFound = true;
    }
  }

  switch(switchNum)
  {
    case 1:
      name = "Switch One";
      break;
    case 2:
      name = "Switch Two";
      break;
    case 3:
      name = "Switch Three";
      break;
    case 4:
      name = "Switch Four";
      break;
    case 5:
      name = "Switch Five";
      break;
    case 6:
      name = "Switch Six";
      break;
    default:
      name = "Default / Noone";
      break;
  }

  server.send(200, "text/plane", name);
}

void handle_time()
{
  String s = myTime.fLEDTime();
  String t = "";
  int lenHold = s.length();
  switch(lenHold)
  {
    case 1:
      t = "00:0" + s;
      break;
    case 2:
      t = "00:" + s;
      break;
    case 3:
      t = "0" + s.substring(0,1) + ":" + s.substring(1);
      break;
    case 4:
      t = s.substring(0,2) + ":" + s.substring(2);
      break;
  }
  //if(myTime.LEDTime() % 2 == 1) {Serial.print(myTime.fLEDTime() + " ------ "); Serial.print(lenHold + " ------ "); Serial.println(t);}
  
  server.send(200, "text/plane", t);
}


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

  connectToWifi();
  
  server.on("/", handle_root);
  server.on("/readTime", handle_time);
  server.on("/readName", handle_name);
  server.begin();
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
  server.handleClient();
  
  
  switchFlipped = switchChecker();
  

  if(firstRun)
  {
    myTime.initalizeTime();
    firstRun = false;
  }
  
  ledFunction();
  update_one_digit();
}




