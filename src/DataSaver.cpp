#include "DataSaver.h"
TaskHandle_t dataSaverHandle;



const String DataSaver::sammplesToString(const float * samples, const int samplesCount) {
    
    return "";
}


bool DataSaver::toFs(const String _samplesStr) {
    // delete if file exist
    // write new file
    // close
}

bool DataSaver::toMqtt(const String _samplesStr) {
    // publish
}

bool DataSaver::toHttp(const String _samplesStr) {
    // send http request
}

void dataSaverTask(void* pvParameters) {
    // receive data from pvParameters
    // cast to String/File for upload
    // save data to SPIFFS
    // upload using pubsubclient/http

    _PTN("[dataSaverTask] Hi!");

    vTaskDelete(NULL);
}