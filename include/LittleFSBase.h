// Adapted from: https://github.com/espressif/arduino-esp32/blob/master/libraries/LittleFS/examples/LITTLEFS_test/LITTLEFS_test.ino
// Project details: https://RandomNerdTutorials.com/esp32-write-data-littlefs-arduino/

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

//  You only need to format LittleFS the first time you run a
//  test or else use the LITTLEFS plugin to create a partition
//  https://github.com/lorol/arduino-esp32littlefs-plugin

#define FORMAT_LITTLEFS_IF_FAILED true
#define FILE_NAME_LOG "log.txt"
#define FILE_NAME_DATA "data.txt"

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("[FS] Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("[FS] - failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("[FS]  - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("[FS] Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("[FS] - failed to open file for reading");
        return;
    }

    Serial.println("[FS] - read from file:");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("[FS] Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("[FS] - failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("[FS] - file written");
    }
    else
    {
        Serial.println("[FS] - write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("[FS] Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("[FS] - failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("[FS] - message appended");
    }
    else
    {
        Serial.println("[FS] - append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("- file renamed");
    }
    else
    {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path))
    {
        Serial.println("- file deleted");
    }
    else
    {
        Serial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char *path)
{
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
            Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("- failed to open file for reading");
    }
}

void setupFS()
{

    Serial.println(" - [LittleFS] build - ");

    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("[LittleFS] Mount Failed");
        return;
    }

    /*      LOG file creation   */
    bool datafolderexists = LittleFS.exists("/data");
    bool logfileexists = LittleFS.exists("/data/log.txt");
    bool datafileexists = LittleFS.exists("/data/data.txt");
    Serial.print(datafolderexists);
    Serial.print(logfileexists);
    Serial.print(datafileexists);
    if (!datafolderexists)
    {
        Serial.println("[FS] DataFolder doesn’t exist");
        Serial.println("[FS] Creating logfile...");
        // Create File and add header
        createDir(LittleFS, "/data");
    }
    else
    {
        /*       Serial.println("[FS] DataFolder already exists");
              listDir(LittleFS, "/data", 1); */
    }
    if (!datafileexists)
    {
        Serial.println("[FS] DataFile doesn’t exist");
        Serial.println("[FS] Creating datafile...");
        // Create File and add header
        writeFile(LittleFS, "/data/data.txt", "MY ESP32 DATA \r\n");
    }
    else
    {
        Serial.println("[FS] DataFile already exists OVERWRITING");
        writeFile(LittleFS, "/data/data.txt", "MY ESP32 DATA \r\n"); // TODO REMOVE THIS
        readFile(LittleFS, "/data/data.txt");
    }
    if (!logfileexists)
    {
        Serial.println("[FS] LogFile doesn’t exist");
        Serial.println("[FS] Creating logfile...");
        // Create File and add header
        writeFile(LittleFS, "/data/log.txt", "MY ESP32 DATA \r\n");
    }
    else
    {
        Serial.print("[FS] LogFile already exists");
        Serial.print("[FS] LogFile overwrite!!");
        writeFile(LittleFS, "/data/log.txt", "MY ESP32 DATA \r\n");
    }

    //    listDir(LittleFS, "/", 1);
    //    readFile(LittleFS, "/data/log.txt");
    // readFile(LittleFS, "/data/data.txt");
    //    testFileIO(LittleFS, "/data/data.txt"); // Testin

    /*
        createDir(LittleFS, "/test");                      // Create a test folder
        writeFile(LittleFS, "/test/hello1.txt", "Hello1"); // Create a hello1.txt file with the content "Hello1"
        listDir(LittleFS, "/", 1);                         // List the directories up to one level beginning at the root directory
        deleteFile(LittleFS, "/test/hello1.txt");          // delete the previously created file
        removeDir(LittleFS, "/test");                      // delete the previously created folder
        listDir(LittleFS, "/", 1);                         // list all directories to make sure they were deleted

        writeFile(LittleFS, "/hello.txt", "Hello ");      // Create and write a new file in the root directory
        appendFile(LittleFS, "/hello.txt", "World!\r\n"); // Append some text to the previous file
        readFile(LittleFS, "/hello.txt");                 // Read the complete file
        renameFile(LittleFS, "/hello.txt", "/foo.txt");   // Rename the previous file
        readFile(LittleFS, "/foo.txt");                   // Read the file with the new name
        deleteFile(LittleFS, "/foo.txt");                 // Delete the file
        testFileIO(LittleFS, "/test.txt");                // Testin
        deleteFile(LittleFS, "/test.txt");                // Delete the file

    */

    Serial.println("[FS] Test complete");
}

void loopFS()
{
}