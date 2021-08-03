#define MaxScore 4                                          // Max Score is the score on which a player wins...
#define GreenLeds 5                                         // Total Number of Green Leds...
#define YellowLeds 4                                        // Total Number of Yellow Leds...
#define RedLeds 2                                           // Total Number of Red Leds....
#define TotalLeds (GreenLeds+YellowLeds+RedLeds)            // Total Number of Leds (Auto Set)...
#define StartingSpeed 300                                   // Speed at which the Game Starts (Set this Speed in milliseconds)...
#define ConstantSpeed 25                                    // This Speed is set so that delay function doesn't complain out of memory (Set this speed in milliseconds)
#define Button1Pin 34                                       // Pin Number to which Player 1's Button is connected...
#define Button2Pin 35                                       // Pin Number to which Player 2's Button is connected...
#define SpeedReduce 50                                      // More the Value of SpeedReduce, more faster the LEDs will become

// Set Pins for all LEDs manually in the Pin[] array initialization...

int i, k, State=LOW, Pin[TotalLeds]={22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32}, Button1=Button1Pin, Button2=Button2Pin, Speed=StartingSpeed, Player1Score=0, Player2Score=0 ;

void setup()
{
  void BlinkLeds(int) ;
  for( i=0 ; i < TotalLeds ; ++i )
  {
    pinMode(Pin[i], OUTPUT) ;
  }
  pinMode(Button1, INPUT) ;
  pinMode(Button2, INPUT) ;
   Serial.begin(9600) ;
}

void(* resetFunc)(void) = 0x0 ;                          // This function resets Arduino without pushing reset button

void loop()
{
  Serial.flush() ;
  Speed = Speed-SpeedReduce ;
  if(Speed < 1)
    Speed = ConstantSpeed ;
  State = LOW ;
  for( i=(RedLeds/2) ; i < (GreenLeds+((YellowLeds+RedLeds)/2)) ; ++i )
  {
    State = digitalRead(Button1) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 1 Score : ") ;
      Serial.println(Player1Score) ;
      goto Reverse ;
    }
    digitalWrite(Pin[i], HIGH) ;
    delay(Speed) ;
    State = digitalRead(Button1) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 1 Score : ") ;
      Serial.println(Player1Score) ;
      goto Reverse ;
    }
    digitalWrite(Pin[i], LOW) ;
    State = digitalRead(Button1) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 1 Score : ") ;
      Serial.println(Player1Score) ;
      goto Reverse ;
    }
  }
  k=0 ;
  do
  {
    digitalWrite(Pin[i], HIGH) ;
    delay(Speed) ;
    State = digitalRead(Button1) ;
    digitalWrite(Pin[i], LOW) ;
    ++i ;
    ++k ;
  }while(k < (YellowLeds)/2) ;
  if(State == HIGH)
  {
    ++Player1Score ;
    Serial.println("\n Player 1 Score : ") ;
    Serial.println(Player1Score) ;
    goto Reverse ;
  }
  else
  {
    k=0 ;
    do
    {
      digitalWrite(Pin[i], HIGH) ;
      delay(Speed) ;
      digitalWrite(Pin[i], LOW) ;
      ++k ;
    }while(k < (RedLeds)/2) ;
    Serial.println("\n Player 1 Score : ") ;
    Serial.println(Player1Score) ;
    BlinkLeds(5) ;
    // Uncomment next line if we want to increase Player 2's score if Player 1 fails to press button at right time...
    //++Player2Score ;
  }
  Reverse :
  if(Player1Score == MaxScore)
  {
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |           Final Scores              | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |             Player 1                | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(Player1Score) ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |             Player 2                | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(Player2Score) ;
    Serial.println("\n --> Player 1 Wins this Game <-- \n") ;
    BlinkLeds(25) ;
    resetFunc() ;
  }
  State = LOW ;
  for( i=(GreenLeds+((YellowLeds+RedLeds)/2)) ; i >= (YellowLeds+RedLeds)/2 ; --i )
  {
    State = digitalRead(Button2) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 2 Score : ") ;
      Serial.println(Player2Score) ;
      goto End ;
    }
    digitalWrite(Pin[i], HIGH) ;
    delay(Speed) ;
    State = digitalRead(Button2) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 2 Score : ") ;
      Serial.println(Player2Score) ;
      goto End ;
    }
    digitalWrite(Pin[i], LOW) ;
    State = digitalRead(Button2) ;
    if(State == HIGH)
    {
      BlinkLeds(5) ;
      Serial.println("\n Player 2 Score : ") ;
      Serial.println(Player2Score) ;
      goto End ;
    }
  }
  k=0 ;
  do
  {
    digitalWrite(Pin[i], HIGH) ;
    delay(Speed) ;
    State = digitalRead(Button2) ;
    digitalWrite(Pin[i], LOW) ;
    --i ;
    ++k ;
  }while(k < (YellowLeds)/2) ;
  if(State == HIGH)
  {
    ++Player2Score ;
    Serial.println("\n Player 2 Score : ") ;
    Serial.println(Player2Score) ;
    goto End ;
  }
  else
  {
    k=0 ;
    do
    {
      digitalWrite(Pin[i], HIGH) ;
      delay(Speed) ;
      digitalWrite(Pin[i], LOW) ;
      ++k ;
    }while(k < (RedLeds)/2) ;
    Serial.println("\n Player 2 Score : ") ;
    Serial.println(Player2Score) ;
    BlinkLeds(5) ;
    // Uncomment next line if we want to increase Player 1's score if Player 2 fails to press button at right time...
    //++Player1Score ;
  }
  End :
  if(Player2Score == MaxScore)
  {
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |           Final Scores              | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |             Player 1                | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(Player1Score) ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(" |             Player 2                | ") ;
    Serial.println("  ------------------------------------- ") ;
    Serial.println(Player2Score) ;
    Serial.println("\n --> Player 2 Wins this Game <-- \n") ;
    BlinkLeds(25) ;
    resetFunc() ;
  }
}

void BlinkLeds(int MaxTime)
{
  for(int Delay=0 ; Delay < MaxTime ; ++Delay)
  {
    for( i=0 ; i < 11 ; ++i )
        digitalWrite(Pin[i], HIGH) ;
    delay(100) ;
    for( i=0 ; i < 11 ; ++i )
      digitalWrite(Pin[i], LOW) ;
    delay(150) ;
  }
}

