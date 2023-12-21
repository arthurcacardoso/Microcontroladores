// Genius 
// Grupo 8
// Arthur e Scarlet

#define STARTBUTTON 2
//Botao Start (interrupt) pino 2 
#define NOTE_D4 294
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_A5 880
#define NOTE_C4 523
#define NOTE_E4 659
#define NOTE_F4S 739
#define NOTE_D4S 622
#define NOTE_C4S 554


int pinosLeds[4] = {8,9,10,11};
// LED Verde pino 8
// LED Amarelo pino 9
// LED Azul pino 10
// LED Vermelho pino 11
int buttonPins[4] = {3,4,5,6};
// Botao LED Verde pino 3
// Botao LED Amarelo pino 4
// Botao LED Azul pino 5
// Botao LED Vermelho pino 6
int botaoPress = 0; // Verifica se o botão foi pressionado
int LED_VERDE, LED_AMARELO, LED_AZUL, LED_VERMELHO;
int buttonPin_VERDE, buttonPin_AMARELO, buttonPin_AZUL, buttonPin_VERMELHO;
int pinoBuzzer = 12; // Buzzer pino 12

bool perdeu_o_jogo = false;
int sequencia[10] = {}; // Sequencia de 10 níveis
int rodadaAtual = 0;
int passoAtual = 0;
int start = 0;

void setup() {
  pinMode(STARTBUTTON, INPUT_PULLUP); // Configura o pino do botão como entrada com pull-up interno

  randomSeed(analogRead(0));

  for(int i = 0; i<=3; i++)
  {
    pinMode(pinosLeds[i],OUTPUT);
  }

  LED_VERDE = pinosLeds[0];
  LED_AMARELO = pinosLeds[1];
  LED_AZUL = pinosLeds[2];
  LED_VERMELHO = pinosLeds[3];

  for(int i = 0; i<=3; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  pinMode(pinoBuzzer, OUTPUT);

  // Configura a interrupção para o pino do botão, para acionar na borda de descida
  attachInterrupt(digitalPinToInterrupt(STARTBUTTON), startGame, FALLING);

  buttonPin_VERDE = buttonPins[0];
  buttonPin_AMARELO = buttonPins[1];
  buttonPin_AZUL = buttonPins[2];
  buttonPin_VERMELHO = buttonPins[3];

  // Inicializa a comunicacao serial
  Serial.begin(9600);
  
}

void loop() {

  // Aguarda até o momento em que haja a interrupção ao pressionar o botão do Start e mude a variavel start de 0 para 1
  while(start == 1) {

  proximaRodada();

  reproduzirSequencia();

  aguardarJogador();

  delay(1000);
  }

}

void startGame(){
  int sequencia[10] = {};
  rodadaAtual = 0;
  passoAtual = 0;
  perdeu_o_jogo = false;
  start = 1;
  somInicio();
  Serial.println("Inicio de jogo\n");  
}

void proximaRodada(){

  if(rodadaAtual > 0)
  {
    Serial.println("\nAcertou! Proximo nivel\n");
    somAcerto();
  }
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_AZUL, HIGH);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(200);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_AZUL, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    delay(200);
  }
  delay(1000);

  int numeroSorteado = random(0,4);
  sequencia[rodadaAtual++] = numeroSorteado;

}

void reproduzirSequencia(){
  
  somNivel();
  Serial.print("Nivel ");
  Serial.print(rodadaAtual);
  Serial.print(": ");
  for(int i = 0; i < rodadaAtual; i++)
  {
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    
        if(sequencia[i] == 0)
        {
          Serial.print("Verde - ");
        }
        else if(sequencia[i] == 1)
        {
          Serial.print("Amarelo - ");
        }
        else if(sequencia[i] == 2)
        {
          Serial.print("Azul - ");
        }
        else if(sequencia[i] == 3)
        {
          Serial.print("Vermelho - ");
        }

    delay(500);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
    
  }
  Serial.println("");

}

void aguardarJogador() {

while(passoAtual < rodadaAtual){

  for(int i = 0; i < rodadaAtual; i++)
  {

    bool jogadaEfetuada = false;

    while(!jogadaEfetuada)
    {
      for(int i = 0; i <=3 ; i++)
      {
        
        if(!digitalRead(buttonPins[i]))
        {
          botaoPress = i;

          if(botaoPress == 0)
          {
            Serial.println("Verde pressionado");
          }
          else if(botaoPress == 1)
          {
            Serial.println("Amarelo pressionado");
          }
          else if(botaoPress == 2)
          {
            Serial.println("Azul pressionado");
          }
          else if(botaoPress == 3)
          {
            Serial.println("Vermelho pressionado");
          }

          digitalWrite(pinosLeds[i], HIGH);
          delay(300);
          digitalWrite(pinosLeds[i], LOW);

          jogadaEfetuada = true;

        }

      }

    }
    delay(10);
    

    if(sequencia[passoAtual] != botaoPress)
    {
      gameOver();
    }
    else if(sequencia[passoAtual] == botaoPress)
    {
      passoAtual++;
      
      if(passoAtual == 10 && rodadaAtual == 10)
      {
      	ganhou();
      }
    }


    if(perdeu_o_jogo = true)
    {
      break;
    }
  }
}
  
  passoAtual = 0;

}

void gameOver() {

  somErro();
  ledFinal();
  perdeu_o_jogo = true;
  Serial.println("Errou!! Aperte Start e tente novamente!");
  start = 0;
  passoAtual = 0;
  rodadaAtual = 0;

}

void ganhou()
{  
  somInicio();
  Serial.println("\nParabens! Voce ganhou!!!\n");
  ledFinal();
  perdeu_o_jogo = true;
  start = 0;
  passoAtual = 0;
  rodadaAtual = 0;
}

void somInicio()
{
  for(int i = 0; i < 3; i++)
  {
    tone(pinoBuzzer, NOTE_C4, 2000);

    tone(pinoBuzzer, NOTE_E4, 2000);

    tone(pinoBuzzer, NOTE_G4, 2000);

    delay(1000);
  }  
  
}

void somAcerto()
{
  tone(pinoBuzzer, NOTE_D4, 1000);
  tone(pinoBuzzer, NOTE_F4S, 1000);
  tone(pinoBuzzer, NOTE_A4, 1000);
}

void somErro()
{
  tone(pinoBuzzer, NOTE_C4, 3000);
  tone(pinoBuzzer, NOTE_D4S, 3000);
  tone(pinoBuzzer, NOTE_F4S, 3000);
}

void somNivel()
{
  tone(pinoBuzzer, NOTE_A4, 500);
  tone(pinoBuzzer, NOTE_C4S, 500);
  tone(pinoBuzzer, NOTE_E4, 500);
}

void ledFinal()
{
  for(int w = 0; w < 4; w++)
  {
    for(int i = 0; i < 4 ; i++)
    {
      digitalWrite(pinosLeds[i], HIGH);
      delay(100);
      digitalWrite(pinosLeds[i], LOW);
    }
      for(int j = 3; j >= 0; j--)
      {
        digitalWrite(pinosLeds[j], HIGH);
        delay(100);
        digitalWrite(pinosLeds[j], LOW);
      }  
  }
}