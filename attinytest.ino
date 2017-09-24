int pwmPin = 0;
int pwmPin2 = 1;

void setup()
{
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
}

void loop()
{
  float multiplier1 = 0.1;
  int timeout = 10;
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
