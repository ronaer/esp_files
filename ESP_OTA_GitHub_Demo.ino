/********************************************************************
 * esp8266 modülleri için GitHub üzerinden belirli zaman intervallerinde 
 * güncelleme yapabilmek için demo
 * TR/izmir/agustos/2022/Dr.TRonik YouTube
 * email:bilgi@ronaer.com
 * insta: https://www.instagram.com/dr.tronik2022/
 * YouTube: Dr.TRonik: www.youtube.com/c/DrTRonik
********************************************************************/
 
/********************************************************************
  GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___GLOBALS___
 ********************************************************************/

#include <ESP8266WiFi.h>
#include <FS.h> 
#include <SimpleTimer.h> 

SimpleTimer timer; // timer nesnesi oluşturuldu...

#ifndef STASSID
#define STASSID "Dr.TRonik"
#define STAPSK  "Dr.TRonik"
#endif

// Tüm program boyunca kullanılabilen ve aktif kalan bir CertStore bağlantısı
// A single, global CertStore which can be used by all connections.  Needs to stay live the entire time any of the WiFiClientBearSSLs are present.
#include <CertStoreBearSSL.h>
BearSSL::CertStore certStore;

// GitHub repository ve binary tanımları
/* Set up values for your repository and binary names */
#define GHOTA_USER "ronaer" //GitHub OTA USER
#define GHOTA_REPO "esp82"
#define GHOTA_CURRENT_TAG "0.0.0"
#define GHOTA_BIN_FILE "ESP_OTA_GitHub_Demo.ino.nodemcu.bin"
#define GHOTA_ACCEPT_PRERELEASE 0

#include <ESP_OTA_GitHub.h>
// Güncelleme Kodunu Başlatalım
// Initialise Update Code
ESPOTAGitHub ESPOTAGitHub(&certStore, GHOTA_USER, GHOTA_REPO, GHOTA_CURRENT_TAG, GHOTA_BIN_FILE, GHOTA_ACCEPT_PRERELEASE);

/********************************************************************
  SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___SETUP___
 ********************************************************************/
void setup() {
  // Olası hata ayıklama için seri iletişimi başlatalım.
  // Start serial for debugging (not used by library, just this sketch).
  Serial.begin(115200);

  // SPIFFS'yi başlatalım  ve sertifikaları alalım.
  // Start SPIFFS and retrieve certificates.
  //SPIFFS https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/spiffs.html
  //http://gomuluyazilim.com/esp8266-ile-flash-dosya-sistemi-multi-tasking-ve-tcp-server-kullanimi/
  SPIFFS.begin(); 
  int numCerts = certStore.initCertStore(SPIFFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.print(F("Number of CA certs read: "));
  Serial.println(numCerts);
  if (numCerts == 0) {
    Serial.println(F("No certs found. Did you run certs-from-mozill.py and upload the SPIFFS directory before running?"));
    return; // Can't connect to anything w/o certs!
  }

  //WiFi bağlantısı
  // Connect to WiFi
  Serial.print("Connecting to WiFi... ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.print(" OK");
  }
  Serial.println();

  // Varsa setup kodlarına devam...
  // Your setup code goes here
  
  pinMode(LED_BUILTIN, OUTPUT);

  timer.setInterval((10 * 60000), update_); // her 10 dakikada update_ fonksiyonu çalışsın

}

/********************************************************************
  LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__LOOP__
 ********************************************************************/
void loop () {
  // loop kodları (SPIFFS ile her kod, loop içinde öngörüldüğü gibi çalışmayabilir!)
  // Your loop code goes here

  timer.run();
  led();

}
/********************************************************************
  VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs__VOIDs
********************************************************************/
void update_() {
  // GitHub üzerindeki yeni kodu kontrol ve güncelleme...
  /* This is the actual code to check and upgrade */
  Serial.println("Checking for update...");
  if (ESPOTAGitHub.checkUpgrade()) {
    Serial.print("Upgrade found at: ");
    Serial.println(ESPOTAGitHub.getUpgradeURL());
    if (ESPOTAGitHub.doUpgrade()) {
      Serial.println("Upgrade complete."); //This should never be seen as the device should restart on successful upgrade.
    } else {
      Serial.print("Unable to upgrade: ");
      Serial.println(ESPOTAGitHub.getLastError());
    }
  } else {
    Serial.print("Not proceeding to upgrade: ");
    Serial.println(ESPOTAGitHub.getLastError());
  }
  /* End of check and upgrade code */
  // GitHub kontrol ve güncelleme sonu...
}

void led () {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

/* email:bilgi@ronaer.com
   insta: https://www.instagram.com/dr.tronik2022/
   YouTube: Dr.TRonik: www.youtube.com/c/DrTRonik
*/
