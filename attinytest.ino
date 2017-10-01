#define PWMHIGH 255
#define PWMLOW 0
#define ANALOGLOW 0
#define ANALOGHIGH 1023

int pwmPin = 0;
int pwmPin2 = 1;
int timerPin = A1;
int panPin = A2;

int i = 0;
boolean fadingIn = true;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  pinMode(timerPin, INPUT);
  pinMode(panPin, INPUT);
}

int readTimerInput(void) {
  int timerInput = analogRead(timerPin);
  return map(timerInput, ANALOGLOW, ANALOGHIGH, 10, 5000);
}

float readPanInput(void) {
  float timerInput = (float) analogRead(panPin);
  return timerInput / 1023.0;
}

/*
  outputs inverted since pins will connect to LED ground
  255 is therefore off and 0 is on
*/
void writeLeds(int pwmVal, float led1Weight, float led2Weight) {
  analogWrite(pwmPin, PWMHIGH - (pwmVal * led1Weight));
  analogWrite(pwmPin2, PWMHIGH - (pwmVal * led2Weight));
}

void loop() {
  int timeout = readTimerInput();

  float multiplier1 = readPanInput();
  float multiplier2 = 1.0 - multiplier1;

  if (fadingIn) {
    writeLeds(i, multiplier1, multiplier2);

    if (i == PWMHIGH) {
      fadingIn = false;
    }
    else {
      i++;
    }

    delayMicroseconds(timeout);
  }
  else {
    writeLeds(i, multiplier1, multiplier2);

    if (i == PWMLOW) {
      fadingIn = true;
    }
    else {
      i--;
    }

    delayMicroseconds(timeout);
  }
}
