#pragma once

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "PinDefinitions.h"
#include "Utilities.h"

/* MADE TO RUN ON ESP32
 *  CANNOT RUN ON ARDUINO
 * PINOUT DIAGRAM:
 * MicroSD card module ESP32
      VIN ------------ 5V
      CS  ------------ GPIO 5
      CLK  ----------- GPIO 18
      MOSI ----------- GPIO 23
      MISO ----------- GPIO 19
      GND ------------ GND
 
              MOSI  MISO  SCK CS
  Arduino Uno 11  12  13  10
  Arduino Nano  11  12  13  10
  Arduino Mega  51  50  52  53

 */

class SdCardHandler {
   public:
    bool setup();

    // BASIC FUNCTIONS
    void listDir(fs::FS &, const char *, uint8_t);
    void createDir(fs::FS &, const char *);
    void removeDir(fs::FS &, const char *);
    void readFile(fs::FS &, const char *);
    String getFileString(fs::FS &, const char *);
    bool writeFile(fs::FS &, const char *, const char *);
    void appendFile(fs::FS &, const char *, const char *);
    void copyFile(fs::FS &, const char *, const char *);
    void renameFile(fs::FS &, const char *, const char *);
    bool deleteFile(fs::FS &, const char *);
    void testFileIO(fs::FS &, const char *);
    bool fileExists(fs::FS &, const char *);
    bool deleteAll(fs::FS &, const char *, uint8_t);
    bool deleteAllDirs(fs::FS &, const char *, uint8_t);
    bool deleteAllFiles(fs::FS &, const char *, uint8_t);
    
    bool cardCheck();
};

extern SdCardHandler sdCardHandler;