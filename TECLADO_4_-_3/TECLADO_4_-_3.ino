#define ONLINE 1
#include <Keypad.h>
#include <LiquidCrystal.h>

#ifdef ONLINE
#include <SPI.h>
#include <UIPEthernet.h>
#include <utility/logging.h>
#include <PubSubClient.h>
#endif

int msg;

const int rs = 9, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xF1, 0x63 };

#ifdef ONLINE
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "ativaAlarme") == 0) {
    msg = atoi (payload);
    if (msg == 1) {
      ativaAlarme();
      msg = 0;
    }
  }
}

EthernetClient ethClient;
PubSubClient client("m14.cloudmqtt.com", 14527, callback, ethClient);

long lastReconnectAttempt = 0;

void updateMQTT() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }
}

boolean reconnect() {
  if (client.connect("arduinoClient", "arduino", "123")) {
    Serial.println("conectado");
    client.subscribe("ativaAlarme");
    client.subscribe("verificaSenha");
  }
  return client.connected();
}
#endif

bool digitandoSenha = false;
String senhaDigitada = "0000";
const char* password = "1577";
const int botaoAtivaAlarme = 2;
bool alarmeAtivado = false;

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(A0, INPUT);
  pinMode (botaoAtivaAlarme, INPUT);
#ifdef ONLINE
  Ethernet.begin(mac);
  delay(1500);
  lastReconnectAttempt = 0;
#endif
}

void ativaAlarme() {
  int estadoBotao = digitalRead (botaoAtivaAlarme) ;
  if (estadoBotao == HIGH || msg == 1) {
    alarmeAtivado = true;
    lcd.clear();
    lcd.print ("PROTEGIDO");
    lcd.setCursor(0, 1);
    lcd.print ("DIGITE A SENHA");
    delay(500);
  }
}
void loop() {

#ifdef ONLINE
  updateMQTT();
#endif
  ativaAlarme();

  if (alarmeAtivado) {

    char key = keypad.getKey();

    if (key) {
      if (digitandoSenha && key != '#' ) {
        aceitarNovoCaractere(key);
      }

      if (key == '*') {
        limpar();
      }

      if (key == '#') {
        validarSenha();
      }
    }
  }
}

void aceitarNovoCaractere(char key) {
  senhaDigitada += key;
  lcd.setCursor(0, 1);
  lcd.print(senhaDigitada);
}

void limpar() {
  lcd.clear();
  lcd.print ("VALIDANDO SENHA");
  senhaDigitada = "";
  digitandoSenha = true;
}

void validarSenha() {
  lcd.clear();
  if (senhaDigitada == password) {
    senhaDigitada = "";
    digitandoSenha = false;
    client.publish("verificaSenha", "1");
    lcd.clear();
    lcd.print ("DESATIVADO");
    alarmeAtivado = false;

    delay (300);
  }

  else {
    lcd.clear();
    lcd.print ("SENHA ERRADA");
    lcd.setCursor(0, 1);
    lcd.print ("DIGITE DE NOVO");
    delay(2000);
    lcd.clear();
    lcd.print ("PROTEGIDO");
    lcd.setCursor(0, 1);
    lcd.print ("DIGITE A SENHA");
    senhaDigitada = "";
  }
}
