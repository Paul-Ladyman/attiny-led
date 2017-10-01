#define PWMHIGH 255
#define PWMLOW 0
#define ANALOGLOW 0
#define FLASHMODE 0
#define SFLASHMODE 1
#define CONSTANTMODE 2
#define NUMMODES 3
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

boolean modeToggled(int newMode) {
  if (newMode != lastModeToggleState) {
    lastModeToggleState = newMode;
    return newMode == HIGH;
  }
  else {
    return false;
  }
}

void changeMode(void) {
  mode++;
  if (mode == NUMMODES) {
    mode = FLASHMODE;
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

int getOutput(int pwmVal, int mode, boolean fadingIn) {
  switch (mode) {
    case CONSTANTMODE:
      return PWMHIGH;
    case SFLASHMODE:
      return pwmVal;
    case FLASHMODE:
    default:
      if (fadingIn) {
        return PWMHIGH;
      }
      else {
        return PWMLOW;
      }

  }
}

void setNextPwmVal(void) {
  if (fadingIn) {
    if (i == PWMHIGH) {
      fadingIn = false;
    }
    else {
      i++;
    }
  }
  else {
    if (i == PWMLOW) {
      fadingIn = true;
    }
    else {
      i--;
    }
  }
}

void loop() {
  int newMode = readModeInput();

  if (modeToggled(newMode)) {
    changeMode();
  }
  
  float multiplier1 = readPanInput();
  float multiplier2 = 1.0 - multiplier1;

  writeLeds(getOutput(i, mode, fadingIn), multiplier1, multiplier2);

  setNextPwmVal();

  delayMicroseconds(readTimerInput());
}
