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
const int pin_10 = 10;
const int hitled0 = 7;

const int buzzer  = 9;
const int sensor_LDR_front = A0;
const int sensor_ir_front = A1;
const int sensor_LDR_back = A2;
const int sensor_ir_back  = A3;
const int pin_start = A4;
const int pin_stop = A5;
const unsigned long max_runtime = 600000; // 10 minutes
//const unsigned long max_runtime=62000; // 1 minute 2 seconds
const unsigned long millis_per_min = 60000;

bool suit_on = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("In Setup");
  
  pinMode(pakled0, OUTPUT);
  pinMode(hitled0, OUTPUT);
  pinMode(pin_10, OUTPUT);
  pinMode(pin_start, INPUT); //digital
  pinMode(pin_stop, INPUT);  //digital
  pinMode(buzzer, OUTPUT);
  pak_reset();
  lcd.begin(16,2);
}

void pak_reset(){
 
  hit_counter = 0;
  index = 0;
  previous = 0L;
  digitalWrite(pakled0, LOW);
  digitalWrite(hitled0, LOW);
  digitalWrite(pin_10, LOW);
  noTone(buzzer);
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
 
  // Print time and hits on first line
  lcd.setCursor(0,0);
  lcd.print( minstr);
  lcd.print(":");
  lcd.print( secstr);
 
  lcd.print( " - ");
  lcd.print( hits);
    
}


void blink_suit() {
  unsigned long now = millis();
  
  long elapsed_secs = (now - previous)/333L;
  
  if ( elapsed_secs >= 1L) { //make sure a second has passed
      Serial.print("\nblink\n");
      previous = now;
      switch (index) {
         case 0:
           digitalWrite(pakled0, HIGH);
           break;
         case 1:
           digitalWrite(pakled0, LOW);
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
   // and turn on the buzzer
   // and display hit on second line
   
   digitalWrite(pakled0, LOW);
   // Display the power UP
   digitalWrite(hitled0, HIGH);
   // Turn on the buzzer, freq = 1000Hz
   tone(buzzer, 1000);
   // cursor on second line
   lcd.setCursor(0, 1);
   lcd.print("Hit!");
   
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
   // Buzzer off
   noTone(buzzer);
   // clear LCD second line
   lcd.setCursor(0, 1);
   lcd.print("      ");
}

bool sensorRead(int pin){
  int sensor_value = analogRead(pin);
  char sensstr[32];
  sprintf(sensstr, "sensor %d\n", sensor_value);
  Serial.print(sensstr);
  return (sensor_value > 365) ? true:false;
}


void suit_off() {
  digitalWrite(pakled0, LOW);
  digitalWrite(hitled0, LOW);
  digitalWrite(pin_10, HIGH);
  noTone(buzzer);
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long current_time = millis();
  unsigned long time_elapsed = current_time - start_time;
  long time_left = max_runtime - time_elapsed;
  bool sensor1 = false;
  bool sensor2 = false;
  bool suit_start = false;
  int start_val = HIGH;
  bool suit_stop = false;
  int stop_val = HIGH;
  
  char pinstr[32];
  
  if ( suit_on ) {
    Serial.print("In Suit_on branch true\n");
    
     if (time_left > 0 && hit_counter < 10) {
        displayStats(time_left, hit_counter);
        blink_suit();
        sensor1 = sensorRead(sensor_LDR_front);
        sensor2 = sensorRead(sensor_LDR_back);
        if (sensor1 || sensor2) {
           hit_counter++;
           displayStats(time_left, hit_counter);
           coolDown();
        }
        if ((hit_counter >= 10) || (time_left <= 0L)){
          digitalWrite(pin_10, HIGH);
        }
        start_val = digitalRead(pin_start);
 
        if (start_val == LOW){
          suit_start = true;
        }
        else{
          suit_start = false;
        }
        stop_val= digitalRead(pin_stop);
  
        
        if (stop_val == LOW){
          suit_stop = true;
        }
        else {
          suit_stop = false;
        }
        // both pins pressed at same time -> reset
        if (suit_start && suit_stop){
          Serial.print("===> Resetting suit\n");
          pak_reset();
          delay(10000);
          suit_on = false;
        } else if (suit_stop) {
          Serial.print("======> Stopping suit\n");
          suit_on = false;
        }
       

     }
     else {
       suit_off();
     }
  }
  else {
    int start_val;
    Serial.print ("Suit is off, read pin\n");
    start_val = digitalRead(pin_start);
    if (start_val == LOW){
      Serial.print("=======> Starting Suit\n");
      suit_on = true;
      pak_reset();
    }
    else {
      Serial.print("Turn suit off\n");
      suit_on = false;
    }
 
  }

}
