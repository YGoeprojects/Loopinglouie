/*
Arduino looping louie with light barriers conting lives of the player and controlling the players "play light"
and variable motor control.  
*/

//Define everything 
//Motor = PWM Pin for variable motor control; randomMode control Pin which state the game is in 
#define motor 3
#define randomMode 6
int motorSpeed = 0;
int delayTime = 0;

// Light barriers 1-4 + lives + mean for comparison and readout variable
int ifRead1; 
int ifReadmean1 = 0; 
int ifReadlives1 = 4; 
int ifRead2;
int ifReadmean2 = 0; 
int ifReadlives2 = 3;  
int ifRead3;
int ifReadmean3 = 0; 
int ifReadlives3 = 4;  
int ifRead4;
int ifReadmean4 = 0; 
int ifReadlives4 = 4; 
int counterifread1;
int counterifread2; 
int counterifread3;  
int counterifread4; 

// Some timer used for motor control period 
int variableTime = 0; 
 
// Set up of timers used for save period after life lost or events like calculating new meanvalue for barrier comparison or new motor movement event  
unsigned long lastMillis1 = millis(); 
unsigned long lastMillis2 = millis(); 
unsigned long savetimer1; // Timer during which a player cant lose a life needed for first setup
unsigned long ifReadmeantimer1 = millis(); 
unsigned long ifReadlivetimer1 = millis();
unsigned long ifReadmeantimer2 = millis();
unsigned long ifReadlivetimer2 = millis();
unsigned long ifReadmeantimer3 = millis();
unsigned long ifReadlivetimer3 = millis();
unsigned long ifReadmeantimer4 = millis();
unsigned long ifReadlivetimer4 = millis();

void setup()
{
  // Steup every pin 
  pinMode(motor, OUTPUT);
  pinMode(randomMode, INPUT);
  // Light barriers 
  pinMode(A0,INPUT); 
  pinMode(A1,INPUT); 
  pinMode(A2,INPUT); 
  pinMode(A3,INPUT); 
  // LEDs of players
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  //Serial monitor for testing 
  Serial.begin(38400);

  // Turn on players LEDs
  digitalWrite(8,HIGH); 
  digitalWrite(9,HIGH); 
  digitalWrite(10,HIGH); 
  digitalWrite(11,HIGH);
  
  randomSeed(analogRead(4)); 

  //Mean value for lightbarrier comparison of each player 
  for (int i = 0; i<9; i++){
    ifReadmean1 = ifReadmean1+analogRead(A0); 
  }
  ifReadmean1 = ifReadmean1/10; 

  for (int i = 0; i<9; i++){
    ifReadmean2 = ifReadmean2+analogRead(A1); 
  }
  ifReadmean2 = ifReadmean2/10;
  
  for (int i = 0; i<9; i++){
    ifReadmean3 = ifReadmean3+analogRead(A2); 
  }
  ifReadmean3 = ifReadmean3/10;

  for (int i = 0; i<9; i++){
    ifReadmean4 = ifReadmean4+analogRead(A3); 
  }
  ifReadmean4 = ifReadmean4/10;

  // Reset save period timer 
  savetimer1= millis(); 
}

// the loop function runs over and over again until power down or reset
void loop()
{
  //Selection of two possible modes Random Mode of the game and Rest Mode 
  // Random Mode is the mode in which the game will be played Reset Mode will stop the game and reset everything back to start values 
  if (digitalRead(randomMode) == HIGH){
    Random_Mode(); 
  }

  else{
    Reset_Mode(); 
  }

}

