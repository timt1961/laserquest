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


int hit_counter = 0;
unsigned long start_time = 0L;
// Previous time we changed led state
unsigned long previous = 0L;
int index =0;
int recharge = 1000;

const int pakled0 = 8;
//const int pakled1 = 9;
//const int pakled2 = 10;
//const int pakled3 = 11;
const int hitled0 = 7;
//const int hitled1 = 2;
//const int hitled2 = 3;
//const int hitled3 = 4;

const int buzzer  = 9;
const int sensor_LDR_front = A0;
const int sensor_ir_front = A1;
const int sensor_LDR_back = A2;
const int sensor_ir_back  = A3;
const int pin_reset = A4;
// const unsigned long max_runtime = 600000; // 10 minutes
const unsigned long max_runtime=62000; // 1 minute 2 seconds
const unsigned long millis_per_min = 60000;




void pak_reset(){
 
  hit_counter = 0;
  index = 0;
  previous = 0L;
  digitalWrite(pakled0, LOW);
  //digitalWrite(pakled1, LOW);
  //digitalWrite(pakled2, LOW);
  //digitalWrite(pakled3, LOW);
  digitalWrite(hitled0, LOW);
  //digitalWrite(hitled1, LOW);
  //digitalWrite(hitled2, LOW);
  //digitalWrite(hitled3, LOW);
  
  start_time = millis();
    
}
void displayStats( unsigned long timeleft, int hits){
  
  // convert seconds to minutes
  unsigned long mins = timeleft/ millis_per_min;
  unsigned long seconds = timeleft % millis_per_min;
  char secstr[32];
  char minstr[32];
  sprintf(secstr, "%02d", seconds/1000);
  sprintf(minstr, "%02d", mins);
  Serial.print(" displayStats: minutes:");
  Serial.print(minstr);
  Serial.print(" displayStats: seconds: ");
  Serial.print(secstr);
  
  Serial.print("\n");
 
  
  lcd.setCursor(0,0);
  lcd.print( minstr);
  lcd.print(":");
  lcd.print( secstr);
 
  lcd.print( " - ");
  lcd.print( hits);
    
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(pakled0, OUTPUT);
  //pinMode(pakled1, OUTPUT);
  //pinMode(pakled2, OUTPUT);
  //pinMode(pakled3, OUTPUT);  
  pinMode(hitled0, OUTPUT);
  //pinMode(hitled1, OUTPUT);
  //pinMode(hitled2, OUTPUT);
  //pinMode(hitled3, OUTPUT);
  pak_reset();
  lcd.begin(16,2);
}

void blink_suit() {
  unsigned long now = millis();
  
  long elapsed_secs = (now - previous)/1000L;
  
  if ( elapsed_secs >= 1L) { //make sure a second has passed
      Serial.print("\nblink\n");
      previous = now;
      switch (index) {
         case 0:
           digitalWrite(pakled0, HIGH);
           //digitalWrite(pakled1, LOW);
           //digitalWrite(pakled2, LOW);
           //digitalWrite(pakled3, LOW);
           break;
         case 1:
           digitalWrite(pakled0, LOW);
           //digitalWrite(pakled1, HIGH);
           //digitalWrite(pakled2, LOW);
           //digitalWrite(pakled3, LOW);
           break;
 
      }  
      index += 1;
      if (index > 1){
        index = 0;    
      }
  }
}

void coolDown() {
   // We've been hit: turn off the suit leds
   digitalWrite(pakled0, LOW);
   // Display the power UP
   digitalWrite(hitled0, HIGH);
   delay( recharge);
   digitalWrite(hitled0, LOW);
   delay(recharge);
   digitalWrite(hitled0, HIGH);
   delay(recharge);
   digitalWrite(hitled0, LOW);
   delay(recharge);
   digitalWrite(hitled0, HIGH);
   delay(recharge);
   digitalWrite(hitled0, LOW);
   
}

bool sensorRead(int pin){
  int sensor_value = analogRead(pin);

  return (sensor_value > 155) ? true:false;
}

bool get_reset(){
  int sensor_value = analogRead(pin_reset);
  return (sensor_value  > 155) ? true: false;
}
void suit_off() {
  digitalWrite(pakled0, LOW);
  digitalWrite(hitled0, LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  unsigned long time_elapsed = current_time - start_time;
  long time_left = max_runtime - time_elapsed;
  bool sensor1 = false;
  bool sensor2 = false;
  bool reset = false;
  char timeleftbuf[32];
  char timeelapsedbuf[32];
  
  
  if (time_left > 0 && hit_counter < 10) {
    //sensor1 = sensorRead(sensor_LDR_front);
    //sensor2 = sensorRead(sensor_LDR_back);
    if (sensor1 || sensor2) {
       hit_counter++;
       coolDown();
    }
    //reset = get_reset();
    if (reset){
       pak_reset();
    }
    displayStats(time_left, hit_counter);
    blink_suit();
    delay(1000);
  }
  else {
    suit_off();
  }

}
