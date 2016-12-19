#include <SdFat.h>
#include <SdFatUtil.h>
#include <SPI.h>
#include <SFEMP3Shield.h>

SdFat sd;
SFEMP3Shield MP3player;

void(* resetFunc) (void) = 0;

void setup() {
  randomSeed(analogRead(A0));
  uint8_t result;
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  Serial.begin(9600);
  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
//  if (result != 0) {
//    Serial.print(F("Error code: "));
//    Serial.print(result);
//    Serial.println(F(" when trying to start MP3 player"));
//    if ( result == 6 ) {
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

//      Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
//      Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
//    }
//  }

  //MP3player.setPlaySpeed(2);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(5) == 0) {
    Serial.println("Playing");
    uint8_t result = MP3player.playTrack(7);
    delay(150000);
    digitalWrite(10, HIGH);
    delay(10);
    digitalWrite(10, LOW);
  }

  if (digitalRead(11) == 0) {
    MP3player.stopTrack();
    Serial.println("Stopping");
  }
}

//365-375 100
//95-100 150

void initMP3Player()
{
  uint8_t result = MP3player.begin(); // init the mp3 player shield
  if (result != 0) // check result, see readme for error codes.
  {
    // Error checking can go here!
  }
  MP3player.setVolume(225, 225);
  MP3player.setMonoMode(1);
}

void initSD()
{
  //Initialize the SdCard.
  if (!sd.begin(SD_SEL, SPI_HALF_SPEED))
    sd.initErrorHalt();
  if (!sd.chdir("/"))
    sd.errorHalt("sd.chdir");
}

