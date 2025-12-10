/*
 * Leitor e Replicador de Controle Remoto RF 433MHz
 * ESP8266 + FS1000A (TX) + MX-RM-5V (RX)
 * 
 * Conexões:
 * Receptor MX-RM-5V:
 *   - VCC -> 5V
 *   - GND -> GND
 *   - DATA -> D2 (GPIO4)
 * 
 * Transmissor FS1000A:
 *   - VCC -> 5V
 *   - GND -> GND
 *   - DATA -> D1 (GPIO5)
 */

#include <RCSwitch.h>

// Pinos
#define PIN_RECEPTOR 4  // D2
#define PIN_TRANSMISSOR 5  // D1
#define PIN_LED_INTERNO LED_BUILTIN

// Objetos RCSwitch
RCSwitch receptor = RCSwitch();
RCSwitch transmissor = RCSwitch();

// Armazenamento de códigos capturados
#define MAX_CODIGOS 10
struct CodigoRF {
  unsigned long valor;
  unsigned int tamanho;
  unsigned int atraso;
  unsigned int protocolo;
  bool valido;
};

CodigoRF codigos[MAX_CODIGOS];
int totalCodigos = 0;
int modo = 0; // 0 = leitura, 1 = replay

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== Leitor/Replicador RF 433MHz ===");
  Serial.println("ESP8266 + FS1000A/MX-RM-5V\n");
  
  pinMode(PIN_LED_INTERNO, OUTPUT);
  digitalWrite(PIN_LED_INTERNO, HIGH); // LED apagado (lógica invertida)
  
  // Configura receptor
  receptor.enableReceive(digitalPinToInterrupt(PIN_RECEPTOR));
  Serial.println("✓ Receptor configurado no pino D2");
  
  // Configura transmissor
  transmissor.enableTransmit(PIN_TRANSMISSOR);
  transmissor.setRepeatTransmit(10); // Repete 10 vezes para maior alcance
  Serial.println("✓ Transmissor configurado no pino D1");
  
  // Limpa array de códigos
  for(int i = 0; i < MAX_CODIGOS; i++) {
    codigos[i].valido = false;
  }
  
  imprimirMenu();
}

void loop() {
  // Verifica comandos via Serial
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    processarComando(cmd);
  }
  
  // Modo leitura - captura sinais
  if (modo == 0 && receptor.available()) {
    capturarSinal();
  }
}

void capturarSinal() {
  // Pisca LED ao capturar
  digitalWrite(PIN_LED_INTERNO, LOW);
  
  unsigned long valor = receptor.getReceivedValue();
  
  if (valor == 0) {
    Serial.println("⚠ Sinal desconhecido recebido");
  } else {
    unsigned int tamanho = receptor.getReceivedBitlength();
    unsigned int atraso = receptor.getReceivedDelay();
    unsigned int protocolo = receptor.getReceivedProtocol();
    
    Serial.println("\n🎯 SINAL CAPTURADO!");
    Serial.println("═══════════════════════════════");
    Serial.print("Código: ");
    Serial.print(valor);
    Serial.print(" (0x");
    Serial.print(valor, HEX);
    Serial.println(")");
    Serial.print("Bits: ");
    Serial.println(tamanho);
    Serial.print("Protocolo: ");
    Serial.println(protocolo);
    Serial.print("Atraso: ");
    Serial.println(atraso);
    Serial.println("═══════════════════════════════\n");
    
    // Salva o código
    if (totalCodigos < MAX_CODIGOS) {
      // Verifica se já existe
      bool existe = false;
      for(int i = 0; i < totalCodigos; i++) {
        if(codigos[i].valor == valor && codigos[i].valido) {
          existe = true;
          Serial.println("ℹ Código já está na memória!");
          break;
        }
      }
      
      if(!existe) {
        codigos[totalCodigos].valor = valor;
        codigos[totalCodigos].tamanho = tamanho;
        codigos[totalCodigos].atraso = atraso;
        codigos[totalCodigos].protocolo = protocolo;
        codigos[totalCodigos].valido = true;
        totalCodigos++;
        Serial.print("✓ Código salvo na posição ");
        Serial.println(totalCodigos);
      }
    } else {
      Serial.println("⚠ Memória cheia! (máx 10 códigos)");
    }
  }
  
  receptor.resetAvailable();
  digitalWrite(PIN_LED_INTERNO, HIGH);
}

