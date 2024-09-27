/*

Benötigte Bibliotheken

Du benötigst die OneWire-Bibliothek, um mit den DS18B20-Sensoren zu kommunizieren.
Aufbau

    Verbinde die Datenleitung der DS18B20-Sensoren mit einem digitalen Pin deines Mikrocontrollers (z.B. Pin 2).
    Verwende einen 4,7 kOhm Pull-up-Widerstand zwischen der Datenleitung und VCC.


Verwendung

    Hardware-Verbindung: Verbinde die DS18B20-Sensoren entsprechend mit deinem Mikrocontroller. Stelle sicher, dass die Datenleitung korrekt angeschlossen ist und der Pull-up-Widerstand vorhanden ist.
    Bibliothek installieren: Stelle sicher, dass die OneWire-Bibliothek in der Arduino-IDE installiert ist.
    Programm hochladen: Lade das oben stehende Programm auf deinen Mikrocontroller hoch.
    Seriellen Monitor öffnen: Öffne den seriellen Monitor in der Arduino-IDE, setze die Baudrate auf 9600. Der Monitor wird die Adressen (Device IDs) der angeschlossenen DS18B20-Sensoren ausgeben.


Erklärung des Codes

    oneWire.search(addr): Diese Funktion sucht nach einem angeschlossenen 1-Wire-Gerät und speichert dessen Adresse in addr.
    addr[0] == 0x28: Diese Bedingung prüft, ob das gefundene Gerät ein DS18B20-Sensor ist (alle DS18B20 haben das Familiencode-Byte 0x28).
    oneWire.reset_search(): Setzt die Suchfunktion zurück, sodass beim nächsten Durchlauf von vorne nach Geräten gesucht wird.

Unterstützte Mikrocontroller und Boards

    ATmega328 und ATmega328P: Dies sind die Mikrocontroller, die z.B. auf einem Arduino Uno oder Nano verwendet werden.
    ATmega2560: Verwendet z.B. auf einem Arduino Mega 2560.
    Arduino UNO R4 WiFi: Ein moderneres Board, das ebenfalls mit dem Code kompatibel ist.

Dieser Code funktioniert auf allen genannten Plattformen und gibt die Adressen der angeschlossenen DS18B20-Sensoren im seriellen Monitor aus.

*/






#include <OneWire.h>

// Datenleitung der DS18B20-Sensoren ist an Pin 2 angeschlossen
#define ONE_WIRE_BUS 2

// Setup der OneWire-Bibliothek
OneWire oneWire(ONE_WIRE_BUS);

void setup(void) {
  // Startet die serielle Kommunikation
  Serial.begin(9600);
  while (!Serial); // Warten, bis die serielle Verbindung hergestellt ist (optional für UNO R4 WiFi)
  
  Serial.println("Starte DS18B20 Adressen-Scanner...");
}

void loop(void) {
  byte addr[8];
  
  // Sucht nach neuen 1-Wire-Geräten
  if (!oneWire.search(addr)) {
    Serial.println("Keine weiteren Geräte gefunden.");
    oneWire.reset_search();
    delay(250); // Wartezeit, bevor erneut gesucht wird
    return;
  }
  
  Serial.print("Gefunden: ");
  for (int i = 0; i < 8; i++) {
    // Gibt jede Adresse in Hexadezimalformat aus
    if (addr[i] < 16) Serial.print("0");
    Serial.print(addr[i], HEX);
  }
  Serial.println();

  // Überprüfen, ob es sich um einen DS18B20 handelt
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC ist nicht gültig!");
    return;
  }

  if (addr[0] == 0x28) {
    Serial.println(" -> DS18B20 Sensor erkannt.");
  }
}

