#include "SdCardHandler.h"

SdCardHandler sdCardHandler;

void SdCardHandler::listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    _PRINTF("[SdCardHandler::listDir] Listing directory: \"%s\"\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        _PRINTLN("    | Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        _PRINTLN("    | Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            _PRINT("    | DIR : ");
            _PRINTLN(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            _PRINT("    | FILE: ");
            _PRINT(file.name());
            _PRINT("    | SIZE: ");
            _PRINTLN(file.size());
        }
        file = root.openNextFile();
    }
    _PRINTF("    | -- End of Dir \"%s\" --\n", dirname);
}

void SdCardHandler::createDir(fs::FS &fs, const char *path) {
    _PRINTF("[SdCardHandler::createDir] Creating Dir: \"%s\"\n", path);

    if (fs.exists(path)) {
        _PRINTLN("    | Dir already exists! Do nothing.");
        return;
    }

    if (fs.mkdir(path)) {
        _PRINTLN("    | Dir created");
    } else {
        _PRINTLN("    | mkdir failed");
    }
}

void SdCardHandler::removeDir(fs::FS &fs, const char *path) {
    _PRINTF("[SdCardHandler::removeDir] Removing Dir: \"%s\"\n", path);

    if (fs.exists(path)) {
        _PRINTLN("    | Dir does not exists! Do nothing.");
        return;
    }

    if (fs.rmdir(path)) {
        _PRINTLN("    | Dir removed");
    } else {
        _PRINTLN("    | rmdir failed");
    }
}

void SdCardHandler::readFile(fs::FS &fs, const char *path) {
    _PRINTF("[SdCardHandler::readFile] Reading file: \"%s\"\n", path);

    File file = fs.open(path);
    if (!file) {
        _PRINTLN("    | Failed to open file for reading");
        return;
    }

    _PRINT("    | Read from file: ");
    bool isFirstChar = true;
    bool isLastChar = !file.available();
    while (!isLastChar) {
        if (isFirstChar) {
            _PRINT("\n    | ");
            isFirstChar = false;
        }

        char temp = file.read();

        Serial.write(temp);
        isLastChar = !file.available();
        if (temp == '\n' && !isLastChar) {
            _PRINT("    | ");
        }
    }

    file.close();
}

String SdCardHandler::getFileString(fs::FS &fs, const char *path) {
    _PRINTF("[SdCardHandler::getFileString] Getting file: \"%s\"\n", path);

    File file = fs.open(path);
    if (!file) {
        _PRINTLN("    | Failed to open file for reading");
        return "";
    }

    bool isFirstChar = true;
    bool isLastChar = !file.available();

    String fileString = "";
    do {
        fileString += (char)file.read();
        isLastChar = !file.available();
    } while (!isLastChar);
    _PRINTF("    | file \"%s\": %s\n", path, fileString.c_str());

    file.close();
    return fileString;
}

bool SdCardHandler::writeFile(fs::FS &fs, const char *path, const char *message) {
    _PRINTF("[SdCardHandler::writeFile] Writing file: \"%s\"\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        _PRINTLN("    | Failed to open file for writing");
        return false;
    }
    if (!file.print(message)) {
        _PRINTLN("    | Write failed");
        file.close();
        return false;
    } else {
        _PRINTLN("    | File written");
        file.close();
        return true;
    }
}

void SdCardHandler::appendFile(fs::FS &fs, const char *path, const char *message) {
    _PRINTF("[SdCardHandler::appendFile] Appending to file: \"%s\"\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        _PRINTLN("    | Failed to open file for appending");
        return;
    }
    if (file.print(message)) {
        _PRINTLN("    | Message appended");
    } else {
        _PRINTLN("    | Append failed");
    }
    file.close();
}

void SdCardHandler::copyFile(fs::FS &fs, const char *pathSrc, const char *pathDes) {
    _PRINTF("[SdCardHandler::copyFile] Copying file: \"%s\" to \"%s\"\n", pathSrc, pathDes);
    // OPEN SRC
    File fileSrc = fs.open(pathSrc);
    if (!fileSrc) {
        _PRINTLN("    | Failed to open file for reading");
        return;
    }

    // CREATE DES
    if (fs.exists(pathDes)) {
        deleteFile(fs, pathDes);
    }

    File fileDes = fs.open(pathDes, FILE_WRITE);
    writeFile(fs, pathDes, "");
    fileDes.close();

    // READ SRC, APPEND DES
    fileDes = fs.open(pathDes, FILE_APPEND);
    if (!fileDes) {
        _PRINTLN("    | Failed to open file for appending");
        return;
    }
    _PRINT("    | Copying...");

    while (fileSrc.available()) {
        char temp = fileSrc.read();

        if (!fileDes.print(temp)) {
            _PRINTLN(" failed");
            return;
        }
    }

    _PRINTLN(" done!");
    fileSrc.close();
    fileDes.close();
}

