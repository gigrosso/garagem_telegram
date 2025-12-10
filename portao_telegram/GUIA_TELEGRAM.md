# 🤖 GUIA COMPLETO - Bot Telegram para Controle de Portão

## 📚 BIBLIOTECAS NECESSÁRIAS

Instale estas bibliotecas no Arduino IDE:

### 1. UniversalTelegramBot
```
Sketch → Incluir Biblioteca → Gerenciar Bibliotecas
Busque: "UniversalTelegramBot"
Instale: "Universal Telegram Bot Library" por Brian Lough
```

### 2. ArduinoJson
```
Busque: "ArduinoJson"
Instale: "ArduinoJson" por Benoit Blanchon
⚠️ Use versão 6.x (não a 7.x)
```

### 3. rc-switch (já deve estar instalada)
```
Busque: "rc-switch"
```

## 🤖 PASSO 1: CRIAR BOT NO TELEGRAM

### 1.1 Abrir o BotFather
1. Abra o Telegram no celular ou computador
2. Busque por: **@BotFather**
3. Inicie conversa com ele

### 1.2 Criar novo bot
Digite os comandos:

```
/newbot
```

O BotFather vai perguntar:
- **Nome do bot:** Digite algo como "Portão Casa" ou "Controle Portão"
- **Username do bot:** Deve terminar com "bot", exemplo: `portao_casa_bot` ou `controle_portao_bot`

### 1.3 Obter o Token
Após criar, o BotFather vai te dar um **TOKEN** assim:
```
123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890
```

🔴 **COPIE E GUARDE ESTE TOKEN!** Você vai precisar dele!

### 1.4 Obter seu Chat ID (opcional mas recomendado)

Para descobrir seu Chat ID:

**Opção A - Usar bot auxiliar:**
1. No Telegram, busque: **@userinfobot**
2. Inicie conversa com ele
3. Ele vai mostrar seu ID: `Id: 123456789`

**Opção B - Pelo código:**
1. Configure só o token no código (deixe CHAT_ID vazio)
2. Faça upload
3. Envie `/start` para seu bot
4. Olhe o Serial Monitor - vai mostrar seu Chat ID
5. Copie o Chat ID e coloque no código

## 📝 PASSO 2: CAPTURAR CÓDIGOS RF

### 2.1 Usar o código de captura
Use o código `rf433_clone.ino` original para capturar os códigos:

```
1. Faça upload do rf433_clone.ino
2. Digite: L (modo leitura)
3. Pressione cada botão do controle
4. ANOTE os códigos que aparecerem:
   - Código decimal
   - Bits
   - Protocolo
   - Atraso
```

### Exemplo do que você vai ver:
```
🎯 SINAL CAPTURADO!
═══════════════════════════════
Código: 5393 (0x1511)
Bits: 24
Protocolo: 1
Atraso: 189
═══════════════════════════════
```

**ANOTE TUDO!** Você vai precisar destes valores.

## ⚙️ PASSO 3: CONFIGURAR O CÓDIGO

Abra o arquivo `portao_telegram.ino` e edite as seguintes seções:

### 3.1 Configurar WiFi
```cpp
#define WIFI_SSID "SEU_WIFI_AQUI"           // Nome da sua rede WiFi
#define WIFI_PASSWORD "SUA_SENHA_AQUI"     // Senha do WiFi
```

**Exemplo:**
```cpp
#define WIFI_SSID "MinhaRede5G"
#define WIFI_PASSWORD "senha12345"
```

### 3.2 Configurar Token do Bot
```cpp
#define BOT_TOKEN "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
```

**Exemplo:**
```cpp
#define BOT_TOKEN "123456789:ABCdefGHIjklMNOpqrsTUVwxyz-1234567890"
```

### 3.3 Configurar Chat ID (opcional)
```cpp
#define CHAT_ID "123456789"  // Seu Chat ID
```

Se deixar vazio `""`, qualquer pessoa que souber do bot poderá usar.
**Recomendado:** Configure para aceitar apenas você!

### 3.4 Configurar Códigos RF ⚠️ IMPORTANTE!

Substitua os códigos de exemplo pelos que você capturou:

```cpp
CodigoRF codigos[] = {
  // {código, bits, protocolo, atraso, "nome"}
  {5393, 24, 1, 189, "Abrir"},      // SUBSTITUA pelos seus códigos!
  {5396, 24, 1, 189, "Fechar"},     // SUBSTITUA pelos seus códigos!
  {5412, 24, 1, 189, "Parar"},      // SUBSTITUA pelos seus códigos!
};
```

**Exemplo real (substitua com seus códigos):**
```cpp
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},
  {8234568, 24, 1, 350, "Fechar"},
};
```

Se você tem apenas 2 botões (abrir e fechar), deixe só 2 códigos:
```cpp
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},
  {8234568, 24, 1, 350, "Fechar"},
};
```

## 🔌 PASSO 4: CONEXÕES DO HARDWARE

```
Transmissor FS1000A      NodeMCU V3
──────────────────       ──────────
VCC                  →   5V (ou VU)
GND                  →   GND
DATA                 →   D1 (GPIO5)
```

**💡 Dica:** Adicione antena de 17cm no transmissor para maior alcance!

