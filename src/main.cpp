#include "Command.h"
#include "Peripherals/DacHandler.h"
#include "Peripherals/FlowSensor.h"
#include "Internet/InternetHandler.h"
#include "Peripherals/PumpHandler.h"
#include "Sampler.h"
#include "Utilities.h"
#include "esp_task_wdt.h"

#define LOADCELL_THRESHOLD 0

/**
 * @brief This function checks if any loadcell have reached threshold
 */
bool checkLoadThreshold() {
    uint16_t * sensorValues = loadcellSampler.getSensorValues();
    for (int i=0; i<4; i++) {
        if (sensorValues[i] == LOADCELL_THRESHOLD) {
             _PTN("[checkLoadThreshold] triggered!");
            return true;
        }
    }
    return false;
}

/**
 * @brief The default sampling start for demo purposes
 * to start, input to serial
 * "samplingStartDemo:" // don't forget the ':'
 */
void samplingStartDemo() {
    // lakukan pembacaan hingga buffer penuh
    loadcellSampler.set("avgSamplingBufferSize", "60");
    loadcellSampler.set("totalSamplingStopMode", "buffer");
    loadcellSampler.set("totalSamplingBufferSize", "100");
    loadcellSampler.start();
    bool samplingState;
    samplingState = loadcellSampler.getState();
    do {
        samplingState = loadcellSampler.getState();
    } while (samplingState);
    _PTN("loadcellSampler done#1");
    delay(5000);

    // kembali lakukan pembacaan, tetapi hidupkan water pump
    loadcellSampler.start();
    pumpOut.start();
    samplingState = loadcellSampler.getState();

    // will stop if buffer filled fully
    do {
        samplingState = loadcellSampler.getState();
    } while (samplingState);
    _PTN("loadcellSampler done#2");
    pumpOut.stop();
    delay(5000);

    /** 
     * kembali lakukan pembacaan, tetapi hidupkan water pump. cukup lakukan
     * bagian ini secara manual melalui serial command, 
     * 
     * startLc:
     * setPi:state=start;
     * 
     * tetapi kalau mau otomatis, 
     * tinggal uncomment dibawah ini
     */
    loadcellSampler.start();
    pumpIn.start();
    bool sampleThreshold;
    bool toStop;

    // will stop if loadcell reach threshold, or buffer filled fully
    do {
        sampleThreshold = checkLoadThreshold();
        samplingState = loadcellSampler.getState();
    } while (sampleThreshold || samplingState);
    _PTN("loadcellSampler done#3");
    pumpIn.stop();
    
}

/**
 * @brief connect to internet, activate DAC to low value,
 * flowsensor in and pump in activate with debitTarget,
 * flowsensor out and pump out activate with debitTarget,
 * loadcellSampler start sampling, serial command start
 * listening for serial inputs
 */ 
void setup() {
    Serial.begin(115200);
    esp_task_wdt_init(120, false);  // turn off wdt panic, so never restart
    _PTN("=========== LOADCELL:INIT START ===========");

    internetHandler.connect();
    dacHandler.init();

    flowSensorIn.init("flowSensorIn", FLOWSENSOR_IN_PIN);
    attachInterrupt(FLOWSENSOR_IN_PIN, flowInTickIsr, RISING);
    pumpIn.set("debitTarget", "20.0");
    pumpIn.init("pumpIn", PUMP_IN_PIN);

    flowSensorOut.init("flowSensorOut", FLOWSENSOR_OUT_PIN);
    attachInterrupt(FLOWSENSOR_OUT_PIN, flowOutTickIsr, RISING);
    pumpOut.set("debitTarget", "0.0");
    pumpOut.init("pumpOut", PUMP_OUT_PIN);

    loadcellSampler.init();

    _PTN("=========== LOADCELL:INIT DONE ==========="); 

    serialCommand.init();
}

void loop() {
    vTaskDelete(NULL);  // just delete main loop task
}