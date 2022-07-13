/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/cademissner/Desktop/IoT/Capstone/src/Capstone.ino"
void mqttLoop();
void checkPractices();
void metronomeAndTimer();
void setup();
void loop();
#line 1 "/Users/cademissner/Desktop/IoT/Capstone/src/Capstone.ino"
SYSTEM_THREAD(ENABLED);

#include "oled-wing-adafruit.h"
#include <blynk.h>
#include "MQTT.h"
// #include <SPI.h>
// #include "SdFat.h"
// #include "Adafruit_VS1053.h"
// #include "Adafruit_LiquidCrystal.h"

#define KtimeOn 50
#define BUZZER D6

// SdFat SD;
const int MP3_RESET = -1;
const int SD_CS = D2;
const int MP3_CS = D3;
const int DREQ = D4;
const int MP3_DCS = D5;
// D7 also used

OledWingAdafruit display;
// LiquidCrystal lcd(0);

int timer;
unsigned long time0;
unsigned long time1;
int time2;
int time3;
int time4;
int time5;
int buzz = 0;

int value = 0;
unsigned long myTimer = millis();
unsigned long timeOn = millis();

bool isOn = false;
bool metronome;

int timeBetween;


// int fileNum = 0;
// bool music = false;
// unsigned long timerOfMine = millis();

// int fileScroll = 0;
// unsigned long scrollTime = millis();
// int fileScrollDiff = 16;

// unsigned long secondLineTime = millis();
// bool secondLine = true;
// String lineTwo = "Loading...";

// unsigned long songTimer;

// long fileTimes[7] = {302, 273, 263, 280, 249, 346, 225};

// int fileBpm[7] = {176, 42, 63, 120, 88, 88, 69};

// String files[7] = {"1.mp3", "2.mp3", "3.mp3", "4.mp3", "5.mp3", "6.mp3", "7.mp3"};

// String fileNames[7] = {"Hard Rock Backing Track", "Piano Backing Track", "Acoustic Backing Track", "Funk Backing Track", "R n B Backing Track", "Soft Rock Backing Track", "Orchestral Backing Track"};

// Adafruit_VS1053_FilePlayer musicPlayer(MP3_RESET, MP3_CS, MP3_DCS, DREQ, SD_CS);



void callback(char* topic, byte* payload, unsigned int length);
MQTT client("lab.thewcl.com", 1883, callback);

int practices = -1;
int practicesNeeded = -1;

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;

  int recievedInt = atoi(p);
  Serial.println(recievedInt);
  if (String(topic).equals("cade/capstone/calendarout") && practices != -1) {
    if (recievedInt >= practices) {
      practicesNeeded = 0;
    } else if (recievedInt < practices) {
      practicesNeeded = practices - recievedInt;
    }
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V3);
}

BLYNK_WRITE(V0) {
  value = param.asInt();
  Serial.println("Slider");
}

BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    timer = 0;
    buzz = 0;
  }
  Serial.println("Reset");
}

BLYNK_WRITE(V3) {
  if (param.asInt() == 0) {
    metronome = false;
  } else {
    metronome = true;
  }
  Serial.println("Metronome Toggle");
}

BLYNK_WRITE(V1) {
  if (practices != param.asInt()) {
    practices = param.asInt();
    client.publish("cade/capstone/calendarin", "go");
  }
  Serial.println("Practices");
}

// BLYNK_WRITE(V4) {
//   if (millis() >= timerOfMine + 2000) {
//     musicPlayer.pausePlaying(music);
//     music = !music;
//     timerOfMine = millis();
//   }
// }

// BLYNK_WRITE(V5) {
//   if (millis() >= timerOfMine + 2000) {
//     playMusic();
//     timerOfMine = millis();
//   }
// }

// BLYNK_WRITE(V6) {
//   if (millis() >= timerOfMine + 2000) {
//     fileNum = param.asInt() - 1;
//     playMusic();
//     timerOfMine = millis();
//   }
// }

void mqttLoop() {
  if (client.isConnected()) {
    client.loop();
  } else {
    client.connect(System.deviceID());
    client.subscribe("cade/capstone/calendarout");

    client.publish("cade/capstone/calendarin", "go");
  }
}

void checkPractices() {
  if (practicesNeeded == 0) {
    Blynk.notify("Great job creating enough practice events this week!");
    practicesNeeded = -1;
  } else if (practicesNeeded != -1) {
    Blynk.notify(String("You need to add ") + String(practicesNeeded) + String(" more practice events this week."));
    practicesNeeded = -1;
  }
}

