#include <Arduino.h>
#include <Ticker.h>

#define PULSE_A 2
#define PULSE_B 3
// #define PULSE_AL 4
// #define PULSE_BL 5

#define AXIS_1 PIN_A2
#define AXIS_2 PIN_A3
#define AXIS_3 PIN_A4
#define AXIS_4 PIN_A5

// #define AXIS_SHIFT_0 12
// #define AXIS_SHIFT_1 11

#define MAG_X1 10
#define MAG_X10 9
#define MAG_X100 8

#define LED_INDICATOR 7

#define ESTOP 6

bool wasStop = true;
volatile int8_t steps = 0;

void doSendState();
void doPulse();

Ticker sendState(doSendState, 10, 0, MILLIS);

void setup()
{
  pinMode(PULSE_A, INPUT);
  pinMode(PULSE_B, INPUT);
  // pinMode(PULSE_AL, INPUT);
  // pinMode(PULSE_BL, INPUT);

  pinMode(AXIS_1, INPUT_PULLUP);
  pinMode(AXIS_2, INPUT_PULLUP);
  pinMode(AXIS_3, INPUT_PULLUP);
  pinMode(AXIS_4, INPUT_PULLUP);

  // pinMode(AXIS_SHIFT_0, INPUT_PULLUP);
  // pinMode(AXIS_SHIFT_1, INPUT_PULLUP);

  pinMode(MAG_X1, INPUT_PULLUP);
  pinMode(MAG_X10, INPUT_PULLUP);
  pinMode(MAG_X100, INPUT_PULLUP);

  pinMode(LED_INDICATOR, INPUT_PULLUP);

  pinMode(ESTOP, INPUT_PULLUP);

  pinMode(LED_INDICATOR, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PULSE_A), doPulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PULSE_B), doPulse, CHANGE);

  sendState.start();

  Serial.begin(115200);
}

void doPulse()
{
  static byte state = 0;
  state = (state << 2) | (PIND & (_BV(PULSE_A) | _BV(PULSE_B)));

  switch (state)
  {
  case B10110100:
    steps--;
    break;
  case B01111000:
    steps++;
    break;
  }
}

void sendPin(int pin)
{
  int val = digitalRead(pin);
  Serial.print(val, DEC);
}

void loop()
{
  digitalWrite(LED_INDICATOR, !digitalRead(ESTOP));
  sendState.update();
}

bool sendStatus();
void doSendState()
{
  static int emptyCount = 0;
  if (sendStatus())
  {
    emptyCount = 0;
  }
  emptyCount++;
  if (emptyCount == 10)
  {
    emptyCount = 0;
    Serial.println("READY");
  }
}

bool sendStatus()
{
  if (digitalRead(ESTOP))
  {
    wasStop = true;
    Serial.println("STOP");
    return true;
  }

  if (wasStop)
  {
    wasStop = false;
    noInterrupts();
    steps = 0;
    interrupts();
    Serial.println("READY");
    return true;
  }

  int8_t curSteps;
  noInterrupts();
  curSteps = steps;
  steps = 0;
  interrupts();

  if (!curSteps)
    return false;

  int mag = 0;
  if (!digitalRead(MAG_X1))
    mag = 1;
  if (!digitalRead(MAG_X10))
    mag = 10;
  if (!digitalRead(MAG_X100))
    mag = 100;
  if (!mag)
    return false;

  int axis = 0;
  if (!digitalRead(AXIS_1))
    axis = 1;
  if (!digitalRead(AXIS_2))
    axis = 2;
  if (!digitalRead(AXIS_3))
    axis = 3;
  if (!digitalRead(AXIS_4))
    axis = 4;
  if (!axis)
    return false;

  // if (!digitalRead(AXIS_SHIFT_0))
  //   axis += 4;
  // if (!digitalRead(AXIS_SHIFT_1))
  //   axis += 8;

  Serial.print("STEP:");
  Serial.print(axis);
  Serial.print(",");
  Serial.print(mag);
  Serial.print(",");
  Serial.println(curSteps);

  return true;
}
