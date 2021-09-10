#include "Utilities.h"
#include "Internet/InternetHandler.h"
#include "Command.h"
#include "Sampler.h"
#include "PumpHandler.h"
#include "Mcp4725Handler.h"
#include "FlowSensor.h"
#include "esp_task_wdt.h"

/**
 * @brief The default sampling start
 * to start, input to serial
 * "samplingStart:" // don't forget the :
 */ 
void samplingStart() {
    // lakukan pembacaan hingga buffer penuh
    loadcellSampler.set("totalSamplingStopMode", "buffer");
    loadcellSampler.set("totalSamplingBufferSize", "100");
    loadcellSampler.start();    
    delay(5000);

    // kembali lakukan pembacaan, tetapi hidupkan water pump
    loadcellSampler.start();
    pumpOut.start();
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
    // loadcellSampler.start();
    // pumpIn.start();
}

// MAIN
void setup() {
    Serial.begin(115200);
    esp_task_wdt_init(120, false); // turn of wdt panic, so never restart
    _PTN("=========== LOADCELL:INIT START ===========");

    internetHandler.connect();
    mcp4725Handler.init();

    florSensorIn.init(FLOWSENSOR_IN_PIN);
    florSensorOut.init(FLOWSENSOR_OUT_PIN);
    attachInterrupt(FLOWSENSOR_IN_PIN, flowInTickIsr, RISING);
    attachInterrupt(FLOWSENSOR_OUT_PIN, flowOutTickIsr, RISING);

    pumpIn.set("debitTarget", "20.0");
    pumpIn.init(PUMP_IN_PIN);
    pumpOut.set("debitTarget", "0.0");
    pumpOut.init(PUMP_OUT_PIN);
    
    loadcellSampler.init();
    
    serialCommand.init();

    _PTN("=========== LOADCELL:INIT DONE ===========");
}

void loop() {
    vTaskDelete(NULL); // just delete main loop task
}