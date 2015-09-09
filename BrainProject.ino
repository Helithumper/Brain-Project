//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;


int nextColor;
byte colors;
byte storedColor;

int redPin = 3;
int bluePin = 5;
int greenPin = 6;

int redButton = 0;
int blueButton = 1;
int greenButton = 2;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin,OUTPUT);

  pinMode(redButton,INPUT);
  pinMode(blueButton,INPUT);
  pinMode(greenButton,INPUT);

  blinkAll_2Bytes(2,500); 
}

void loop() {
  //check for click
  if(debounceRED(lastRED)){
    queueColor(0);
  }
  else if(debounceBLUE(lastBLUE)){
    queueColor(1);
  }
  else if(debounceGREEN(lastGREEN)){
    queueColor(2);
  }

  refreshColors();

  //delay by pot
  lastRED = digitalRead(redButton);
  blueRED = digitalRead(blueButton);
  greenRED = digitalRead(greenButton);
  
}

boolean debounceRED(boolean last){
  boolean current = digitalRead(redButton);

  if(last!= current){
    delay(5);
    current = digitalRead(redButton);
  }
  return current;
}
boolean debounceBLUE(boolean last){
  boolean current = digitalRead(blueButton);

  if(last!= current){
    delay(5);
    current = digitalRead(blueButton);
  }
  return current;
}
boolean debounceGREEN(boolean last){
  boolean current = digitalRead(greenButton);

  if(last!= current){
    delay(5);
    current = digitalRead(greenButton);
  }
  return current;
}

/*Queue functions
 * 0: Red
 * 1: Blue
 * 2: Green
 */
void queueColor(int color){
  nextColor = color;
}

void refreshColors(){
  colors = colors >> 3; //Shift right three
  storedColor = colors;
  switch(nextColor){
    case 0:
      bitWrite(colors,11,1);break;
    case 1:
      bitWrite(colors,10,1);break;
    case 2:
      bitWrite(colors,9,1);break;
  }
  digitalWrite(latchPin,LOW);
  shiftOut(dataPin, clockPin, colors);
  digitalWrite(latchPin,HIGH);
  int finalColor = 0;
  if(bitRead(colors,2)==1){ //red
    finalColor = 0;
  }
  else if(bitRead(colors,1)==1){ //blue
    finalColor = 1;
  }
  else if(bitRead(colors,0)==1){ //green
    finalColor = 2;
  }
  else{
    finalColor = 3; //none
  }
  writeFinalColor(finalColor);
}

void writeFinalColor(int finalColor){
  switch(finalColor){
    case 0:
      digitalWrite(redPin,HIGH);
      digitalWrite(bluePin,LOW);
      digitalWrite(greenPin,LOW);
      break;
    case 1:
      digitalWrite(redPin,LOW);
      digitalWrite(bluePin,HIGH);
      digitalWrite(greenPin,LOW);
      break;
    case 2:
      digitalWrite(redPin,LOW);
      digitalWrite(bluePin,LOW);
      digitalWrite(greenPin,HIGH);
      break;
    case 3:
      break;
  }
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

