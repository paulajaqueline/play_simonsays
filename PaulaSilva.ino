int buttonPin[] = {0, 6, 7, 8, 9};  // Pinos de entrada dos botões: 6, 7, 8, e 9
int ledPin[] = {0, 2, 3, 4, 5};  // Pinos dos LEDs: 2, 3, 4 e 5
int buzzerPin = 10;  // Pino do buzzer
int turnCount = 1;  // Contador de rodadas
int buttonState = 0;  // Verificador de estado do botão

const int maxTurns = 60;  // Número máximo de rodadas gerados aleatoriamente
byte randomArray[maxTurns]; // Array para armazenar a sequência aleatória
byte inputArray[maxTurns];  // Array para armazenar a entrada do usuário

boolean playedThisTurn = false;
boolean buttonPressed = false;

const unsigned long timeLimit = 3000;  // Tempo limite de 3 segundos para o jogador responder

void setup()
{
  // Configura os LEDs e botões
  for (int x = 1; x <= 4; x++) {
    pinMode(ledPin[x], OUTPUT);
    digitalWrite(ledPin[x], LOW);
    pinMode(buttonPin[x], INPUT);
  }

  pinMode(buzzerPin, OUTPUT);  // Buzzer como saída
  digitalWrite(buzzerPin, LOW); // Buzzer começa desligado

  // Gera a sequência aleatória inicial para o jogo
  randomArrayGen();
}

void loop()
{
  if (!playedThisTurn) {  // Não pressionou botão nessa rodada
    playedThisTurn = true;

    for (int x = 1; x <= turnCount; x++) {  // Faz a sequência de LEDs para a rodada atual
      LEDFeedback(ledPin[randomArray[x]]);
    }
  }

  for (int z = 1; z <= turnCount; z++) {  // Checa a entrada do jogador
    buttonCheck(z);
    buttonPressed = false;
  }
  turnCount++;  // Vai pra próxima rodada
  delay(500);
}

void randomArrayGen ()  // Função para gerar a sequência aleatória
{
  randomSeed(analogRead(0));  // Usa uma leitura do pino analógico 0 como semente para a sequência
  for (int x = 1; x < maxTurns; x++) {
    randomArray[x] = random(1, 5);
  }
  delay(500);
}

void LEDFeedback(int ledPin2) //criando a "piscada" ;)
{
  digitalWrite(ledPin2, HIGH);
  delay(400);
  digitalWrite(ledPin2, LOW);
  delay(100);
}

void buttonCheck(int inputNumber)  // Função para verificar a entrada do usuário
{
  unsigned long startTime = millis();  // relógio na tela, tempo começa a contar

  while (!buttonPressed) {  // Continua vendo se nenhum botão foi pressionado
    if (millis() - startTime > timeLimit) {  // Se o tempo limite foi ultrapassado
      fail();  // Chama a função de falha e acaba o jogo
      return;
    }

    for (int y = 1; y <= 4; y++) {
      buttonState = digitalRead(buttonPin[y]);
      if (buttonState == HIGH) { // se o botão tiver sido pressionado

        LEDFeedback(ledPin[y]);  // Liga o LED correspondente
        inputArray[inputNumber] = y;

        playedThisTurn = false;
        buttonPressed = true;

        if (inputArray[inputNumber] == randomArray[inputNumber]) {
          // Se a entrada do usuário corresponde à sequência
          return;  // Continua para a próxima rodada
        } else {
          fail();  // Se a entrada estiver incorreta, chama a função de falha
          return;
        }
      }
    }
  }
  delay(200);
}

void fail()  // Função chamada quando o jogador erra a sequência ou o tempo limite é ultrapassado
{
  tone(buzzerPin, 1000, 500);

  for (int x = 1; x <= 4; x++) {
    digitalWrite(ledPin[x], LOW);  // Liga todos os LEDs
  }
  delay(2000);
  turnCount = 0;  // Reinicia o jogo

  for (int x = 1; x <= 4; x++) {
    digitalWrite(ledPin[x], HIGH);  // Desliga todos os LEDs
  }
  delay(1000);
  randomArrayGen();  // Gera uma nova sequência
}
