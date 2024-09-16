#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
//#include <SoftwareSerial.h>

// Définition des identifiants du réseau WiFi
const char *ssid = "ton_identifiant_wifi";
const char *password = "ton_mot_de_passe";

AsyncWebServer server(80); // Initialisation du serveur web sur le port 80

// Définition des broches pour la communication série
#define RXp2 16
#define TXp2 17

// Variable pour suivre l'état de l'authentification
bool isAuthenticated = false;

void setup(){
    // Initialisation de la communication série
    Serial.begin(115200);
    //Serial.println("\n");

    //Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
    //Serial2.setTimeout(200);

    //----------------------------------------------------SPIFFS
    // Vérification de l'initialisation de SPIFFS (système de fichiers)
    if(!SPIFFS.begin()){
        Serial.println("Erreur SPIFFS...");
        return;
    }
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while(file){
        Serial.print("File: ");
        Serial.println(file.name());
        file.close();
        file = root.openNextFile();
    }

    //------------------------------------------------------WIFI
    // Connexion au réseau WiFi
    WiFi.begin(ssid, password);
        Serial.print("Tentative de connexion...");
        while(WiFi.status() != WL_CONNECTED){
            Serial.print(".");
            delay(100);
        }
    Serial.println("\n");
    Serial.print("Connecté à ");
    Serial.println(ssid);
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());

    // Initialisation de la communication série 2 (Serial2) avec les paramètres spécifiés
    Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

    //----------------------------------------------------SERVER
    // Gestionnaire de requête pour la page d'accueil
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (isAuthenticated) {
        request->send(SPIFFS, "/index.html", "text/html");
        } else {
        request->send(SPIFFS, "/login.html", "text/html");
        }
    });

    // Gestionnaire de requête pour le processus de connexion
    server.on("/login", HTTP_POST, [](AsyncWebServerRequest *request) {
        if(request->hasParam("username", true) && request->hasParam("password", true)) {
        String username = request->getParam("username", true)->value();
        String password = request->getParam("password", true)->value();

        // Vérification des informations d'identification
        if(username.equals("admin") && password.equals("password")) {
            isAuthenticated = true;
            request->send(200, "text/plain", "Success");
            request->redirect("/");
        } else {
            request->send(401, "text/plain", "Unauthorized");
        }
        } else {
        request->send(400, "text/plain", "Bad Request");
        }
    });

    // Gestionnaire de requête pour le fichier JavaScript
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    // Gestionnaire de requête pour le fichier audio d'alerte
    server.on("/Alerte-rouge.mp3", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/Alerte-rouge.mp3", "audio/mpeg");
    });

    // Nouveau point de terminaison pour lire les données des capteurs
    server.on("/lireDonnees", HTTP_GET, [](AsyncWebServerRequest *request) {

        String data = Serial2.readString();

        int index1 = data.indexOf('/'); // Trouver l'indice de la première occurrence de '/'
        String rexu1 = data.substring(0, index1); // Extraire rexu1

        int index2 = data.indexOf('/', index1 + 1);
        String rexu2 = data.substring(index1 + 1, index2);

        int index3 = data.indexOf('/', index2 + 1);
        String rexu3 = data.substring(index2 + 1, index3);

        int index4 = data.indexOf('/', index3 + 1);
        String rexu4 = data.substring(index3 + 1, index4);

        int index5 = data.indexOf('/', index4 + 1);
        String rexu5 = data.substring(index4 + 1, index5);

        int index6 = data.indexOf('/', index5 + 1);
        String rexu6 = data.substring(index5 + 1, index6);

        // Envoyer les deux parties de données séparément dans la réponse HTTP
        // Utilisez '/' comme séparateur
        request->send(200, "text/plain", rexu1 + "/" + rexu2 + "/" + rexu3 + "/" + rexu4 + "/" + rexu5 + "/" + rexu6); 
        
    });

    // Démarrage du serveur web
    server.begin();
    Serial.println("Serveur actif!");
}

void loop() 
{
    // Affiche un message indiquant que le message est reçu
    Serial.print("Données reçues: ");

    // Attend jusqu'à ce qu'il y ait des données disponibles sur Serial2
    while(!Serial2.available());
    // Lit les données disponibles sur Serial2 et les stocke dans une chaîne de caractères appelée 'data'
    String data = Serial2.readString();
    // Affiche le contenu de 'data' sur le port série par défaut
    Serial.println(data);
        
    int index1 = data.indexOf('/');
    String rexu1 = data.substring(0, index1);
        
    int index2 = data.indexOf('/', index1 + 1);
    String rexu2 = data.substring(index1 + 1, index2);

    int index3 = data.indexOf('/', index2 + 1);
    String rexu3 = data.substring(index2 + 1, index3);

    int index4 = data.indexOf('/', index3 + 1);
    String rexu4 = data.substring(index3 + 1, index4);

    int index5 = data.indexOf('/', index4 + 1);
    String rexu5 = data.substring(index4 + 1, index5);

    int index6 = data.indexOf('/', index5 + 1);
    String rexu6 = data.substring(index5 + 1, index6);

    Serial.print("Data extraites: ");
    Serial.println(rexu1 + "/" + rexu2 + "/" + rexu3 + "/" + rexu4 + "/" + rexu5 + "/" + rexu6);
    
    Serial.println("-----*-----*-----*-----*-----*-----*-----*----*-----*-----*-----*-----*-----*-----*----*-----*-----");

    delay(5000);
}