#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define RELAY_PIN 18

// Wokwi Virtual Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// NTP Configuration (IST UTC +5:30 = 19800 seconds)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 0;

// Class Slot Data Structure
struct Slot {
  int startHour;
  int startMin;
  int endHour;
  int endMin;
  const char* subject;
  const char* faculty;
  const char* room;
};

// College Period Timetable
const int TOTAL_SLOTS = 4;
Slot timetable[TOTAL_SLOTS] = {
  { 9,  0, 10,  0, "CS401: OS",      "Prof. Sharma",  "LH-201" },
  { 10, 0, 11,  0, "CS402: Networks", "Dr. Banerjee", "LH-201" },
  { 11, 0, 11, 30, "LUNCH BREAK",    "---",           "LH-201" },
  { 17, 0, 18,  0, "CS405: Embedded", "Prof. Roy",     "LH-201" } // Set to match testing window
};

bool bellActive = false;
int lastRungMinute = -1;

void triggerBell(int durationMs) {
  Serial.println(">>> PERIOD TRANSITION: Ringing Bell! <<<");
  digitalWrite(RELAY_PIN, HIGH); // Relay closes -> activates buzzer
  
  // Show alert on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18, 15);
  display.print("PERIOD");
  display.setCursor(26, 38);
  display.print("BELL!");
  display.display();

  delay(durationMs);
  digitalWrite(RELAY_PIN, LOW); // Relay opens
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Connecting to");
  display.println("Wokwi-GUEST WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected! Syncing NTP...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for NTP synchronization...");
    delay(1000);
    return;
  }

  int curH = timeinfo.tm_hour;
  int curM = timeinfo.tm_min;
  int curS = timeinfo.tm_sec;

  // Search active timetable slot
  int activeIndex = -1;
  int curTotalMin = curH * 60 + curM;

  for (int i = 0; i < TOTAL_SLOTS; i++) {
    int startTotal = timetable[i].startHour * 60 + timetable[i].startMin;
    int endTotal   = timetable[i].endHour * 60 + timetable[i].endMin;

    if (curTotalMin >= startTotal && curTotalMin < endTotal) {
      activeIndex = i;
      break;
    }
  }

  // Check if current minute is a bell trigger time (period start or end)
  for (int i = 0; i < TOTAL_SLOTS; i++) {
    if ((curH == timetable[i].startHour && curM == timetable[i].startMin) ||
        (curH == timetable[i].endHour   && curM == timetable[i].endMin)) {
      if (lastRungMinute != curM) {
        lastRungMinute = curM;
        triggerBell(3500); // Ring for 3.5 seconds
      }
    }
  }

  // Render Smart Classroom Info on OLED
  char timeStr[10];
  sprintf(timeStr, "%02d:%02d:%02d", curH, curM, curS);

  display.clearDisplay();

  // Header: Room and Live Clock
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("ROOM: LH-201");
  display.setCursor(76, 0);
  display.print(timeStr);

  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Body: Current Session
  if (activeIndex != -1) {
    display.setCursor(0, 16);
    display.print("Subject:");
    display.setCursor(0, 27);
    display.setTextSize(1);
    display.print(timetable[activeIndex].subject);

    display.setCursor(0, 42);
    display.print("Faculty:");
    display.setCursor(0, 53);
    display.print(timetable[activeIndex].faculty);
  } else {
    display.setCursor(20, 28);
    display.setTextSize(1);
    display.print("CLASSROOM FREE");
    display.setCursor(12, 44);
    display.print("(No Active Lecture)");
  }

  display.display();
  delay(1000);
}
