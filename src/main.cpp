#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(char *texto);
int lerProximoNumero(char *&cursor);
void aplicaCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //? readStringUntil espera no máximo esse tempo (ms) pelo '\n'.
  //? O padrão é 1000 ms.

  Serial.println("DIGITE UM DOS SEGUINTES COMANDOS E PRESSIONE ENTER");
 Serial.println("| VERMELHO | VERDE | AZUL | BRILHO | DESLIGA | RGB |");
}

void loop()
{
  leituraNaoBloqueante();
}

void leituraNaoBloqueante()
{
  const int TAMANHO_MAX = 50;
  //? Tamanho do vetor (inclui '/0)
  static char frase[TAMANHO_MAX];
  static int posicao = 0;

  while (Serial.available() > 0)
  {
    char c = Serial.read();
    if (c == '\r')
      continue;
    //? O continue; pula o restante do while naquela volta e passa pra leitura do próximo caractere.
    //? O '\r'é lido e descartado, sem entrar no vetor.

    if (c == '\n')
    {
      frase[posicao] = '\0';
      if (posicao > 0)
        processarFrase(frase);
      posicao = 0;
    }
    else if (posicao < TAMANHO_MAX - 1)
    {
      frase[posicao] = c;
      posicao++;
    }
  }
}

void processarFrase(char *texto)
{
  Serial.printf("RECEBIDO: [%s]\n\r", texto);

  char *comando = texto;
  char *parametro = NULL; //? -> \0
  char *espaco = strchr(texto, ' ');
  if (espaco != NULL) //? Se encontrou o espaço...
  {
    parametro = espaco + 1;
    *espaco = '\0';
  }

  //? strcasecmp = É a comparação dos parâmetros sem considerar as letras maiúsculas ou minúsculas.
  if (strcasecmp(comando, "VERMELHO") == 0)
  {
    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }
  else if (strcasecmp(comando, "VERDE") == 0)
  {
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }
  else if (strcasecmp(comando, "AZUL") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 255;
    brilho = 100;
  }
  else if (strcasecmp(comando, "BRANCO") == 0)
  {
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }
  else if (strcasecmp(comando, "DESLIGA") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 0;
  }
  else if (strcasecmp(comando, "RGB") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Utilize o comando: RGB r g b (0 a 255)");
      return;
    }

    char *cursor = parametro;

    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);

    vermelho = constrain(r, 0, 255);
    //? Limita o valor já convertido para int de 0 à 255.
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }
  else if (strcasecmp(comando, "BRILHO") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Utilize o comando: BRILHO n (0 a 100)");
      return;
    }

    brilho = constrain(atoi(parametro), 0, 100);
  }
  else
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Comando: [%s] não existe.", comando);
    Serial.println(buffer);
    return;
  }
  aplicaCor();
}

void aplicaCor()
{
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;

  rgbLedWrite(PIN_RGB_LED, r, g, b);

  char bufferMensagem[100];
  snprintf(bufferMensagem, sizeof(bufferMensagem), "| COR: R = %3d G = %3d B = %3d | BRILHO: %3d %% | HEX: #%02X%02X%02X |",
           vermelho, verde, azul, brilho, r, g, b);
  Serial.println(bufferMensagem);
}

//? *& = Trabalha com o conteúdo do ponteiro.
int lerProximoNumero(char *&cursor)
{
  if (cursor == NULL)
    return 0;
  int valor = atoi(cursor);
  char *espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL) ? espaco + 1 : NULL;
  return valor;
}

void leituraBloqueante()
{
  //? available() diz quantos bytes já chegaram e estão esperando leitura.
  if (Serial.available() > 0)
  {
    String texto = Serial.readStringUntil('\n');
    //? trim() remove dom fim do texto os espaços, '\r', '\t', '\n' que alguns terminais enviam.
    texto.trim();

    if (texto.length() == 0)
      return;

    Serial.printf("Seu nome é %s", texto.c_str());
    Serial.printf("Quantidade de caracteres: %u\n\r", texto.length());
    Serial.printf("Primeira letra: %c\n\r", texto.charAt(0));
    Serial.printf("Última letra: %c\n\r", texto[texto.length() - 1]);

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiúsculas: %s\n\r", maiusculo.c_str());

    if (maiusculo == "MARIANA")
      Serial.println("Esse é seu nome correto.");
  }
}
