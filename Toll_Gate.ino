#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9

// IR sensors
#define IR_ENTRY 2
#define IR_EXIT 3

// LEDs
#define LED_RED 6
#define LED_GREEN 7

// Servo
#define SERVO_PIN 5

Servo gateServo;
MFRC522 rfid(SS_PIN, RST_PIN);

// Servo positions
const int GATE_OPEN = 90;
const int GATE_CLOSED = 0;

// ---- STORED RFID CARDS (4-byte UIDs) ----
byte Tag1[4] = {0x02, 0xC9, 0x8B, 0x41};
byte Tag2[4] = {0x09, 0xD6, 0x8B, 0x5A};
byte Tag3[4] = {0x23, 0xD8, 0x50, 0xD0};

// ---- BALANCES ----
int balance1 = 500;
int balance2 = 300;
int balance3 = 0;

int tollAmount = 20;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(IR_ENTRY, INPUT);
  pinMode(IR_EXIT, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  gateServo.attach(SERVO_PIN);
  gateServo.write(GATE_CLOSED);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);

  Serial.println("Toll System Initialized...");
}

bool uidEquals(byte *a, byte *b) {
  for (byte i = 0; i < 4; i++)
    if (a[i] != b[i]) return false;
  return true;
}

void printUID(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] < 0x10) Serial.print("0");
    Serial.print(uid[i], HEX);
    if (i < 3) Serial.print(" ");
  }
}

void waitForCardRemoval() {
  while (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    delay(50);
  }
}

void loop() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  gateServo.write(GATE_CLOSED);

  if (digitalRead(IR_ENTRY)) {
    Serial.println("\nCar at entry. Waiting for RFID...\n");

    while (digitalRead(IR_ENTRY) == HIGH) {

      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        delay(100);
        continue;
      }

      Serial.print("RFID Tag Detected: ");
      printUID(rfid.uid.uidByte);
      Serial.println();

      bool is1 = uidEquals(rfid.uid.uidByte, Tag1);
      bool is2 = uidEquals(rfid.uid.uidByte, Tag2);
      bool is3 = uidEquals(rfid.uid.uidByte, Tag3);

      // Process each card
      if (is1) {
        processCard(balance1, 1);
      } else if (is2) {
        processCard(balance2, 2);
      } else if (is3) {
        processCard(balance3, 3);
      } else {
        Serial.println("Unknown Card — Access Denied");
        denyAccess();
      }

      // Cleanup and wait for removal
      waitForCardRemoval();
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();

      delay(200);
    }
  }
}

void processCard(int &balance, int cardNumber) {
    if (balance >= tollAmount) {
    balance -= tollAmount; 

    Serial.print("New Balance: Rs ");
    Serial.println(balance);

    openGateAndWaitExit(); 
  }
  else {
    Serial.println("Insufficient Balance — Access Denied");
    denyAccess();
  }
 

}

void denyAccess() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_RED, LOW);
    delay(150);
    digitalWrite(LED_RED, HIGH);
    delay(150);
  }
}

void openGateAndWaitExit() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  gateServo.write(GATE_OPEN);
  Serial.println("Gate Opened...\n");

  // Wait for car to reach exit sensor
  while (digitalRead(IR_EXIT) == LOW) delay(50);

  // Wait for car to clear exit sensor
  while (digitalRead(IR_EXIT) == HIGH) delay(50);

  delay(500); 

  // Close gate
  gateServo.write(GATE_CLOSED);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);

  Serial.println("Gate Closed.");
}
