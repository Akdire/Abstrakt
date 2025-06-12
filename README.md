#  BabyVekt – Teknologisk støtte for nybakte mødre

Et studentprosjekt i IN1060 ved Universitetet i Oslo, våren 2025.  
Vi utviklet en prototype på en babyvekt basert på Arduino-teknologi, med mål om å gi nybakte mødre bedre støtte og kontroll over barnets vektutvikling – hjemmefra.

---

## Om prosjektet

Dette prosjektet utforsker hvordan velferdsteknologi kan støtte kvinner i barselperioden. Mange nybakte mødre opplever usikkerhet og stress knyttet til barnets ernæring og vektøkning. Målet med BabyVekt er å gi et enkelt, visuelt og lavterskel verktøy for vektovervåking hjemme.

---

##  Teknisk løsning

Prototypen er bygget med:
- Arduino Uno
- HX711-modul (for signalforsterkning)
- 5kg lastcelle
- LED-lys (grønn, rød) for visuell tilbakemelding
- Knapp for å aktivere måling (simulerer "tare"-funksjon)

Vi valgte Arduino fordi det tilbyr fleksibilitet for videre testing og iterasjon i et fysisk miljø.

> NB: Tinkercad støtter ikke HX711, så simuleringen ble kun delvis testet digitalt.

---

##  Funksjonalitet

- **Måling av babyens vekt** via lastcelle
- **Visuell tilbakemelding**:
  - Grønt lys: normal vektøkning
  - Rødt lys: mulig vektnedgang
    
- **Kalibreringsstøtte** via separat kode

---

##  Brukertesting og design

Vi brukte prinsipper fra *design med brukere (DMB)* og involverte nybakte mødre i flere faser:
- Intervju og behovskartlegging
- Idéworkshop og diskusjon om form og funksjon
- Evaluering av lavoppløselige prototyper

Tilbakemeldingene viste behov for:
- Enkelt grensesnitt uten forstyrrende lyd
- Klare signaler (lys fremfor tekst)
- Fleksibel plassering på stellebord

---

##  Filstruktur

```plaintext
Abstrakt/
├── babyvekt.ino          # Arduino-kode for hovedfunksjonalitet
├── README.md             # Du leser den nå :)
