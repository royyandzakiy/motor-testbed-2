#include "LoadSensor.h"

HX711 scale(DOUT, CLK);

void LoadSensor::init() {
    scale.set_scale();
    scale.tare();  // Reset the scale to 0

    long zero_factor = scale.read_average();  // Get a baseline reading
    Serial.print("Zero factor: ");            // This can be used to remove the need to tare the scale. Useful in permanent scale projects.
    Serial.println(zero_factor);
}

float calibration_factor = -109525; //-106600 worked for my 40Kg max scale setup
unsigned int avgSamplingBufferSize = 10;
unsigned long rawSampleInterval = 1000000;  // 1 second

float LoadSensor::read() {
    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    return scale.get_units();   
}

float* LoadSensor::avgSampling() {
    _PTN("LoadSensor::avgSampling()");

    // initialize variables
    bool stopSampling = false;
    unsigned int rawSampleCount = 0;
    unsigned long start = micros();

    while (!stopSampling) {
        scale.set_scale(calibration_factor); //Adjust to this calibration factor
        sensorValues += scale.get_units();

        delayMicroseconds(rawSampleInterval);
        rawSampleCount++;
        stopSampling = rawSampleCount >= avgSamplingBufferSize;  // stop based on buffer size
    }

    float* avgSamples = (float*)malloc(sizeof(float));

    avgSamples[0] = (float)(sensorValues / rawSampleCount);

    // #ifdef SAMPLING_PRINT_VERBOSE
    _PTF("    |\n");
    _PTF("    | rawSampleCount: %d\n", rawSampleCount);
    _PTF("    | avgSampling time elapsed: %lu us\n", micros() - start);
    // #endif  // SAMPLING_PRINT_VERBOSE

    return avgSamples;
}