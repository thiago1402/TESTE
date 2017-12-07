int pinVermelho = 10;
int pinAmarelo = 9;
int pinVerde = 8;
int pinBotao = 7;

int pinPedestreVermelho = 3;
int pinPedestreVerde = 2;

int faseSemaforo;

int estadoBotao;

int ultimoEstadoBotao;

int tempopisca = 0;
int estadoPisca = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode (pinVermelho , OUTPUT);
  pinMode (pinAmarelo , OUTPUT);
  pinMode (pinVerde , OUTPUT);
  pinMode (pinBotao , INPUT);
  pinMode (pinPedestreVermelho , OUTPUT);
  pinMode (pinPedestreVerde , OUTPUT);

  ultimoEstadoBotao = digitalRead (pinBotao);
  faseSemaforo = 1;
}

void loop() {

  estadoBotao = digitalRead (pinBotao);

  if (estadoBotao == LOW && estadoBotao < 4 && ultimoEstadoBotao == HIGH ) {
    faseSemaforo = faseSemaforo ++;
  }
  else {
    faseSemaforo = 1;
  }
  ultimoEstadoBotao = estadoBotao;


  if (faseSemaforo == 1) {
    digitalWrite (pinVermelho, LOW);
    digitalWrite (pinAmarelo, LOW);
    digitalWrite (pinVerde, HIGH);

    digitalWrite (pinPedestreVermelho, HIGH);
    digitalWrite (pinPedestreVerde, LOW);
  }
  if (faseSemaforo == 2) {
    digitalWrite (pinVermelho, LOW);
    digitalWrite (pinAmarelo, HIGH);
    digitalWrite (pinVerde, LOW);

    digitalWrite (pinPedestreVermelho, HIGH);
    digitalWrite (pinPedestreVerde, LOW);
  }
  if (faseSemaforo == 3) {
    digitalWrite (pinVermelho, HIGH);
    digitalWrite (pinAmarelo, LOW);
    digitalWrite (pinVerde, LOW);

    digitalWrite (pinPedestreVermelho, LOW);
    digitalWrite (pinPedestreVerde, HIGH);
  }
  if (faseSemaforo == 4) {
    digitalWrite (pinVermelho, HIGH);
    digitalWrite (pinAmarelo, LOW);
    digitalWrite (pinVerde, LOW);

    tempopisca = tempopisca ++;
    if (tempopisca == 400) {
    estadoPisca = !estadoPisca;
    tempopisca = 0;
  }

  digitalWrite (pinPedestreVermelho, LOW);
  digitalWrite (pinPedestreVerde, estadoPisca);
}
delay(1);
}
