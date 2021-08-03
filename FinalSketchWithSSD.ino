#define MaxScore 4                                          // Max Score is the score on which a player wins...
#define GreenLeds 5                                         // Total Number of Green Leds...
#define YellowLeds 8                                        // Total Number of Yellow Leds...
#define RedLeds 4                                           // Total Number of Red Leds....
#define TotalLeds (GreenLeds+YellowLeds+RedLeds)            // Total Number of Leds (Auto Set)...
#define StartingSpeed 25                                   // Speed at which the Game Starts (Set this Speed in milliseconds)...
#define ConstantSpeed 150                                    // This Speed is set so that delay function doesn't complain out of memory (Set this speed in milliseconds)
#define ConstantPushSpeed 50
#define Player1Button1Pin 48                                // Pin Number to which **Player 1's First Button** is connected...
#define Player1Button2Pin 49                                // Pin Number to which **Player 1's Second Button** is connected...
#define Player2Button1Pin 50                                // Pin Number to which **Player 2's First Button** is connected...
#define Player2Button2Pin 51                                // Pin Number to which **Player 2's Second Button** is connected...
#define SpeedReduce 0                                      // More the Value of SpeedReduce, more faster the LEDs will become
#define pathLeds (GreenLeds+((YellowLeds+RedLeds)/2))       // Total number of LEDs in one path
#define IncorrectMoveLedBlinkingSpeed 150
#define BlinkCount  6

// Set Pins for all LEDs manually in the Pin[] array initialization...

int i, k, Button1, Button2, Button3, Button4, redLedIndex, PrimaryButton, SecondaryButton ;

int State1, State2, State, Pin[TotalLeds]={22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38} ;

int Speed=StartingSpeed, PushSpeed=StartingSpeed+25 ;

int Player1Score=0, Player2Score=0, Index=0 ;

// Setting Paths and buttons for LEDs...

int forwardPath[4][pathLeds], backwardPath[4][pathLeds], *LedPath, oldRandomPath, newRandomPath, temp  ;

// Prototying Functions

void BlinkRedLed(int) ;
void BlinkPath(int *) ;
void BlinkAllLeds(void) ;

