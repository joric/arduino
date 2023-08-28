// exterminate! video: http://webm.armarium.org/i/zBTtIo.mp4

#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>
#include <Ultrasonic.h>
#include <Servo.h>

const int chipSelect = 10;
const int speaker = 9;
const int servoPin = A0;
const int ultraTrq = 2;
const int ultraEcho = 3;

char * fname = (char*)"EXTERMIN.WAV";

Servo servo;
TMRpcm tmrpcm;
Ultrasonic ultrasonic(ultraTrq, ultraEcho);

Sd2Card card;
SdVolume volume;
SdFile root;

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.print("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  Serial.println("card initialized.");
  tmrpcm.speakerPin = speaker;
  tmrpcm.volume(1);
}

void loop() {
  float dist_cm = ultrasonic.Ranging(CM);
  if (dist_cm < 10) {
    tmrpcm.play(fname);
    delay(2000);
    servo.attach(servoPin);
    servo.write(180);
    delay(500);
    servo.write(0);
    delay(500);
    servo.write(90);
    delay(500);
    servo.detach();
    delay(1000);
  }
}