void Random_Mode(){

  //Motor control event done after variableTime with random motorSpeed
    if ((millis()-lastMillis1)>=variableTime){
    motorSpeed = random(50,255); 
    analogWrite(motor,motorSpeed);
    variableTime= random(1000,3000);
    lastMillis1 = millis(); 
    counter = 0; 
  }

  // Due to often changing light conditions the mean values of the light barriers need to be adjusted constanly during the game here its done after every 300 ms 
  if ((millis() - ifReadmeantimer1)>=300 || (millis()-lastMillis1)>=variableTime){
      for (int i = 0; i<9; i++){
        ifReadmean1 = ifReadmean1+analogRead(A0); 
      }
      ifReadmean1 = ifReadmean1/10; 
      ifReadmeantimer1 = millis(); 
  } 

    if ((millis() - ifReadmeantimer2)>=300 || (millis()-lastMillis1)>=variableTime){
      for (int i = 0; i<9; i++){
        ifReadmean2 = ifReadmean2+analogRead(A1); 
      }
      ifReadmean2 = ifReadmean2/10; 
      ifReadmeantimer2 = millis(); 
  } 

    if ((millis() - ifReadmeantimer3)>=125 || (millis()-lastMillis1)>=variableTime){
      for (int i = 0; i<9; i++){
        ifReadmean3 = ifReadmean3+analogRead(A2); 
      }
      ifReadmean3 = ifReadmean3/10; 
      ifReadmeantimer3 = millis(); 
  } 

    if ((millis() - ifReadmeantimer4)>=300 || (millis()-lastMillis1)>=variableTime){
      for (int i = 0; i<9; i++){
        ifReadmean4 = ifReadmean4+analogRead(A3); 
      }
      ifReadmean4 = ifReadmean4/10; 
      ifReadmeantimer4 = millis(); 
  } 

  // Reading out every IR-light Senesor every 10 ms to detect loss of players life 
  if ((millis()-lastMillis2)>=10){
    ifRead1 = analogRead(A0); 
    ifRead2 = analogRead(A1); 
    ifRead3 = analogRead(A2); 
    ifRead4 = analogRead(A3);  
    lastMillis2=millis(); 
  }

  // Loss of players life event if the currently read value is smaller than the mean value with some buffer due to changing light conditions a counter value is added
  if ((ifReadmean1-10) > ifRead1 && savetimer1 - millis() >= 150){
    counterifread1++; 
    
    // if the counter is greater than a given previous determined threshold and the value is out of bounds of the lifetimer a life will be substracted 
    if (counterifread1 >= 3 && (ifReadlivetimer1 - millis())>=300){
        ifReadlives1 --; 
        ifReadlivetimer1 = millis(); 
        if (ifReadlives1 <= 0){
          digitalWrite(8,LOW);  
        }
        else{
            digitalWrite(8,LOW); 
            delay(100); 
            digitalWrite(8,HIGH);
            counterifread1 = 0; 
        }
    }
  }

  if ((ifReadmean2-15) > ifRead2 && savetimer1 - millis() >= 150){
    counterifread2++; 
    if (counterifread2 >= 3 && (ifReadlivetimer2 - millis())>=300){
        ifReadlives2 --; 
        ifReadlivetimer2 = millis(); 
        if (ifReadlives2 <= 0){
          digitalWrite(9,LOW);  
        }
        else{
            digitalWrite(9,LOW); 
            delay(100); 
            digitalWrite(9,HIGH);
            counterifread2 = 0;  
        }
    }   
  }

    if ((ifReadmean3-7) >= ifRead3 && savetimer1 - millis() >= 150){
    counterifread3++; 
    if (counterifread3++ >= 5 &&(ifReadlivetimer3 - millis())>=300){
        ifReadlives3 --; 
        ifReadlivetimer3 = millis(); 
        if (ifReadlives3 <= 0){
          digitalWrite(10,LOW);  
        }
        else{
            digitalWrite(10,LOW); 
            delay(100); 
            digitalWrite(10,HIGH); 
            counterifread3 = 0; 
        }
    }   
  }

    if ((ifReadmean4-20) > ifRead4 && savetimer1 - millis() >= 150){
    counterifread4++; 
    if (counterifread4++ >= 5 &&(ifReadlivetimer4 - millis())>=300){
        ifReadlives4 --; 
        ifReadlivetimer4 = millis(); 
        if (ifReadlives4 <= 0){
          digitalWrite(11,LOW);  
        }
        else{
            digitalWrite(11,LOW); 
            delay(100); 
            digitalWrite(11,HIGH); 
            counterifread4 = 0; 
        }
    }   
  }
}

void Reset_Mode(){
  // Reset lifes and turn every LED back on 
  ifReadlives1 = 3; 
  ifReadlives2 = 3; 
  ifReadlives3 = 3; 
  ifReadlives4 = 3; 

  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH);
  digitalWrite(11,HIGH); 
}
