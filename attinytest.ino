#define PWMHIGH 255
#define PWMLOW 0
#define ANALOGLOW 0
#define FLASHMODE 0
#define SFLASHMODE 1
#define NUMMODES 2
#define ANALOGHIGH 1023

// pin definitions
int pwmPin = 0;
int pwmPin2 = 1;
int timerPin = A1;
int panPin = A2;
int modePin = 3;

// system defaults
int i = 0;
boolean fadingIn = true;
int mode = FLASHMODE;
int modePinState = LOW;
int lastModePinState = LOW;
int lastModeToggleState = LOW;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastModeToggleTime = 0;
unsigned long modeToggleDelay = 100;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin2, OUTPUT);
  pinMode(timerPin, INPUT);
  pinMode(panPin, INPUT);
  pinMode(modePin, INPUT);
}

int readTimerInput(void) {
  int timerInput = analogRead(timerPin);
  return map(timerInput, ANALOGLOW, ANALOGHIGH, 10, 5000);
}

float readPanInput(void) {
  float timerInput = (float) analogRead(panPin);
  return timerInput / 1023.0;
}

boolean modeToggleDelayLapsed(void) {
  return (millis() - lastModeToggleTime) > modeToggleDelay;
}

void resetModeToggleTime(void) {
  lastModeToggleTime = millis();
}

/*
   Read push button mode toggle with debounce
*/
int readModeInput(void) {
  int reading = digitalRead(modePin);

  if (reading != lastModePinState) {
    resetModeToggleTime();
  }

  if (modeToggleDelayLapsed() && reading != modePinState) {
    modePinState = reading;
  }

  lastModePinState = reading;

  return modePinState;
}

boolean modeToggled(void) {
  int newMode = readModeInput();

  if (newMode != lastModeToggleState) {
    lastModeToggleState = newMode;
    return newMode == HIGH;
  }
  else {
    return false;
  }
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
  if (modeToggled()) {
    mode++;
    if (mode == NUMMODES) {
      mode = FLASHMODE;
    }
  }

  int timeout = readTimerInput();

  float multiplier1 = readPanInput();
  float multiplier2 = 1.0 - multiplier1;

  if (fadingIn) {

    if (mode == FLASHMODE) {
      writeLeds(PWMHIGH, multiplier1, multiplier2);
    }
    else {
      writeLeds(i, multiplier1, multiplier2);
    }

    if (i == PWMHIGH) {
      fadingIn = false;
    }
    else {
      i++;
    }

    delayMicroseconds(timeout);
  }
  else {
    if (mode == FLASHMODE) {
      writeLeds(PWMLOW, multiplier1, multiplier2);
    }
    else {
      writeLeds(i, multiplier1, multiplier2);
    }

    if (i == PWMLOW) {
      fadingIn = true;
    }
    else {
      i--;
    }

    delayMicroseconds(timeout);
  }
}
