#include <M5Core2.h>

const int maxFile = 100;
String fileList[maxFile];
int fileCount = 0;
uint8_t image[256][256] = {};


void setup() {
  M5.begin();
  delay(500);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(1);
  sdcard_type_t Type = SD.cardType();
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  dacWrite(25,128);
  dacWrite(26,128);  
  if (Type == CARD_UNKNOWN || Type == CARD_NONE) {
    Serial.printf("SDCard error \r\n");
    M5.Lcd.println("SDCard error");
    while (1)
      ;
  }
  M5.Lcd.println("SDCard exist");
  Serial.printf("SDCard Type = %d \r\n", Type);
  Serial.printf("SDCard Size = %d Mbyte\r\n", (int)(SD.cardSize() / 1024 / 1024));
  Serial.printf("SDCard Use Size = %d Kbyte\r\n", (int)(SD.usedBytes() / 1024));

  if (!SD.exists("/image.raw")) {
    Serial.printf("image.raw not exist \r\n");
    M5.Lcd.println("image.raw not exist");
    while (1)
      ;
  }
  M5.Lcd.println("image.raw exist");
  File f = SD.open("/image.raw", FILE_READ);
  if (f) {
    for (int i = 0; i < 256; i++) {
      for (int j = 0; j < 256; j++) {
        if (f.available()) {
          image[i][j] = f.read();
        }
      }
    }
  }
  f.close();

  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      if (image[i][j] < 128) {
        Serial.print(" ");
      }else{
        Serial.print("*");
      }
    }
    Serial.println("");
  }
}

void loop() {
for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      if (image[i][j] > 128) {
        dacWrite(25,255-i);
        dacWrite(26,j);
        delayMicroseconds(3);
      }
    }
  }  
}