void setup()
{
  Serial.begin(9600) ;
  int counter=0 ;
  oldRandomPath = 2 ;
  newRandomPath = 0 ;
  Button1 = Player1Button1Pin ;
  Button2 = Player1Button2Pin ;
  Button3 = Player2Button1Pin ;
  Button4 = Player2Button2Pin ;
  void BlinkLeds(int) ;
  // Setting up all LEDs...
  for( i=0 ; i < TotalLeds ; ++i )
  {
    pinMode(Pin[i], OUTPUT) ;
  }
  /* Setting Up Forward Paths */
  
  // Path 0 //
  
  for( i=0, k=0 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[0][i] = Pin[k] ;
    if(k == (((RedLeds+YellowLeds)/4)-1))
    {
      k = (((RedLeds+YellowLeds)/2)-1) ;
    }
  }

  // Path 0 //
  
  // Path 1 //
  
  for( i=0, k=0 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[1][i] = Pin[k] ;
    if(k == (((RedLeds+YellowLeds)/4)-1))
    {
      k = (((RedLeds+YellowLeds)/2)-1) ;
    }
    if(k == (((RedLeds+YellowLeds)/2)+4))
    {
      k = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
  }
  
  // Path 1 //

  // Path 2 //
  
  for( i=0, k=3 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[2][i] = Pin[k] ;
  }
  
  // Path 2 //

  // Path 3 //
  
  for( i=0, k=3 ; i < pathLeds ; ++i, ++k )
  {
    forwardPath[3][i] = Pin[k] ;
    if(k == (((RedLeds+YellowLeds)/2)+4))
    {
      k = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
  }
  
  // Path 3 //

  /* Setting of forward Paths done */
  
  /* Setting Up backward Paths */

  // Setting All Backward Paths at once //
  
  for(int j=0 ; j < 4 ; ++j )
  {
    for( i=0, k=pathLeds-1 ; i < pathLeds ; ++i, --k )
    {
      backwardPath[j][i] = forwardPath[j][k] ;
    }
  }

  // Setting Done //
  
  /* Setting of backward Paths done */
  
  // Player 1 Button Pins...
  pinMode(Button1, INPUT) ;
  pinMode(Button2, INPUT) ;
  // Player 2 Button Pins...
  pinMode(Button3, INPUT) ;
  pinMode(Button4, INPUT) ;
  Serial.begin(9600) ;
}

void(* resetFunc)(void) = 0x0 ;                          // This function resets Arduino without pushing reset button

void loop()
{
  Speed = Speed-SpeedReduce ;
  PushSpeed = PushSpeed-SpeedReduce ;
  if(Speed <= 1)
    Speed = ConstantSpeed ;
  if(PushSpeed <= 1)
    PushSpeed = ConstantPushSpeed ;
    
  /* Determining Forward Path and Correct Button */
  
  oldRandomPath = random(3-2+1)+2 ;
  if(newRandomPath == 0)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[0] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[1] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  else if(newRandomPath == 1)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[2] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[3] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  
  /* Forward Path and Correct Button Determined */

  State = LOW ;
  State1 = LOW ;
  State2 = LOW ;
  
  for( i=Index ; i < (pathLeds-((RedLeds+YellowLeds)/4)) ; ++i )
  {
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    delay(Speed) ;
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  for( k=0 ; k < YellowLeds/8 ; ++i, ++k )
  {
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto NextPath ;
    }
    delay(PushSpeed) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  oldRandomPath = random(3-2+1)+2 ;
  if(newRandomPath == 0)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[0] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[1] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  else if(newRandomPath == 1)
  {
    if(oldRandomPath == 2)
    {
      LedPath = forwardPath[2] ;
      PrimaryButton = Button3 ;
      SecondaryButton = Button4 ;
      redLedIndex = (((((RedLeds+YellowLeds)/4)*3)+GreenLeds)-1) ;
    }
    else
    {
      LedPath = forwardPath[3] ;
      PrimaryButton = Button4 ;
      SecondaryButton = Button3 ;
      redLedIndex = TotalLeds-1 ;
    }
  }
  for( k=0 ; k < YellowLeds/8 ; ++i, ++k )
  {
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto NextPath ;
    }
    delay(PushSpeed) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto NextPath ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  // Uncomment next line Pigeon
  /*
  if(State == LOW)
  {
    BlinkRedLed(redLedIndex) ;
  }
  */
  /* Determining Backward Path and Correct Button */

  NextPath :
  newRandomPath = random(1-0+1)+0 ;
  if(oldRandomPath == 2)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[0] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[2] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }
  else if(oldRandomPath == 3)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[1] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[3] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }

  /* Backward Path and Correct Button Determined */

  State = LOW ;
  State1 = LOW ;
  State2 = LOW ;
  
  for( i=Index ; i < (pathLeds-((RedLeds+YellowLeds)/4)) ; ++i )
  {
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    delay(Speed) ;
    State1 = digitalRead(PrimaryButton) ;
    State2 = digitalRead(SecondaryButton) ;
    if((State1 == HIGH) || (State2 == HIGH))
    {
      BlinkPath(LedPath) ;
      goto End ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  for( k=0 ; k < YellowLeds/8 ; ++i, ++k )
  {
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      goto End ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto End ;
    }
    delay(PushSpeed) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto End ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  newRandomPath = random(1-0+1)+0 ;
  if(oldRandomPath == 2)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[0] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[2] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }
  else if(oldRandomPath == 3)
  {
    if(newRandomPath == 0)
    {
      LedPath = backwardPath[1] ;
      PrimaryButton = Button1 ;
      SecondaryButton = Button2 ;
      redLedIndex = 0 ;
    }
    else
    {
      LedPath = backwardPath[3] ;
      PrimaryButton = Button2 ;
      SecondaryButton = Button1 ;
      redLedIndex = ((RedLeds+YellowLeds)/4) ;
    }
  }
  for( k=0 ; k < YellowLeds/8 ; ++i, ++k )
  {
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      goto End ;
    }
    digitalWrite(LedPath[i], HIGH) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto End ;
    }
    delay(PushSpeed) ;
    State = digitalRead(PrimaryButton) ;
    if(State == HIGH)
    {
      Index = pathLeds-i ;
      digitalWrite(LedPath[i], LOW) ;
      goto End ;
    }
    digitalWrite(LedPath[i], LOW) ;
  }
  // Uncomment next line Pigeon
  /*
  if(State == LOW)
  {
    BlinkRedLed(redLedIndex) ;
  }
  */
  End :;
} 

void BlinkRedLed(int index)
{
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    digitalWrite(Pin[index], HIGH) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
    digitalWrite(Pin[index], LOW) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
  }
}

void BlinkPath(int *path)
{
  /*
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    digitalWrite(Pin[index], HIGH) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
    digitalWrite(Pin[index], LOW) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
  }
  */
  for(int blinkLed=0 ; blinkLed < BlinkCount ; ++blinkLed )
  {
    for(int blinkPath=0 ; blinkPath < pathLeds ; ++blinkPath )
    {
      digitalWrite(path[blinkPath], HIGH) ;
    }
    delay(IncorrectMoveLedBlinkingSpeed ) ;
    for(int blinkPath=0 ; blinkPath < pathLeds ; ++blinkPath )
    {
      digitalWrite(path[blinkPath], LOW) ;
    }
    delay(IncorrectMoveLedBlinkingSpeed ) ;
  }
}

void BlinkAllLeds(void)
{
  /*
  for(int blinkAll=0 ; blinkAll < BlinkCount ; ++blinkAll )
  {
    digitalWrite(Pin[i], HIGH) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
    digitalWrite(Pin[i], LOW) ;
    delay(IncorrectMoveLedBlinkingSpeed) ;
  }
  */
}

