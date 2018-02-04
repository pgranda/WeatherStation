///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Pawel Granda
// Date Created:  12/11/2016
// Brief:   Arduino weather station - using LCD Display, Rain Sensor and DHT11 Humidity and Temperature Sensor  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>                  //standard arduino library for handling Analog IO
#include <dht.h>                   //library for Temperature and Humidity Sensor - DHT 11
#include <LiquidCrystal_I2C.h>     //library for LCD Display

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //initializing the lcd library with the numbers of interface pins

dht DHT;                           //declaring global variable for DHT Sensor

#define DHT11_PIN 13               //defining which pin is a signal pin for DHT 11

const int redLED = 10;             //declaring Red pin to Digital 10 Pin slot
const int greenLED = 8;            //declaring Green pin to Digital 8 Pin slot
const int blueLED = 9;             //declaring Blue pin to Digital 9 Pin slot

const int rainSensorMin = 0;       // Rain Sensor minimum value
const int rainSensorMax = 1024;    // Rain Sensor maximum value
int rainSensorPreviousState = -1;  // Auxillary Variable for Rain Sensor State Check 
bool dhtErrorOccured = false;

int RainSensorSetup();
void RainSensorSetupAndDisplay();
void DHT11Setup();
void DHT11Display();

void setup() 
{
  pinMode(redLED, OUTPUT);         //setting Red pin for RGB Led Light to OUTPUT option
  pinMode(greenLED, OUTPUT);       //setting Green Pin for RGB Led Light to OUTPUT option
  pinMode(blueLED, OUTPUT);        //setting Blue Pin for RGB Led Light to OUTPUT option
  lcd.begin(16, 2);                //setting the number of columns and rows for LCD Display
  lcd.setCursor(0,0);              //setting LCD Cursor on column 0, row 0
  Serial.begin(9600);              //setting the data rate in bits per seconds for serial data transmission
  changeColour(0, 255, 0);          //setting RGB LED to green colour 
}

void loop() 
{
  RainSensorSetupAndDisplay();     //calling RainSensorSetupAndDisplay function
  DHT11Setup();                    //calling DHT11Setup function
  if(dhtErrorOccured==false)
  {
    DHT11Display();                  //calling DHT11Display function
  }
  delay(1000);                     //delaying whole loop code for 1000 miliseconds
}

int RainSensorSetup()
{
  int rainSensorReading = analogRead(A0);                            //creating variable which contains data received from Rain Sensor on Analog Input A0
  return map(rainSensorReading, rainSensorMin, rainSensorMax, 0, 3); //creating variable which will be used for switch loop to choose options depending on its value
}

void RainSensorSetupAndDisplay()
{
  lcd.clear();                                                       //clearing LCD Display Screen
  lcd.setCursor(0,0);
  
  int rainSensorState = RainSensorSetup();
  
  if(rainSensorPreviousState == rainSensorState)
  {
    return;
  }
  switch (rainSensorState)                                              
  {
         case 0:                                                                //Sensor is all wet
                changeColour(255, 0, 0);                                         //red light on LED
                lcd.print("It's raining day!");                                 //printing the sentence
                lcd.setCursor(0,1);                                             //setting cursor to column 0, row 1 
                lcd.print("Take an umbrella with you!");                        
                delay(500);
                
                for(int i=0; i <26; i++)                                        //loop for scrolling the text to the left on LCD Display
                {
                  lcd.scrollDisplayLeft();
                  delay(400);
                }
                break;
         
         case 1:                                                                //Sensor is getting wet
                changeColour(255, 255, 0);                                       //yellow light on LED
                lcd.print("H2O dropping from the sky!");                        //printing the sentence
                delay(500);
                
                for(int i=0; i <26; i++)
                {
                  lcd.scrollDisplayLeft();
                  delay(400);
                }
                break;
         
         case 2:
         case 3:                                                                 //Sensor is dry
                changeColour(0, 255, 0);                                         //green light on LED 
                lcd.print("Take sunglasses! No rain in forecast!");             //printing the sentence
                delay(500);
                
                for(int i=0; i <40; i++)
                {
                  lcd.scrollDisplayLeft();
                  delay(400);
                }
                break;
  }
  rainSensorPreviousState = rainSensorState;                                    //setting the current sensor state to previous state variable
  lcd.clear();
}

void DHT11Setup()
{
  int chk = DHT.read11(DHT11_PIN); //setting up DHT 11
  
  switch (chk)                     //displays if sensor got an error
  {
    case DHTLIB_OK: 
                dhtErrorOccured = false;
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.println("DHT 11 Sensor: Checksum error");
                dhtErrorOccured = true;
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.println("DHT 11 Sensor: Time out error"); 
                dhtErrorOccured = true;
		break;
    default: 
		Serial.println("DHT 11 Sensor: Unknown error");
                dhtErrorOccured = true;
		break;
  }
}

void DHT11Display()
{
  lcd.setCursor(0,0);         //setting cursor to column 0, row 0
  lcd.print("Temp: ");        //printing message to lcd
  lcd.print(DHT.temperature); //printing data to lcd
  lcd.print((char)223);       //printing celsius symbol
  lcd.print("C");             //printing letter C for Celsius
  lcd.print("   ");           //printing spaces for better readability
  lcd.setCursor(0,1);         //setting cursor to column 0, row 1
  lcd.print("Humidity: ");    //printing message to lcd
  lcd.print(DHT.humidity);    //printing data to lcd
  lcd.print("%");             //printing percentage symbol
}

void changeColour(int red, int green, int blue)
{
  analogWrite(redLED, red);     
  analogWrite(greenLED, green); 
  analogWrite(blueLED, blue);  
}
