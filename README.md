# Arduino Pendant

This project allows connecting an MPG pandant to an Arduino Nano to be used as a USB serial device.

## Protocol

Messages are terminated with `\r\n`.

| Message     | Details                                                                                                                                                                                         |
| ----------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `READY`     | Sent when first connecting and/or when ESTOP is reset.                                                                                                                                          |
| `STOP`      | Sent when ESTOP is triggered.                                                                                                                                                                   |
| `STEP:1,1,1 | Indicates a step action should be performed. The 3 comma-separated values are as follows (in order): axis index (1-12, X=1, Y=2, Z=3, etc), magnification (1, 10, or 100), the number of step.s |

`STEP` and `STOP` commands are sent at most every 100ms.

## Pin Connections

| Wire Color   | Signal | Const         | Nano Pin |
| ------------ | ------ | ------------- | -------- |
| Red          | +5V    |               | 5V       |
| Black        | 0V     |               | GND      |
| Green        | A      | PULSE_A       | D2       |
| White        | B      | PULSE_B       | D3       |
| Purple       | A̅      |               |          |
| Purple/Black | B̅      |               |          |
| Yellow       | X      | AXIS_1        | A2       |
| Yellow/Black | Y      | AXIS_2        | A3       |
| Brown        | Z      | AXIS_3        | A4       |
| Brown/Black  | 4      | AXIS_4        | A5       |
| Pink         | 5      |               |          |
| Pink/Black   | 6      |               |          |
| Gray         | X1     | MAG_X1        | D10      |
| Gray/Black   | X10    | MAG_X10       | D9       |
| Orange       | X100   | MAG_X100      | D8       |
| Orange/Black | COM    |               | GND      |
| Green/Black  | LED(+) | LED_INDICATOR | D7       |
| White/Black  | LED(-) |               | GND      |
| Blue         | C      | ESTOP         | D6       |
| Blue/Black   | CN     |               | GND      |
| Fibrous      | Shield |               |          |

![MPG Pendant](https://i.imgur.com/TpIkR5L.jpg)
