#include "HX711.h"                      // Inkluderer bibliotek for HX711-veiemodul
HX711 loadcell;                        // Lager et HX711-objekt kalt loadcell

// === 1. Koblingspinner for HX711 ===
const int LOADCELL_DOUT_PIN = 2;       // DOUT fra HX711 til Arduino D2
const int LOADCELL_SCK_PIN  = 3;       // SCK fra HX711 til Arduino D3

// === 2. Kalibreringsinnstillinger ===
const long LOADCELL_OFFSET  = 50682624; // Måleoffset for å nullstille skalaen
const long LOADCELL_DIVIDER = 5895655;  // Divider/verdi for å konvertere rådata til kg

// === 3. Tilstands­maskin og terskelverdier ===
float lowestPlausibleBabyWeightThreshold = 0.03; // Minste vekt (kg) som anses gyldig
int   programState   = 0;            // Nåværende programtilstand
const int STATE_INIT        = 0;     // Initiering (ikke i bruk)
const int STATE_STANDBY     = 1;     // Venter på knappetrykk
const int STATE_TARE        = 2;     // Nullstiller vekten
const int STATE_WEIGHING    = 3;     // Måler vekt og sammenligner
const int STATE_STABILIZED  = 4;     // Viser resultat før reset

// === 4. Brukergrensesnitt-pinner ===
const int buttonPin = 10;             // Knapp for tare/veiestart

// === 5. LED-pinner for poeng­display ===
const int led1Pin = 4;
const int led2Pin = 5;
const int led3Pin = 6;
const int led4Pin = 7;
const int led5Pin = 8;
const int led6Pin = 9;

// === 6. Variabler for vekt- og poeng­logikk ===
float currentWeight       = 0;        // Holder siste målte vekt
float previousStoredWeight = 0;       // Tidligere vekt
int   currentScore        = 0;        // Akkumulert poengsum

// === 7. Funksjon: endre poengsum ===
void changeScore(int change) {
  currentScore += change;             // Legg til (+1 eller –1)
  if (currentScore == 0) {            // Unngå null (ingen historie)
    currentScore += change;
  }
  // Begrens poeng innenfor intervallet –3…+3
  if (currentScore > 3)  currentScore = 3;
  if (currentScore < -3) currentScore = -3;
}

// === 8. Funksjon: oppdatere LED-display ===
void displayScore(int score) {
  // Slå av alle lys før oppdatering
  digitalWrite(led1Pin, LOW);
  digitalWrite(led2Pin, LOW);
  digitalWrite(led3Pin, LOW);
  digitalWrite(led4Pin, LOW);
  digitalWrite(led5Pin, LOW);
  digitalWrite(led6Pin, LOW);
  if (score == 0) return;             // Ingen poeng å vise

  // Tenn lys for negative poeng (venstre)
  if (score <= -3) digitalWrite(led1Pin, HIGH);
  if (score <= -2) digitalWrite(led2Pin, HIGH);
  if (score <= -1) digitalWrite(led3Pin, HIGH);
  // Tenn lys for positive poeng (høyre)
  if (score >=  1) digitalWrite(led4Pin, HIGH);
  if (score >=  2) digitalWrite(led5Pin, HIGH);
  if (score >=  3) digitalWrite(led6Pin, HIGH);
}

void setup() {
  // === 9. Oppsett av pinner ===
  pinMode(buttonPin, INPUT);          // Knapp som digital inngang
  pinMode(led1Pin, OUTPUT);           // LED-utganger
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
  pinMode(led5Pin, OUTPUT);
  pinMode(led6Pin, OUTPUT);

  // === 10. Initialiser HX711-modulen ===
  loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // Definerer data- og klokke­pinner
  loadcell.set_scale(LOADCELL_DIVIDER);                // Setter skaleringsfaktor for kg
  loadcell.set_offset(LOADCELL_OFFSET);                // Nullstiller målehjemmel
  Serial.begin(9600);                                  // Starter seriell kommunikasjon

  // === 11. Starttilstand ===
  programState = STATE_STANDBY;
  displayScore(0);                                      // Slår av alle LED
}

void loop() {
  Serial.println(programState);  // Debug: viser hvilken tilstand vi er i

  // === 12. Tilstandsmaskinen ===
  if (programState == STATE_STANDBY) {
    // Venter på at brukeren trykker knappen
    if (digitalRead(buttonPin) == HIGH) {
      programState = STATE_TARE;
    }
  }
  else if (programState == STATE_TARE) {
    // Indiker tare-modus ved at alle lys tennes
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, HIGH);
    digitalWrite(led4Pin, HIGH);
    digitalWrite(led5Pin, HIGH);
    digitalWrite(led6Pin, HIGH);
    delay(1000);                 // Vent 1 s før nullstilling
    loadcell.tare();             // Nullstill vekten
    programState = STATE_WEIGHING;
    displayScore(currentScore);  // Vis gjeldende poeng
  }
  else if (programState == STATE_WEIGHING) {
    // Gjør to målinger for å sjekke stabilitet
    currentWeight = loadcell.get_units(10);
    Serial.print("Weight: ");
    Serial.println(currentWeight);
    if (currentWeight > lowestPlausibleBabyWeightThreshold) {
      float againCurrentWeight = loadcell.get_units(10);
      Serial.print("Again Weight: ");
      Serial.println(againCurrentWeight);
      Serial.println(abs(againCurrentWeight - currentWeight));
      // Hvis vektendringen er mindre enn 0,01 kg, registrer poeng
      if (abs(againCurrentWeight - currentWeight) < 0.01) {
        changeScore(againCurrentWeight >= previousStoredWeight ? 1 : -1);
        previousStoredWeight = againCurrentWeight;
        displayScore(currentScore);
        programState = STATE_STABILIZED;
      }
    }
  }
  else if (programState == STATE_STABILIZED) {
    delay(5000);                   // Vis resultat i 5 s
    programState = STATE_STANDBY;  // Gå tilbake til standby
    displayScore(0);               // Slå av LED før neste runde
  }
}