void SdCardHandler::renameFile(fs::FS &fs, const char *path1, const char *path2) {
    _PRINTF("[SdCardHandler::renameFile] Renaming file \"%s\" to \"%s\"\n", path1, path2);
    if (fs.rename(path1, path2)) {
        _PRINTLN("    | File renamed");
    } else {
        _PRINTLN("    | Rename failed");
    }
}

bool SdCardHandler::deleteFile(fs::FS &fs, const char *path) {
    _PRINTF("[SdCardHandler::deleteFile] Deleting file: \"%s\"\n", path);
    if (fs.remove(path)) {
        _PRINTLN("    | File deleted");
        return true;
    } else {
        _PRINTLN("    | Delete failed");
        return false;
    }
}

bool SdCardHandler::fileExists(fs::FS &fs, const char *path) {
    return fs.exists(path);
}

bool SdCardHandler::deleteAllFiles(fs::FS &fs, const char *dirname, uint8_t levels) {
    File root = fs.open(dirname);
    if (!root) {
        _PRINTLN("    | Failed to open directory");
        return false;
    }
    if (!root.isDirectory()) {
        _PRINTLN("    | Not a directory");
        return false;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            _PRINT("    | DIR : ");
            _PRINTLN(file.name());
            if (levels) {
                deleteAllFiles(fs, file.name(), levels - 1);
            }
        } else {
            char *path = (char *)malloc(sizeof(char) * 100);
            sprintf(path, "%s", file.name());
            deleteFile(fs, path);
            free(path);
        }
        file = root.openNextFile();
    }
    return true;
}

bool SdCardHandler::deleteAllDirs(fs::FS &fs, const char *dirname, uint8_t levels) {
    File root = fs.open(dirname);
    if (!root) {
        _PRINTLN("    | Failed to open directory");
        return false;
    }
    if (!root.isDirectory()) {
        _PRINTLN("    | Not a directory");
        return false;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            _PRINT("    | DIR : ");
            _PRINTLN(file.name());
            if (levels) {
                deleteAllFiles(fs, file.name(), levels - 1);
            }
            char *path = (char *)malloc(sizeof(char) * 100);
            sprintf(path, "%s", file.name());
            removeDir(fs, path);
            free(path);
        }
        file = root.openNextFile();
    }
    return true;
}

bool SdCardHandler::deleteAll(fs::FS &fs, const char *dirname, uint8_t levels) {
    _PRINTLN("[SdCardHandler::deleteAll]");
    if (deleteAllFiles(fs, dirname, levels) && deleteAllDirs(fs, dirname, levels))
        return true;
    else
        return false;
}

void SdCardHandler::testFileIO(fs::FS &fs, const char *path) {
    _PRINTLN("[SdCardHandler::testFileIO]");
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        _PRINTF("    | %u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        _PRINTLN("    | Failed to open file for reading");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file) {
        _PRINTLN("    | Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    _PRINTF("    | %u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

bool SdCardHandler::setup() {
    _PRINTLN("[SdCardHandler::setup]");
    SPI.begin(SDCARD_SCK_PIN, SDCARD_MISO_PIN, SDCARD_MOSI_PIN, SDCARD_CS_PIN);
    if (!SD.begin(SDCARD_CS_PIN)) {
        _PRINTLN("    | Card Mount Failed");

        const long restartDelay = 5000;
        _PRINTF("    | Restarting in %d seconds...\n", restartDelay / 1000);
        delay(restartDelay);
        ESP.restart();

        return false;
    }

    if (!cardCheck()) {
        _PRINTLN("    | SD Card Init Failed");
        return false;
    }

    return true;
}

void testIo();

bool SdCardHandler::cardCheck() {
    // Check the card details
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        _PRINTLN("    | No SD card attached");
        return false;
    }

    _PRINT("    | SD Card Type: ");
    if (cardType == CARD_MMC) {
        _PRINTLN("MMC");
    } else if (cardType == CARD_SD) {
        _PRINTLN("SDSC");
    } else if (cardType == CARD_SDHC) {
        _PRINTLN("SDHC");
    } else {
        _PRINTLN("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    _PRINTF("    | SD Card Size: %lluMB\n", cardSize);

    _PRINTF("    | Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    _PRINTF("    | Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    // Do I/O actions for checking purposes
    // testIo();

    return true;
}

/**
 * ================= TESTS ================= 
 */

void testIo() {
    sdCardHandler.listDir(SD, "/", 0);
    sdCardHandler.createDir(SD, "/mydir");
    sdCardHandler.listDir(SD, "/", 0);
    sdCardHandler.removeDir(SD, "/mydir");
    sdCardHandler.listDir(SD, "/", 2);
    sdCardHandler.writeFile(SD, "/hello.txt", "Hello ");
    sdCardHandler.appendFile(SD, "/hello.txt", "World!\n");
    sdCardHandler.readFile(SD, "/hello.txt");
    sdCardHandler.renameFile(SD, "/hello.txt", "/foo.txt");
    sdCardHandler.readFile(SD, "/foo.txt");
    sdCardHandler.deleteFile(SD, "/foo.txt");
}