void processarComando(char cmd) {
  switch(cmd) {
    case 'l':
    case 'L':
      modo = 0;
      Serial.println("\n📡 MODO LEITURA ativado");
      Serial.println("Pressione qualquer botão do controle remoto...\n");
      break;
      
    case 'm':
    case 'M':
      listarCodigos();
      break;
      
    case 'r':
    case 'R':
      modo = 1;
      Serial.println("\n📤 MODO REPLAY");
      Serial.println("Digite o número do código (1-" + String(totalCodigos) + ") para transmitir:");
      break;
      
    case 'c':
    case 'C':
      limparCodigos();
      break;
      
    case 'h':
    case 'H':
    case '?':
      imprimirMenu();
      break;
      
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
      if(modo == 1) {
        int indice = cmd - '1';
        transmitirCodigo(indice);
        modo = 0; // Volta para modo leitura
      }
      break;
      
    case '0':
      if(modo == 1) {
        transmitirCodigo(9); // Código 10
        modo = 0;
      }
      break;
      
    default:
      // Ignora outros caracteres
      break;
  }
}

void transmitirCodigo(int indice) {
  if(indice >= totalCodigos || !codigos[indice].valido) {
    Serial.println("❌ Código inválido ou não existe!");
    return;
  }
  
  Serial.println("\n📡 TRANSMITINDO...");
  Serial.print("Código: ");
  Serial.print(codigos[indice].valor);
  Serial.print(" (0x");
  Serial.print(codigos[indice].valor, HEX);
  Serial.println(")");
  
  // Pisca LED durante transmissão
  for(int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED_INTERNO, LOW);
    delay(100);
    digitalWrite(PIN_LED_INTERNO, HIGH);
    delay(100);
  }
  
  // Transmite o código
  transmissor.setProtocol(codigos[indice].protocolo);
  transmissor.send(codigos[indice].valor, codigos[indice].tamanho);
  
  Serial.println("✓ Sinal transmitido!\n");
  
  imprimirMenu();
}

void listarCodigos() {
  Serial.println("\n📋 CÓDIGOS SALVOS NA MEMÓRIA");
  Serial.println("═══════════════════════════════════════════════");
  
  if(totalCodigos == 0) {
    Serial.println("Nenhum código capturado ainda.");
  } else {
    for(int i = 0; i < totalCodigos; i++) {
      if(codigos[i].valido) {
        Serial.print("[");
        Serial.print(i + 1);
        Serial.print("] Código: ");
        Serial.print(codigos[i].valor);
        Serial.print(" (0x");
        Serial.print(codigos[i].valor, HEX);
        Serial.print(") | Bits: ");
        Serial.print(codigos[i].tamanho);
        Serial.print(" | Protocolo: ");
        Serial.println(codigos[i].protocolo);
      }
    }
  }
  Serial.println("═══════════════════════════════════════════════\n");
}

void limparCodigos() {
  for(int i = 0; i < MAX_CODIGOS; i++) {
    codigos[i].valido = false;
  }
  totalCodigos = 0;
  Serial.println("\n🗑 Todos os códigos foram apagados da memória!\n");
}

void imprimirMenu() {
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║           COMANDOS                 ║");
  Serial.println("╠════════════════════════════════════╣");
  Serial.println("║ L - Modo Leitura (capturar)        ║");
  Serial.println("║ M - Mostrar códigos salvos         ║");
  Serial.println("║ R - Replay (replicar código)       ║");
  Serial.println("║ C - Limpar todos os códigos        ║");
  Serial.println("║ H - Mostrar este menu              ║");
  Serial.println("╚════════════════════════════════════╝");
  Serial.println();
  
  if(modo == 0) {
    Serial.println("🔵 Modo atual: LEITURA");
  } else {
    Serial.println("🟢 Modo atual: REPLAY");
  }
  Serial.println();
}
