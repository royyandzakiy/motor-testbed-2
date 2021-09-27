#include "Peripherals/PumpHandler.h"

PumpHandler pumpIn;
PumpHandler pumpOut;

#define PWM_BIT_RESOLUTION 8 // change this to the desired resolution

#if(PWM_BIT_RESOLUTION == 8)
#define PWM_MAX_BIT_VALUE 255
#elif(PWM_BIT_RESOLUTION == 9)
#define PWM_MAX_BIT_VALUE 511
#elif(PWM_BIT_RESOLUTION == 10)
#define PWM_MAX_BIT_VALUE 1023
#endif

// PWM Properties
const int freq = 5000;
const int pumpChannel = 0;
const int resolution = PWM_BIT_RESOLUTION;

void PumpHandler::init(const char * _name, uint8_t _pin) {
    name = _name;
    pin = _pin;

    _PTF("[PumpHandler::init] \"%s\" on gpio pin %d initiated\n", name, pin);
    ledcSetup(pumpChannel, freq, resolution);
    ledcAttachPin(pin, pumpChannel);

    state = false;

    kp = 0.1;
    ki = 1.0;
    error = 0.0;
    errorSum = 0.0;
}

unsigned int PumpHandler::get() {
    return dutyCycle;
}

void PumpHandler::set(const unsigned int _dutyCycle) {
    dutyCycle = _dutyCycle;
    if (state) {
        poll();
    }
}

void PumpHandler::start() {
    _PTF("[PumpHandler::start] %s started\n", name);
    state = true;
    poll();
}

void PumpHandler::stop() {
    _PTF("[PumpHandler::stop] %s stopped\n", name);
    state = false;
    ledcWrite(pumpChannel, 0);
}

void PumpHandler::poll() {
    ledcWrite(pumpChannel, dutyCycle);
}

bool PumpHandler::isActive() {
    return state;
}

void ledTest() {
    for (int dutyCycle = 0; dutyCycle <= PWM_MAX_BIT_VALUE; dutyCycle++) {
        pumpIn.set(dutyCycle);
        delay(15);
    }
    for (int dutyCycle = PWM_MAX_BIT_VALUE; dutyCycle >= 0; dutyCycle--) {
        pumpIn.set(dutyCycle);
        delay(15);
    }
}

void PumpHandler::meassure(float desiredDebit) {
    error = desiredDebit - flowSensorOut.get();
    errorSum+=error;
    
    float pwmVal = kp * error + ki * errorSum;
    if (pwmVal > PWM_MAX_BIT_VALUE) {
        pwmVal = PWM_MAX_BIT_VALUE;
    } else if (pwmVal < 0) {
        pwmVal = 0;
    }
    if (errorSum > 1000) {
        errorSum = 1000;
    } else if (errorSum < -1000) {
        errorSum = -1000;
    }
    
    set(PWM_MAX_BIT_VALUE - (int)pwmVal);
}

float PumpHandler::get(const char* _key) {
    if (strcmp(_key, "ki") == 0) {
        return kp;
    } else if (strcmp(_key, "kp") == 0) {
        return ki;
    } else if (strcmp(_key, "debitTarget") == 0) {
        return debitTarget;
    } else {
        _PTF("    | ERROR: \"%s\" not found.\n", _key);
    }
}

void PumpHandler::set(const char* _key, const char* _value) {
    if (strcmp(_key, "ki") == 0) {
        kp = atof(_value);
    } else if (strcmp(_key, "kp") == 0) {
        ki = atof(_value);
    } else if (strcmp(_key, "debitTarget") == 0) {
        debitTarget = atof(_value);
    } else if (strcmp(_key, "start") == 0) {
        start();
    } else if (strcmp(_key, "stop") == 0) {
        stop();
    } else {
        _PTF("    | ERROR: \"%s\" not found.\n", _key);
    }
}

void PumpHandler::printConfiguration() {
    _PTF("    | ------ PUMP CONFIGURATION ------\n");
    _PTF("    | \n");
    _PTF("    |     ki: %f\n", get("ki"));
    _PTF("    |     kp: %f\n", get("kp"));
    _PTF("    |     debitTarget: %f\n", get("debitTarget"));
    _PTF("    |     state: %s\n", state ? "running" : "stopped");
    _PTF("    | \n");
    _PTF("    | ---------------------------\n");
}