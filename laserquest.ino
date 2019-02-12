/* File: laserquest
 *  Date : 09-Feb-2019
 *  Author : timt1961
 * Purpose: Test code
 * 
  */
#include <LiquidCrystal.h>

int led= 13;
// Set the interface pins
const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

LiquidCrystal lcd( rs, en, d4, d5, d6, d7);

// const unsigned long max_runtime = 600000; // 10 minutes
const unsigned long max_runtime=62000; // 1 minute 2 seconds
int hit_counter = 0;
unsigned long start_time = 0L;
unsigned long millis_per_min = 60000;

void pak_reset(){
 
  hit_counter = 0;
  start_time = millis();
    
}
void displayStats( unsigned long timeleft, int hits){
  
  // convert seconds to minutes
  unsigned long mins = timeleft/ millis_per_min;
  unsigned long seconds = timeleft % millis_per_min;
  char secs[32];
  sprintf(secs, "%02d", seconds/1000);
  
  Serial.print(secs);
  
 lcd.setCursor(0,0);
 char minstr[32];
 sprintf(minstr, "%02d", mins);
  
 lcd.print( minstr);
 lcd.print(":");
 lcd.print( secs);
 
 lcd.print( " - ");
 lcd.print( hits);
  // DigitalWrite(led, HIGH);
  // delay(1000);
  // DigitalWrite(led, LOW);
  // delay(1000);
  
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pak_reset();
  
  lcd.begin(16,2);
  
  //pinMode(led, OUTPUT);
}

void coolDown() {

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  int time_elapsed = current_time - start_time;
  long time_left = max_runtime - (current_time - start_time);
  bool sensor1 = false;
  bool sensor2 = false;

  //lcdsetCursor(0,1);
 
 
  if (time_left > 0 && hit_counter < 10){
    
//    sensor1 = sensorRead();
//    sensor2 = sensorRead();
    if (sensor1 || sensor2) {
       hit_counter++;
       coolDown();
    }
//    reset = get_reset();
//    if (reset){
//      pak_reset();
//    }
    displayStats(time_left, hit_counter);
    delay(1000);
  }

}