void metronomeAndTimer() {
  if (metronome) {
    timeBetween = (60000 / value);

    if (millis() >= (myTimer + timeBetween)) {
      myTimer = millis();
      digitalWrite(BUZZER, HIGH);
      timeOn = millis();
      isOn = true;
    }

    if (isOn == true) {
      if (millis() >= (timeOn + KtimeOn)) {
        digitalWrite(BUZZER, LOW);
        isOn = false;
      }
    }
  } else if (metronome == false) {
    digitalWrite(BUZZER, LOW);
  }

  if (display.pressedA()) {
    timer = timer + 60000;
    buzz = 50;
  } else if (display.pressedB()) {
    timer = timer + 300000;
    buzz = 50;
  } else if (display.pressedC()) {
    timer = timer + 600000;
    buzz = 50;
  }

  time0 = time1;
  time1 = millis();
  time2 = time1 - time0;

  timer = timer - time2;

  if (timer < 1) {
    if (buzz > 1) {
      digitalWrite(BUZZER, HIGH);
      buzz = buzz - 1;
    }

    timer = 0;
  }

  display.setCursor(0, 0);
  display.clearDisplay();

  time3 = int(timer) / 1000;
  time4 = time3 % 60;
  time5 = floor(time3 / 60);
  display.println("Timer:");
  display.print(time5);
  display.print(":");
  display.println(time4);
  display.display();
}

// void musicSetup() {
//   if (!SD.begin(SD_CS)) {
//     while (1)
//       yield(); // don't do anything more
//   }
//   Serial.println("SD OK!");

//   // initialise the music player
//   if (!musicPlayer.begin()) { // initialise the music player
//     while (1)
//       yield();
//   }

//   musicPlayer.setVolume(1, 1);

//   if (musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
//     digitalWrite(D7, HIGH);
//     musicPlayer.setIsrCallback(blink);
//   }
// }

// void musicAndLCD() {
//   if (millis() >= scrollTime + 350) {
//     lcd.clear();
//     lcd.setCursor(fileScrollDiff, 0);
//     String toDisect = fileNames[fileNum];
//     String toDisplay;
//     int nameLength = toDisect.length();

//     if (!music) {
//       lineTwo = "No Music Playing";
//       toDisect = "No Music Playing";
//     } else {
//       if (secondLine) {
//         lineTwo = "BPM: " + String(fileBpm[fileNum]);
//       } else {
//         unsigned long playDuration = millis() - songTimer;
//         float playSeconds = playDuration / 1000.0 - 0.5; // so it always rounds down
//         playDuration = fileTimes[fileNum] - int(playSeconds);
//         String extraZero = "";

//         if (playDuration < 1) {
//           playDuration = 0;
//         }

//         int seconds = playDuration % 60;
//         if (seconds < 10) {
//           extraZero = "0";
//         }

//           lineTwo = "Time Left: " + String(int((playDuration / 60) - 0.5)) + ":" + extraZero + String(playDuration % 60);
//       }
//     }

//     if (0 < fileScrollDiff) {
//       unsigned int pos = fileScroll + 16;
//       toDisplay = toDisect.substring(0, pos - fileScrollDiff);
//       fileScrollDiff--;
//     } else if (nameLength >= fileScroll) {
//       unsigned int pos = fileScroll + 16;
//       toDisplay = toDisect.substring(fileScroll, pos);
//       fileScroll++;
//     } else {
//       fileScrollDiff = 16;
//       fileScroll = 0;
//     }

//     lcd.print(toDisplay);
//     lcd.setCursor(0, 1);
//     lcd.print(lineTwo);
//     scrollTime = millis();
//   }

//   if (millis() >= secondLineTime + 10000) {
//     secondLineTime = millis();
//     secondLine = !secondLine;
//   }
// }

// void playMusic() {
//   if (fileNum == 0) {
//     musicPlayer.setVolume(5, 5);
//   } else {
//     musicPlayer.setVolume(0, 0);
//   }

//   musicPlayer.pausePlaying(false);
//   music = true;
//   musicPlayer.startPlayingFile(files[fileNum]);
//   songTimer = millis();
// }

// void blink(void) {
//   digitalWriteFast(D7, !pinReadFast(D7));
// }

void setup() {
  Blynk.begin("gev9UlWHupS8yCBJ2wpDXDNm6THMu0oS", IPAddress(167, 172, 234, 162), 8080);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // lcd.begin(16, 2);
  // lcd.setBacklight(HIGH);
  // pinMode(D7, OUTPUT);
  // musicSetup();

  Serial.begin(9600);

  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();

  // musicPlayer.startPlayingFile("1.mp3");
}

void loop() {
  Blynk.run();
  display.loop();

  mqttLoop();
  checkPractices();

  // musicAndLCD();
  
  metronomeAndTimer();
}