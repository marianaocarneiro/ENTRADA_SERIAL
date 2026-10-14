#include <Arduino.h>

void leituraBloqueante();
void leituraNaoBloqueante();
void processarFrase(const char* texto);

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //? readStringUntil espera no máximo esse tempo (ms) pelo '\n'.
  //? O padrão é 1000 ms.

  Serial.println("Digite o seu nome e pressione ENTER: ");
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
      if (posicao > 0) processarFrase(frase);
      posicao = 0;
    }
    else if (posicao < TAMANHO_MAX - 1)
    {
      frase[posicao] = c;
      posicao++;
    }
  }
}

void processarFrase(const char* texto)
{
  Serial.printf("Recebido: [%s]\n\r", texto);

  if (strcmp(texto, "Oi") == 0)
    Serial.print("Oi, tudo bem?");
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
