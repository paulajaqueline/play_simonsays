int buttonPin[] = {2, 3};
int ledPin[] = {8, 9};
int turnCount = 0;
volatile bool buttonClicked = false;
volatile bool gameStarted = false;

const int maxTurns = 60;
byte randomArray[maxTurns];
byte inputArray[maxTurns];

boolean playedThisTurn = false;
volatile int buttonPressed = -1;

const unsigned long timeLimit = 3000;

/*
  Configura os pinos dos LEDs como saída e os pinos dos botões como entrada com pull-up interno.
  Inicializa as interrupções nos pinos dos botões, associando a funções específicas para
  detectar cliques. Gera a sequência aleatória para o jogo durante a inicialização.
*/
void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW);
    pinMode(buttonPin[i], INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(buttonPin[0]), button1Clicked, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin[1]), button2Clicked, FALLING);

  randomArrayGen();
}

/*
  Executa o ciclo principal do jogo. Espera o jogador iniciar o jogo pressionando qualquer botão.
  Após iniciado, exibe a sequência de LEDs e aguarda a entrada do jogador. Valida cada entrada do
  jogador em relação à sequência gerada. Se o jogador errar ou não responder no tempo limite, chama
  a função de falha. Caso contrário, avança para a próxima rodada.
*/
void loop() {
  if (!gameStarted) {
    return;
  }

  if (!playedThisTurn) {
    playedThisTurn = true;
    for (int i = 0; i <= turnCount; i++) {
      LEDFeedback(ledPin[randomArray[i]]);
    }
  }

  for (int i = 0; i <= turnCount; i++) {
    unsigned long startTime = millis();

    while (buttonPressed == -1) {
      if (millis() - startTime > timeLimit) {
        fail();
        return;
      }
    }

    if (buttonPressed != randomArray[i]) {
      fail();
      return;
    }

    LEDFeedback(ledPin[buttonPressed]);
    inputArray[i] = buttonPressed;
    buttonPressed = -1;
  }

  turnCount++;
  playedThisTurn = false;
  delay(500);
}

/*
  Gera uma sequência aleatória de valores representando os LEDs.
  Esses valores são armazenados no vetor `randomArray`, que será utilizado durante o jogo.
*/
void randomArrayGen() {
  randomSeed(analogRead(0));
  for (int i = 0; i < maxTurns; i++) {
    randomArray[i] = random(0, 2);
  }
}

/*
  Pisca um LED específico, fornecendo feedback visual ao jogador.
  A luz do LED acende por 400 ms e apaga por 100 ms.
*/
void LEDFeedback(int ledPin) {
  digitalWrite(ledPin, HIGH);
  delay(400);
  digitalWrite(ledPin, LOW);
  delay(100);
}

/*
  Lida com a falha do jogador ao errar a sequência ou ultrapassar o tempo limite.
  Pisca todos os LEDs duas vezes, reinicia o jogo e espera o jogador pressionar um botão para reiniciar.
*/
void fail() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin[i], HIGH);
  }
  delay(2000);

  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin[i], LOW);
  }

  turnCount = 0;
  playedThisTurn = false;
  gameStarted = false;
  randomArrayGen();
}

/*
  Detecta o clique no botão 1. Se o jogo não foi iniciado, inicializa o jogo.
  Caso contrário, registra o clique como entrada do jogador.
*/
void button1Clicked() {
  if (!gameStarted) {
    gameStarted = true;
  } else {
    buttonPressed = 0;
  }
}

/*
  Detecta o clique no botão 2. Se o jogo não foi iniciado, inicializa o jogo.
  Caso contrário, registra o clique como entrada do jogador.
*/
void button2Clicked() {
  if (!gameStarted) {
    gameStarted = true;
  } else {
    buttonPressed = 1;
  }
}
