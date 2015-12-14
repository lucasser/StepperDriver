/*
 * DRV8834 - LV Stepper Motor Driver Driver (A4988-compatible - mostly)
 * Indexer mode only.

 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include "DRV8834.h"

/*
 * Basic connection: only DIR, STEP are connected.
 * Microstepping controls should be hardwired.
 */
DRV8834::DRV8834(int steps, int dir_pin, int step_pin)
:BasicStepperDriver(steps, dir_pin, step_pin)
{}

/*
 * Fully wired. All the necessary control pins for DRV8834 are connected.
 */
DRV8834::DRV8834(int steps, int dir_pin, int step_pin, int m0_pin, int m1_pin)
:BasicStepperDriver(steps, dir_pin, step_pin), m0_pin(m0_pin), m1_pin(m1_pin)
{}

/*
 * Set microstepping mode (1:divisor)
 * Allowed ranges for DRV8834 are 1:1 to 1:32
 * If the control pins are not connected, we recalculate the timing only
 *
 */
unsigned DRV8834::setMicrostep(unsigned divisor){
    BasicStepperDriver::setMicrostep(divisor);

    if (!IS_CONNECTED(m0_pin) || !IS_CONNECTED(m1_pin)){
        return microsteps;
    }

    /*
     * Step mode truth table
     * M1 M0    step mode
     *  0  0     1
     *  0  1     2
     *  0  Z     4
     *  1  0     8
     *  1  1    16
     *  1  Z    32
     *
     *  Z = high impedance mode (M0 is tri-state)
     */

    pinMode(m1_pin, OUTPUT);
    digitalWrite(m1_pin, (divisor < 8) ? LOW : HIGH);

    switch(divisor){
    case 1:
    case 8:
        pinMode(m0_pin, OUTPUT);
        digitalWrite(m0_pin, LOW);
        break;
    case 2:
    case 16:
        pinMode(m0_pin, OUTPUT);
        digitalWrite(m0_pin, HIGH);
        break;
    case 4:
    case 32:
        pinMode(m0_pin, INPUT); // Z - high impedance
        break;
    }
    return microsteps;
}
