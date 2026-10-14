#include <Arduino.h>

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
  //? available() diz quantos bytes já chegaram e estão esperando leitura.
  if (Serial.available() > 0)
  {
    String texto = Serial.readStringUntil('\n');
    //? trim() remove dom fim do texto os espaços, '\r', '\t', '\n' que alguns terminais enviam.
    texto.trim();

    if (texto.length() == 0) return;
    
    Serial.printf("Seu nome é %s", texto.c_str());
    Serial.printf("Quantidade de caracteres: %u\n\r", texto.length());
    Serial.printf("Primeira letra: %c\n\r", texto.charAt(0));
    Serial.printf("Última letra: %c\n\r", texto[texto.length() - 1]);

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiúsculas: %s\n\r", maiusculo.c_str());

    if(maiusculo == "MARIANA")
    Serial.println("Esse é seu nome correto.");
  }
}
