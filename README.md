
# Smart Retro Telefon – Projekt-Dokumentation

## 1. Einführung

Dieses Projekt beschreibt den Umbau eines klassischen analogen Telefons zu einem modernen **Smart Retro Telefon**.
Das Ziel ist es, den originalen Look, die Mechanik und die Haptik zu erhalten, während im Inneren moderne Elektronik
für Bluetooth, WLAN, Sprachsteuerung und Smart-Home-Integration sorgt.

Das Telefon funktioniert **eigenständig**, kann aber durch einen externen Raspberry Pi massiv erweitert werden.

---

## 2. Feature-Set

### Basis-Funktionen (lokal, ohne Netzwerk)
- Bluetooth-Telefonie (HFP)
- Bluetooth-Musik (A2DP)
- Musik hören über internen Lautsprecher ohne Abheben
- Telefonieren über originalen Hörer
- Mechanische Klingel für Anrufe
- Wecker mit Klingel
- Abheben / Auflegen erkennt Anrufe
- Wählscheibe zur Nummern- und Funktionswahl

### Erweiterte Funktionen (WLAN)
- WLAN-Anbindung (ESP32)
- Steuerung von Philips Hue
- Sprachsteuerung (Wake Word + Befehle)
- Zeit- und Szenensteuerung
- OTA-Firmware-Updates

### Externe Erweiterung (Raspberry Pi)
- Home Assistant
- Node-RED / Automationen
- Erweiterte Sprachlogik
- Logging & Statistik
- Smart-Home-Zentrale

---

## 3. Systemarchitektur (Übersicht)

```
Smartphone
   ⇅ Bluetooth
┌───────────────────┐
│       ESP32       │
│  WLAN + Bluetooth │
└───┬─────────┬─────┘
    │         │
 Audio Out   Audio In
    │         │
┌───▼───┐   ┌─▼─────────┐
│Amp(s) │   │ Mikrofone │
└───┬───┘   └───────────┘
    │
┌───▼──────────┐
│ Lautsprecher │
└──────────────┘
    │
┌───▼──────────┐
│ Klingel      │
└──────────────┘

ESP32 ⇄ WLAN ⇄ Raspberry Pi (optional)
```

---

## 4. Zentrale Komponenten

### 4.1 ESP32 – Hauptcontroller

**Funktion**
- Steuerzentrale des Telefons
- Bluetooth & WLAN
- Audio-Verarbeitung
- GPIO-Logik

**Technische Daten**
- Betriebsspannung: 3.3 V
- Stromaufnahme:
  - Idle: ~80 mA
  - WLAN + BT aktiv: bis 300 mA
- CPU: Dual-Core, 240 MHz

---

### 4.2 Audio-System

#### Lautsprecher
- Impedanz: 4 Ω
- Leistung: 3–5 W
- Einsatz:
  - Musik
  - Klingel
  - Wecker

#### Verstärker (MAX98357A)
- Eingang: I2S (digital)
- Versorgung: 5 V
- Ausgangsleistung: bis 3 W @ 4 Ω
- Wirkungsgrad: >85 %

---

### 4.3 Mikrofone

#### Internes Mikrofon (Sprachsteuerung)
- Typ: I2S Mikrofon (INMP441)
- Spannung: 3.3 V
- Strom: ~10 mA
- Sampling: 16 kHz

#### Hörer-Mikrofon (Telefonie)
- Typ: Elektret-Mikrofon + MAX9814
- Spannung: 3–5 V
- Strom: ~3 mA
- Auto-Gain

---

### 4.4 Mechanische Klingel

**Original-Zustand**
- Benötigt Wechselstrom
- Ursprünglich ca. 60–90 V AC

**Neue Ansteuerung**
- Versorgung: 12 V DC
- Ansteuerung über H-Bridge
- Strom: bis 1 A kurzzeitig
- Leistung: max. ~12 W (impulsartig)

---

## 5. Stromversorgung

### Netzteil
- Ausgang: 12 V DC
- Strom: 3 A
- Leistung: 36 W

### Spannungswandler
- 12 V → 5 V (Verstärker)
- 5 V → 3.3 V (ESP32)

**Grundregel**
> Falsche Spannung zerstört Bauteile schneller als zu viel Strom.

---

## 6. Bedienelemente

### Wählscheibe
- Impulswahl
- 1 Puls = Ziffer 1
- 10 Pulse = Ziffer 0
- Auswertung per GPIO-Interrupt

### Hook-Switch
- Mechanischer Schalter
- Erkennung:
  - Aufgelegt
  - Abgehoben

---

## 7. Raspberry Pi (extern)

**Aufgabe**
- Home Assistant
- Philips Hue
- Sprachverarbeitung
- Automationen

**Verbindung**
- WLAN
- HTTP / MQTT

**Strom**
- 5 V / ~2 A
- Nicht im Telefon verbaut

---

## 8. Software-Architektur

### ESP32 (lokal)
- Audio-Routing
- Bluetooth-Handling
- GPIO-Events
- Wecker & Klingel
- Basis-Sprachbefehle

### Raspberry Pi (optional)
- Szenen
- Smart-Home-Logik
- Sprachintelligenz
- Zeitpläne

---

## 9. Entwicklungsumgebung

- Linux Laptop
- Arduino IDE oder PlatformIO
- USB-Verbindung zum ESP32
- Serielles Debugging

---

## 10. Projektphasen

### Phase 1 – Basis
- ESP32
- Lautsprecher
- Bluetooth Musik

### Phase 2 – Retro
- Wählscheibe
- Klingel
- Wecker

### Phase 3 – Smart
- WLAN
- Raspberry Pi
- Sprachsteuerung

---

## 11. Ziel

Ein funktionales, erweiterbares und langlebiges Smart-Objekt,
das Technik und Nostalgie verbindet – ohne Cloud-Zwang und ohne
Verlust des originalen Charakters.
