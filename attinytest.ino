int pwmPin = 0;
int pwmPin2 = 1;
int analogInPin = A1;

void setup()
{
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  pinMode(analogInPin, INPUT);
}

void loop()
{
  int timerInput = analogRead(analogInPin);
  int timeout = map(timerInput, 0, 1023, 1, 10);
  
  float multiplier1 = 0.1;
  float multiplier2 = 1.0 - multiplier1;

  /*
     outputs inverted since pins will connect to LED ground
     255 is therefore off and 0 is on
  */
  for (int i = 0; i <= 255; i += 1)
  {
    analogWrite(pwmPin, 255 - (i * multiplier1));
    analogWrite(pwmPin2, 255 - (i * multiplier2));
    delay(timeout);
  }

  for (int i = 255; i >= 0; i -= 1)
  {
    analogWrite(pwmPin, 255 - (i * multiplier1));
    analogWrite(pwmPin2, 255 - (i * multiplier2));
    delay(timeout);
  }
}
