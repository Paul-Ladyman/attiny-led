int pwmPin = 0;
int pwmPin2 = 1;
int analogInPin = A1;
int i = 0;
int stage = 0; // fade in

void setup()
{
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  pinMode(analogInPin, INPUT);
}

void loop()
{
  int timerInput = analogRead(analogInPin);
  int timeout = map(timerInput, 0, 1023, 10, 5000);

  float multiplier1 = 0.1;
  float multiplier2 = 1.0 - multiplier1;

  /*
    outputs inverted since pins will connect to LED ground
    255 is therefore off and 0 is on
  */
  if (stage == 0 && i <= 255) {
    analogWrite(pwmPin, 255 - (i * multiplier1));
    analogWrite(pwmPin2, 255 - (i * multiplier2));

    i++;
    if (i == 255) {
      stage = 1; // fade out
    }

    delayMicroseconds(timeout);
  }
  else if (stage == 1 && i >= 0) {
    analogWrite(pwmPin, 255 - (i * multiplier1));
    analogWrite(pwmPin2, 255 - (i * multiplier2));

    i--;
    if (i == 0) {
      stage = 0; // fade in
    }

    delayMicroseconds(timeout);
  }
}
