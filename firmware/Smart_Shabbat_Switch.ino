// --- Relay Pins ---
#define RELAY_SHABBAT 12  // Relay to bypass/disconnect the manual switch
#define RELAY_TOGGLE 11   // Relay to keep device ON/OFF
#define AC_SENSE_PIN 9    // Pin connected to AC sense circuit

#include <EEPROM.h>

// EEPROM layout
static const uint8_t EEPROM_MAGIC_ADDR = 0;
static const uint8_t EEPROM_RELAY_SHABBAT_ADDR = 1;
static const uint8_t EEPROM_RELAY_TOGGLE_ADDR = 2;
static const uint8_t EEPROM_MAGIC_VALUE = 0xA5;

static void saveRelayState() {
  EEPROM.update(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
  EEPROM.update(EEPROM_RELAY_SHABBAT_ADDR, digitalRead(RELAY_SHABBAT));
  EEPROM.update(EEPROM_RELAY_TOGGLE_ADDR, digitalRead(RELAY_TOGGLE));
}

static void loadRelayState() {
  if (EEPROM.read(EEPROM_MAGIC_ADDR) != EEPROM_MAGIC_VALUE) {
    // No saved state yet; keep defaults
    return;
  }
  digitalWrite(RELAY_SHABBAT, EEPROM.read(EEPROM_RELAY_SHABBAT_ADDR));
  digitalWrite(RELAY_TOGGLE, EEPROM.read(EEPROM_RELAY_TOGGLE_ADDR));
}

// --- DEBUG MODE SWITCH ---
// Uncomment the line below to allow sending commands via the standard Serial Monitor (pins 0/1)
// #define DEBUG_SERIAL 

void setup() {
  Serial.begin(9600); // Used for HC-12 communication or debug
#ifdef DEBUG_SERIAL
  Serial.println("System Ready. Send 'shabbat' or 'week'.");
#endif

  // Relay declaration and default mode
  pinMode(RELAY_SHABBAT, OUTPUT);
  pinMode(RELAY_TOGGLE, OUTPUT);
  digitalWrite(RELAY_SHABBAT, LOW);
  digitalWrite(RELAY_TOGGLE, LOW);
  loadRelayState();

  // AC sensor declaration
  pinMode(AC_SENSE_PIN, INPUT);
}

void loop() {
  // --------------------- Command Handler ---------------------
  // Check for available data from the primary Serial port (used by HC-12 or Debug)
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();
    
#ifdef DEBUG_SERIAL
    Serial.print("Received command: ");
    Serial.println(cmd);
#endif

    if (cmd == "shabbat") {
      // Step 1: sense current state
      bool deviceWasOn = digitalRead(AC_SENSE_PIN);
#ifdef DEBUG_SERIAL
      Serial.print("AC_SENSE_PIN state (deviceWasOn): ");
      Serial.println(deviceWasOn ? "HIGH (Device OFF)" : "LOW (Device ON)");
#endif
      
      // Step 2: bypass manual switch
      digitalWrite(RELAY_SHABBAT, HIGH);
#ifdef DEBUG_SERIAL
      Serial.println("RELAY_SHABBAT: HIGH (Bypass ON)");
#endif

      // Step 3: keep the device in its previous state
      if (deviceWasOn) {
        digitalWrite(RELAY_TOGGLE, LOW);  // keep OFF
#ifdef DEBUG_SERIAL
        Serial.println("RELAY_TOGGLE: LOW (Keeps device OFF)");
#endif
      } else {
        digitalWrite(RELAY_TOGGLE, HIGH);   // keep ON
#ifdef DEBUG_SERIAL
        Serial.println("RELAY_TOGGLE: HIGH (Keeps device ON)");
#endif
      }
      saveRelayState();

      // Step 4: Confirm execution to the ESP32 (HC-12 link)
      delay(50);
      Serial.println("ACK");
    }
    else if (cmd == "week") {
      // Restore normal mode
      digitalWrite(RELAY_SHABBAT, LOW);
      digitalWrite(RELAY_TOGGLE, LOW);
      saveRelayState();

#ifdef DEBUG_SERIAL
      Serial.println("RELAY_SHABBAT: LOW (Bypass OFF)");
      Serial.println("RELAY_TOGGLE: LOW (Week Mode Default)");
#endif
      
      // RELAY_TOGGLE stays in last state, device back under switch control
      delay(50);
      Serial.println("ACK"); 
    }
  }
}
