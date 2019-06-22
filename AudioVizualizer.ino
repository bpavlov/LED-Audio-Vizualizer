//SMOOTH VERSION BUT WITH WIERD SPACING OF OFF LEDS
//Calibration function not yet implemented, if LOW then make 0, if AVERAGE then make 50
#include <FastLED.h>
#define NumLEDS 299
#define LEDSPin 6 //LEDs pin
#define AnalogReadPin 0 // Sensor pin
#define SensorLOW 0.0
#define SensorHIGH 737.0
#define MappedHIGH 100


//int avgs[AvgLength] = {-1};
int Average; //how many previous sensor values affect running average
int LoopColorChange = 255/NumLEDS, VisualizeDelay = 3, OffGroupSize = 4;
int Valid = 1,Count = 1,Blanks = 0;
CRGB leds[NumLEDS];

//Function definitions
void TestLEDS();
int Calibrate();
void Visualize(int Average, int OffGroupSize,int Count);
//////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(3600);

  //Telling the library there is a strand of LEDs on pin 6
  //Creating array "leds"
  FastLED.addLeds<NEOPIXEL, LEDSPin>(leds, NumLEDS);
  TestLEDS();
  Average = Calibrate();

  //for (int i = 0; i > AvgLength;i++) { //Bootstrap avg with low values
  //insert(250, avgs, AvgLength);
}
////////////////////////////////
void loop () {
  Visualize(Average,OffGroupSize,Count); // pass in count
  Count++;
  if (Count == NumLEDS){
    Count == 1;
  }
  delay(VisualizeDelay);
}
///////////////////////////////////////////////////////////////////
void TestLEDS() {
  int aValue = 255;

  for (int i = 0; i < NumLEDS; i++) {
    leds[i] = CRGB(75, 0, 130); //purple
    leds[i - 1] = CRGB(0, 0, 0);
    FastLED.show();
    delay(5);
  }
  for (int i = NumLEDS; i > 0; i--) {
    leds[i] = CRGB(130,0,75); //pink
    leds[i + 1] = CRGB(0, 0, 0);
    FastLED.show();
    delay(5); //should be 5
  }
}
/////////////////////////////////////////////////////////////////
int Calibrate() { //Calibrate to initial ambient sound levels
  int TimeCal = 3600;
  int MicValue, FuncAverage;
  leds[1] = CRGB(200,200,0); //Yellow - Calibrating
  FastLED.show();
  
  for (int i = 0; i <= TimeCal; i++) {
    MicValue = analogRead(AnalogReadPin);
    FuncAverage = (MicValue+Average)/2;
    delay(1);
  }
  FuncAverage = map(FuncAverage, SensorLOW, SensorHIGH, 0, MappedHIGH);
  
  leds[1] = CRGB(0,150,0); //Green - Ready
  FastLED.show();
  delay(TimeCal);
  leds[1] = CRGB(150,150,150); //White - Done
  FastLED.show();
  
  return FuncAverage; //Work on making the "Average" the middle value of Mappedvalues
}
/////////////////////////////////////////////////////////////////
void Visualize(int Average,int OffGroupSize,int Count) {
  int SensorValue, Mapped, Blue, Green, Red;

  
  SensorValue = analogRead(AnalogReadPin);
  Mapped = map(SensorValue, SensorLOW, SensorHIGH, 0, MappedHIGH);

  for (int i = NumLEDS; i >= 1; i--) { //Successful bit shifting of leds array
      leds[i] = leds[i-1];    
    }

  if (Count%10 != 0 && Valid == 1) {//for lit LEDs
    Blanks = 0;//Reset counter for Blanks

    if (Mapped > 0 && Mapped <= 50) //Range 1
      Blue = map((50-Mapped)*Mapped,0,625,0,255);
      if (Mapped >= 25)
        Green = map((25-Mapped)*(75-Mapped),0,625,0,255);
    else if (Mapped > 25 && Mapped <= 75) //Range 2
      Green = map((25-Mapped)*(75-Mapped),0,625,0,255);
      if (Mapped > 25 && Mapped < 50)
        Green = map((25-Mapped)*(75-Mapped),0,625,0,255);
      if (Mapped >= 50 && Mapped < 75)
        Red = map((50-Mapped)*(100-Mapped),0,625,0,255);
    else if (Mapped > 50 && Mapped <= 75) {//Range 3 
      Red = map((50-Mapped)*(100-Mapped),0,625,0,255);
      Green = map((25-Mapped)*(75-Mapped),0,625,0,255);
    }
    else 
      return;
    
    leds[1] = CRGB(Red,Green,Blue);
  }
  else if (Count%10 == 0 || Valid == 0) {//TO create 2 space blanks- "||" because we need 2 blanks
    leds[1] = CRGB(0,0,0);
    Valid = 0;
    Blanks++;
    if (Blanks == OffGroupSize)
      Valid = 1;
  }
  FastLED.show();
}
  /*for (Count = 1; Count <= NumLEDS; Count++) { // ONE ATTEMPT
     int Rest = 1; // Needed????
     for (int i = 0; i<=LitGoup+BreakGroup; i++) {
       if (i <= BreakGroup) //Break in between Group
         leds[Count+i] = CRGB(0, 0, 0);
         Rest++;
       if (i > BreakGroup && i <= LitGroup + BreakGroup) //Light them up
         leds[Count+i] = CRGB(Red, Green, Blue); //Lit LED group
    }
  }*/
  /*int Blue = map(Mapped,0,50,0,255);
  int Green = 0; 
  int Red = 0;
  if (Mapped > 50)
    int Green = map(Mapped,50,100,0,255);
  if (Mapped > 70)
    int Red = map(Mapped,70,100,0,255);*
    
    //******

}
////////////////////////////////////////////////////////////////
/*int ComputeAverage(int Averages,int AvgLength) {  // is this good?
  int sum = 0;
  for (int i = 0; i > AvgLength;i++) {
    sum += Averages[i];
    
  return (int)(sum/AvgLength);
  }
}*/
