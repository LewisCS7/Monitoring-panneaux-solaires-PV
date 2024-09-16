// Inclusion des bibliothèques nécessaires
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <BH1750.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#define RX 2
#define TX 3

// Définition des broches pour les capteurs
#define currentPin A0 // Broche pour le capteur de courant ACS712
#define voltagePin A6 // Broche pour le capteur de tension B25
#define DHTPIN A3 // Broche pour le capteur de température DHT22

// Type de capteur DHT22 utilisé
#define DHTTYPE DHT22

// Création des objets pour les capteurs
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

SoftwareSerial espseriale(RX,TX);

// Constantes pour le diviseur de tension
const float R1 = 30000.0;
const float R2 = 7500.0;

// Constante pour la surface du panneau solaire (en m²)
//const float S = 0.05724;
const float S = 0.084;

// Fonction d'initialisation, exécutée une fois au démarrage
void setup() {
    // Initialisation de la communication série
    espseriale.begin(9600);
    Serial.begin(9600);
    //Serial.println("\n");

    // Initialisation de la communication I2C
    Wire.begin();

    // Initialisation des capteurs DHT22 et BH1750
    //dht.begin();
    //lightMeter.begin();

    // Configuration des broches des capteurs en entrée
    pinMode(currentPin, INPUT);
    pinMode(voltagePin, INPUT);

    // Message indiquant le début du test
    //Serial.println(F("Début du test (Veuillez appuyer sur ENTRER...)"));
}

// Fonction en boucle, exécutée en continu après l'initialisation
void loop() {
    /*// Mesure du courant à partir du capteur ACS712
    //unsigned int x=0;
    float adcValueCurrent=0.0, SamplesA=0.0, AvgAcs=0.0, I=0.0;
    for (int x = 0; x < 150; x++) { // Collecte de 150 échantillons
        adcValueCurrent = analogRead(currentPin); // Lecture des valeurs actuelles du capteur
        SamplesA = SamplesA + adcValueCurrent; // Accumulation des échantillons
        delay(3); // Attente avant la prochaine lecture (3ms)
    }
    AvgAcs = SamplesA / 150.0; // Calcul de la moyenne des échantillons
    //I = ((2.5 - (AvgAcs * (5.0 / 1024.0))) / 0.185) - 2.399; // Calcul de l'intensité du courant (en A)
    I = ((2.5 - (AvgAcs * (5.0 / 1024.0))) / 0.100);
    I = abs(I); // Conversion en valeur absolue pour éviter les valeurs négatives
    espseriale.print(I); // Affichage de I en A
    espseriale.print("/ ");*/

    // Mesure du courant à partir du capteur ACS712 20A
    float adcValueCurrent = analogRead(currentPin);
    //float I = ((2.5 - (adcValueCurrent * (5.0 / 1024.0))) / 0.185) - 2.399;
    float I1 = (2.5 - (adcValueCurrent * (5.0 / 1024.0))) / 0.100;
    float I = abs(I1); // Conversion en valeur absolue pour éviter les valeurs négatives
    Serial.println(I);
    espseriale.print(I);
    espseriale.print("/ ");

    // Mesure de la tension à partir du capteur B25
    float adcValueVoltage = analogRead(voltagePin);
    float adc_voltage = (adcValueVoltage * 5) / 1024.0; // Calcul de la tension (en V)
    float U = adc_voltage / (R2/(R1+R2)) ;
    Serial.println(U);
    espseriale.print(U);
    espseriale.print("/ ");

    // Mesure de la température à partir du capteur DHT22
    float T = dht.readTemperature(); // Lecture de la température (en °C)
    Serial.println(T);
    espseriale.print(T); // Affichage de T en °C 
    espseriale.print("/ ");

    // Mesure de l'éclairement solaire à partir du capteur BH1750
    float lux = lightMeter.readLightLevel(); // Lecture de l'intensité lumineuse (en lux)
    //float E = (lux*0.0079); 
    float E = (lux/120); // Conversion de lux en W/m²
    Serial.println(E);
    espseriale.print(E); // Affichage de E en W.m-²
    espseriale.print("/ ");

    // Calcul de la puissance délivrée par le panneau solaire (en W)
    float P = I * U;
    Serial.println(P);
    espseriale.print(P); // Affichage de P en W
    espseriale.print("/ ");

    // Calcul du rendement du panneau solaire (%)
    float R = (P / (E * S)) * 100.0;
    Serial.println(R);
    Serial.println(" ");
    espseriale.print(R); // Affichage de R en %
    espseriale.println("/ ");
    espseriale.println(" ");
            
    // Attente de 5 secondes avant de répéter la mesure
    delay(5000);
}