## 📤 PASSO 5: FAZER UPLOAD

1. Conecte o NodeMCU no USB
2. Selecione a placa: **NodeMCU 1.0**
3. Selecione a porta COM
4. Clique em **Upload**
5. Aguarde finalizar

## 🧪 PASSO 6: TESTAR

### 6.1 Abrir Monitor Serial
```
Ctrl + Shift + M (ou Tools → Serial Monitor)
Configure: 115200 baud
```

### 6.2 Verificar inicialização
Você deve ver:
```
╔════════════════════════════════════╗
║  CONTROLE DE PORTÃO VIA TELEGRAM  ║
║  ESP8266 + RF 433MHz              ║
╚════════════════════════════════════╝

✓ Transmissor RF configurado

📋 Códigos RF carregados:
  [1] Abrir - Código: 5393 (0x1511)
  [2] Fechar - Código: 5396 (0x1514)

📡 Conectando ao WiFi: MinhaRede5G
..........
✓ WiFi conectado!
   IP: 192.168.1.100

🤖 Bot Telegram iniciado!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✅ Sistema pronto! Aguardando comandos...
```

### 6.3 Testar no Telegram

1. Abra o Telegram
2. Busque seu bot pelo username (ex: @portao_casa_bot)
3. Digite: `/start`

Você deve receber:
```
👋 Olá, Seu Nome!

🏠 Controle de Portão RF 433MHz

Comandos disponíveis:
━━━━━━━━━━━━━━━━
/abrir - Abrir portão
/fechar - Fechar portão
/codigos - Listar todos códigos
/status - Ver status do sistema
/ajuda - Ver ajuda detalhada
━━━━━━━━━━━━━━━━

🔐 Sistema pronto para uso!
```

### 6.4 Testar comando

Digite: `/abrir`

Deve aparecer:
```
✅ Comando enviado: Abrir
🔢 Código: 5393
📡 Sinal transmitido com sucesso!
```

E no Serial Monitor:
```
📩 Nova mensagem:
   De: Seu Nome
   Chat ID: 123456789
   Mensagem: /abrir

📡 Transmitindo código RF...
   Código: Abrir (5393)
   Solicitado por: Seu Nome
   ✓ Sinal transmitido!
```

## 🎮 COMANDOS DISPONÍVEIS

### Comandos Básicos:
- `/start` - Menu principal
- `/abrir` - Abrir portão
- `/fechar` - Fechar portão
- `/parar` - Parar movimento (se configurado)

### Comandos Informativos:
- `/status` - Ver status do sistema
- `/codigos` - Listar códigos salvos
- `/ajuda` - Ajuda completa

### Comando Avançado:
- `/enviar 1` - Envia código número 1
- `/enviar 2` - Envia código número 2
- `/enviar 3` - Envia código número 3

## 🔧 TROUBLESHOOTING

### ❌ Bot não responde

**Verificar:**
1. WiFi conectado? (veja Serial Monitor)
2. Token correto?
3. Nome do bot correto no Telegram?
4. Aguarde até 1 minuto (o bot verifica mensagens a cada 1 segundo)

### ❌ "Acesso negado"

- Seu Chat ID está configurado errado
- Solução: Deixe CHAT_ID vazio `""` ou configure o correto

### ❌ Portão não abre

**Verificar:**
1. Códigos RF estão corretos? (compare com os capturados)
2. Transmissor conectado corretamente?
3. Transmissor próximo ao motor do portão?
4. Adicione antena de 17cm
5. Seu controle pode usar rolling code (não funciona)

### ❌ WiFi não conecta

1. SSID e senha corretos?
2. Rede 2.4GHz? (ESP8266 não suporta 5GHz)
3. Roteador próximo?

### ❌ Erro na compilação

**Se aparecer erro de biblioteca:**
```
UniversalTelegramBot.h: No such file
```
→ Instale a biblioteca UniversalTelegramBot

**Se aparecer erro de ArduinoJson:**
```
#error incompatible ArduinoJson version
```
→ Desinstale ArduinoJson 7.x e instale 6.x

## 🔐 SEGURANÇA

### Recomendações:
1. ✅ Configure CHAT_ID para aceitar apenas você
2. ✅ Não compartilhe o TOKEN do bot
3. ✅ Use rede WiFi segura (WPA2)
4. ✅ Mude senha do WiFi periodicamente

### Opcional - Múltiplos usuários:
Se quiser permitir outros usuários, modifique a verificação no código.

## 🚀 MELHORIAS FUTURAS

Possíveis upgrades:
1. **Sensor de estado** - Saber se portão está aberto/fechado
2. **Logs** - Registrar quem abriu e quando
3. **Horários** - Abrir/fechar automaticamente
4. **Notificações** - Avisar quando portão abre
5. **WebServer** - Controle via navegador também
6. **MQTT** - Integração com Home Assistant

## 📞 SUPORTE

Se tiver problemas:
1. Verifique Serial Monitor - mostra tudo que acontece
2. Teste WiFi separadamente
3. Teste RF separadamente
4. Confirme que capturou códigos corretos

---

**Boa sorte com seu projeto! 🎉**

Agora você pode controlar seu portão de qualquer lugar do mundo via Telegram! 🌍
