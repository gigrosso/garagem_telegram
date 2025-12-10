/*
 * ============================================
 * CONTROLE DE PORTÃO VIA TELEGRAM BOT
 * ESP8266 + FS1000A (TX 433MHz) + Telegram
 * ============================================
 * 
 * IMPORTANTE: Antes de usar, você precisa:
 * 1. Capturar os códigos RF do seu controle usando o código anterior
 * 2. Criar um bot no Telegram com o @BotFather
 * 3. Configurar WiFi, Token do Bot e Códigos RF abaixo
 * 
 * Conexões:
 * Transmissor FS1000A:
 *   - VCC -> 5V
 *   - GND -> GND
 *   - DATA -> D1 (GPIO5)
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <RCSwitch.h>

// ============================================
// 📝 CONFIGURAÇÕES - EDITE AQUI!
// ============================================

// WiFi
#define WIFI_SSID "SEU_WIFI_AQUI"           // Nome da sua rede WiFi
#define WIFI_PASSWORD "SUA_SENHA_AQUI"     // Senha do WiFi

// Telegram Bot
#define BOT_TOKEN "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // Token do seu bot

// 👥 CHAT IDs AUTORIZADOS (adicione quantos quiser)
// Deixe o array vazio {} para aceitar qualquer pessoa
String chatIDsAutorizados[] = {
  "123456789",    // Usuário 1 (você)
  "987654321",    // Usuário 2 (exemplo: sua esposa)
  "555666777",    // Usuário 3 (exemplo: seu filho)
  // Adicione mais IDs aqui se necessário
};
int totalUsuariosAutorizados = sizeof(chatIDsAutorizados) / sizeof(chatIDsAutorizados[0]);

// Transmissor RF
#define PIN_TRANSMISSOR 5  // D1 no NodeMCU

// ⚠️ CÓDIGOS RF - SUBSTITUA PELOS CÓDIGOS QUE VOCÊ CAPTUROU! ⚠️
// Formato: {código, bits, protocolo, atraso}
struct CodigoRF {
  unsigned long codigo;
  unsigned int bits;
  unsigned int protocolo;
  unsigned int atraso;
  String nome;
};

// 🔧 CONFIGURE AQUI OS CÓDIGOS DO SEU CONTROLE:
CodigoRF codigos[] = {
  {5393, 24, 1, 189, "Abrir"},      // Exemplo - SUBSTITUA!
  {5396, 24, 1, 189, "Fechar"},     // Exemplo - SUBSTITUA!
  {5412, 24, 1, 189, "Parar"},      // Exemplo - SUBSTITUA!
  // Adicione mais códigos se necessário
};

int totalCodigos = sizeof(codigos) / sizeof(codigos[0]);

// ============================================
// OBJETOS E VARIÁVEIS GLOBAIS
// ============================================

RCSwitch transmissor = RCSwitch();
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

unsigned long ultimaVerificacao = 0;
const unsigned long intervaloBot = 1000; // Verifica mensagens a cada 1 segundo

bool wifiConectado = false;

// ============================================
// FUNÇÕES
// ============================================

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n╔════════════════════════════════════╗");
  Serial.println("║  CONTROLE DE PORTÃO VIA TELEGRAM  ║");
  Serial.println("║  ESP8266 + RF 433MHz              ║");
  Serial.println("╚════════════════════════════════════╝\n");
  
  // Configura transmissor RF
  transmissor.enableTransmit(PIN_TRANSMISSOR);
  transmissor.setRepeatTransmit(10);
  Serial.println("✓ Transmissor RF configurado");
  
  // Mostra códigos carregados
  Serial.println("\n📋 Códigos RF carregados:");
  for(int i = 0; i < totalCodigos; i++) {
    Serial.print("  [");
    Serial.print(i + 1);
    Serial.print("] ");
    Serial.print(codigos[i].nome);
    Serial.print(" - Código: ");
    Serial.print(codigos[i].codigo);
    Serial.print(" (0x");
    Serial.print(codigos[i].codigo, HEX);
    Serial.println(")");
  }
  
  // Conecta ao WiFi
  conectarWiFi();
  
  // Configura client SSL (necessário para Telegram)
  client.setInsecure(); // Para ESP8266
  
  Serial.println("\n🤖 Bot Telegram iniciado!");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  Serial.println("✅ Sistema pronto! Aguardando comandos...\n");
}

void loop() {
  // Verifica conexão WiFi
  if (WiFi.status() != WL_CONNECTED) {
    if (wifiConectado) {
      Serial.println("⚠️  WiFi desconectado! Reconectando...");
      wifiConectado = false;
    }
    conectarWiFi();
    return;
  }
  
  // Verifica novas mensagens do Telegram
  if (millis() - ultimaVerificacao > intervaloBot) {
    ultimaVerificacao = millis();
    
    int numNovasMensagens = bot.getUpdates(bot.last_message_received + 1);
    
    while (numNovasMensagens) {
      processarMensagens(numNovasMensagens);
      numNovasMensagens = bot.getUpdates(bot.last_message_received + 1);
    }
  }
}

void conectarWiFi() {
  Serial.print("📡 Conectando ao WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi conectado!");
    Serial.print("   IP: ");
    Serial.println(WiFi.localIP());
    wifiConectado = true;
  } else {
    Serial.println("\n❌ Falha ao conectar WiFi!");
    Serial.println("   Verifique SSID e senha");
    delay(5000);
  }
}

void processarMensagens(int numMensagens) {
  for (int i = 0; i < numMensagens; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    
    Serial.println("\n📩 Nova mensagem:");
    Serial.print("   De: ");
    Serial.println(from_name);
    Serial.print("   Chat ID: ");
    Serial.println(chat_id);
    Serial.print("   Mensagem: ");
    Serial.println(text);
    
    // Verifica autorização
    bool autorizado = false;
    
    // Se o array estiver vazio, aceita todos
    if (totalUsuariosAutorizados == 0) {
      autorizado = true;
      Serial.println("   ℹ️  Modo aberto (aceita qualquer usuário)");
    } else {
      // Verifica se o chat_id está na lista de autorizados
      for (int j = 0; j < totalUsuariosAutorizados; j++) {
        if (chat_id == chatIDsAutorizados[j]) {
          autorizado = true;
          Serial.print("   ✓ Usuário autorizado (ID: ");
          Serial.print(j + 1);
          Serial.println(")");
          break;
        }
      }
    }
    
    if (!autorizado) {
      Serial.println("   ⚠️  Chat ID não autorizado!");
      String mensagem = "❌ *Acesso Negado*\n\n";
      mensagem += "Seu Chat ID: `" + chat_id + "`\n\n";
      mensagem += "Você não está autorizado a usar este bot.\n";
      mensagem += "Entre em contato com o administrador.";
      bot.sendMessage(chat_id, mensagem, "Markdown");
      continue;
    }
    
    // Processa comandos
    if (text == "/start") {
      enviarMenuPrincipal(chat_id, from_name);
    }
    else if (text == "/ajuda" || text == "/help") {
      enviarAjuda(chat_id);
    }
    else if (text == "/status") {
      enviarStatus(chat_id);
    }
    else if (text == "/codigos") {
      listarCodigos(chat_id);
    }
    else if (text.startsWith("/abrir") || text.startsWith("/Abrir")) {
      transmitirCodigo(0, chat_id, from_name); // Código 1 (índice 0)
    }
    else if (text.startsWith("/fechar") || text.startsWith("/Fechar")) {
      transmitirCodigo(1, chat_id, from_name); // Código 2 (índice 1)
    }
    else if (text.startsWith("/parar") || text.startsWith("/Parar")) {
      if (totalCodigos > 2) {
        transmitirCodigo(2, chat_id, from_name); // Código 3 (índice 2)
      } else {
        bot.sendMessage(chat_id, "⚠️ Comando /parar não configurado!", "");
      }
    }
    else if (text.startsWith("/enviar")) {
      // Comando: /enviar N (onde N é o número do código)
      int indice = text.substring(8).toInt() - 1;
      if (indice >= 0 && indice < totalCodigos) {
        transmitirCodigo(indice, chat_id, from_name);
      } else {
        bot.sendMessage(chat_id, "❌ Código inválido! Use /codigos para ver a lista.", "");
      }
    }
    else {
      bot.sendMessage(chat_id, "❓ Comando não reconhecido.\nUse /ajuda para ver os comandos disponíveis.", "");
    }
  }
}

void transmitirCodigo(int indice, String chat_id, String usuario) {
  if (indice < 0 || indice >= totalCodigos) {
    bot.sendMessage(chat_id, "❌ Código inválido!", "");
    return;
  }
  
  Serial.println("\n📡 Transmitindo código RF...");
  Serial.print("   Código: ");
  Serial.print(codigos[indice].nome);
  Serial.print(" (");
  Serial.print(codigos[indice].codigo);
  Serial.println(")");
  Serial.print("   Solicitado por: ");
  Serial.println(usuario);
  
  // Configura protocolo
  transmissor.setProtocol(codigos[indice].protocolo);
  
  // Transmite
  transmissor.send(codigos[indice].codigo, codigos[indice].bits);
  
  Serial.println("   ✓ Sinal transmitido!");
  
  // Responde no Telegram
  String mensagem = "✅ Comando enviado: " + codigos[indice].nome + "\n";
  mensagem += "🔢 Código: " + String(codigos[indice].codigo) + "\n";
  mensagem += "📡 Sinal transmitido com sucesso!";
  
  bot.sendMessage(chat_id, mensagem, "");
}

void enviarMenuPrincipal(String chat_id, String nome) {
  String mensagem = "👋 Olá, " + nome + "!\n\n";
  mensagem += "🏠 *Controle de Portão RF 433MHz*\n\n";
  mensagem += "Comandos disponíveis:\n";
  mensagem += "━━━━━━━━━━━━━━━━\n";
  mensagem += "/abrir - Abrir portão\n";
  mensagem += "/fechar - Fechar portão\n";
  
  if (totalCodigos > 2) {
    mensagem += "/parar - Parar portão\n";
  }
  
  mensagem += "/codigos - Listar todos códigos\n";
  mensagem += "/status - Ver status do sistema\n";
  mensagem += "/ajuda - Ver ajuda detalhada\n";
  mensagem += "━━━━━━━━━━━━━━━━\n\n";
  mensagem += "🔐 Sistema pronto para uso!";
  
  bot.sendMessage(chat_id, mensagem, "Markdown");
}

void enviarAjuda(String chat_id) {
  String mensagem = "📖 *AJUDA - Controle de Portão*\n\n";
  mensagem += "*Comandos Básicos:*\n";
  mensagem += "/start - Menu principal\n";
  mensagem += "/abrir - Abrir o portão\n";
  mensagem += "/fechar - Fechar o portão\n";
  
  if (totalCodigos > 2) {
    mensagem += "/parar - Parar movimento\n";
  }
  
  mensagem += "\n*Comandos Avançados:*\n";
  mensagem += "/codigos - Ver códigos salvos\n";
  mensagem += "/enviar N - Enviar código específico (N = 1 a " + String(totalCodigos) + ")\n";
  mensagem += "/status - Status do sistema\n";
  mensagem += "/ajuda - Esta mensagem\n\n";
  mensagem += "💡 *Dica:* Mantenha o ESP8266 próximo ao motor do portão para melhor alcance!";
  
  bot.sendMessage(chat_id, mensagem, "Markdown");
}

void enviarStatus(String chat_id) {
  String mensagem = "📊 *STATUS DO SISTEMA*\n\n";
  
  // WiFi
  mensagem += "📡 *WiFi*\n";
  mensagem += "SSID: " + String(WIFI_SSID) + "\n";
  mensagem += "IP: " + WiFi.localIP().toString() + "\n";
  mensagem += "Sinal: " + String(WiFi.RSSI()) + " dBm\n\n";
  
  // Sistema
  mensagem += "💾 *Sistema*\n";
  mensagem += "Uptime: " + String(millis() / 1000) + "s\n";
  mensagem += "Heap livre: " + String(ESP.getFreeHeap()) + " bytes\n\n";
  
  // RF
  mensagem += "📻 *RF 433MHz*\n";
  mensagem += "Códigos: " + String(totalCodigos) + " salvos\n";
  mensagem += "Repetições: 10x\n";
  mensagem += "Pino TX: D1 (GPIO5)\n\n";
  
  mensagem += "✅ Sistema funcionando!";
  
  bot.sendMessage(chat_id, mensagem, "Markdown");
}

void listarCodigos(String chat_id) {
  String mensagem = "📋 *CÓDIGOS RF SALVOS*\n\n";
  
  for (int i = 0; i < totalCodigos; i++) {
    mensagem += "*[" + String(i + 1) + "] " + codigos[i].nome + "*\n";
    mensagem += "Código: " + String(codigos[i].codigo);
    mensagem += " (0x" + String(codigos[i].codigo, HEX) + ")\n";
    mensagem += "Bits: " + String(codigos[i].bits) + "\n";
    mensagem += "Protocolo: " + String(codigos[i].protocolo) + "\n";
    
    if (i < totalCodigos - 1) {
      mensagem += "━━━━━━━━━━━━\n";
    }
  }
  
  mensagem += "\n💡 Use /enviar N para transmitir\n";
  mensagem += "(N = número do código)";
  
  bot.sendMessage(chat_id, mensagem, "Markdown");
}
