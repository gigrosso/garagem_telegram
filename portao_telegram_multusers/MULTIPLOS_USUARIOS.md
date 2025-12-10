# 👥 GUIA - Múltiplos Usuários no Bot Telegram

## 🔍 Como descobrir Chat IDs

### Método 1: Bot @userinfobot (Mais Fácil)
```
1. Abra o Telegram
2. Busque: @userinfobot
3. Envie qualquer mensagem
4. Ele responde: "Id: 165519061"
5. Anote este número!
```

### Método 2: Usando seu próprio bot
```
1. Configure o array vazio (aceita todos):
   String chatIDsAutorizados[] = {};

2. Faça upload no ESP8266

3. Cada pessoa envia /start para o bot

4. No Serial Monitor você vê:
   📩 Nova mensagem:
      De: João
      Chat ID: 123456789  ← Anote este!
      
   📩 Nova mensagem:
      De: Maria
      Chat ID: 987654321  ← Anote este!

5. Anote todos os IDs e configure no código
```

### Método 3: API do Telegram (Avançado)
```
1. Cada pessoa envia uma mensagem para o bot

2. Acesse no navegador:
   https://api.telegram.org/botSEU_TOKEN/getUpdates
   
3. Procure por "chat":{"id":123456789

4. Anote os IDs
```

---

## ⚙️ CONFIGURAÇÕES POSSÍVEIS

### 🔓 Configuração 1: Aceitar QUALQUER pessoa
```cpp
String chatIDsAutorizados[] = {};  // Array vazio
int totalUsuariosAutorizados = 0;
```
- ⚠️ Qualquer pessoa pode usar o bot
- Use apenas para testes

### 🔐 Configuração 2: Apenas VOCÊ
```cpp
String chatIDsAutorizados[] = {
  "165519061"    // Seu ID
};
int totalUsuariosAutorizados = sizeof(chatIDsAutorizados) / sizeof(chatIDsAutorizados[0]);
```
- ✅ Apenas você pode usar
- Mais seguro

### 👨‍👩‍👧 Configuração 3: Você + Família
```cpp
String chatIDsAutorizados[] = {
  "165519061",    // Você (Giovanni)
  "123456789",    // Esposa
  "987654321",    // Filho
  "555666777"     // Filha
};
int totalUsuariosAutorizados = sizeof(chatIDsAutorizados) / sizeof(chatIDsAutorizados[0]);
```
- ✅ Família pode usar
- Controle compartilhado

### 🏢 Configuração 4: Você + Funcionários
```cpp
String chatIDsAutorizados[] = {
  "165519061",    // Você (Dono)
  "111222333",    // Funcionário 1
  "444555666",    // Funcionário 2
  "777888999"     // Segurança
};
int totalUsuariosAutorizados = sizeof(chatIDsAutorizados) / sizeof(chatIDsAutorizados[0]);
```

---

## 📝 PASSO A PASSO PARA ADICIONAR USUÁRIOS

### Exemplo prático:

**Situação:** Você quer autorizar você, sua esposa e seu filho.

### Passo 1: Descobrir os Chat IDs

**Método fácil:**
1. Configure array vazio temporariamente:
```cpp
String chatIDsAutorizados[] = {};
```

2. Faça upload

3. Peça para cada pessoa enviar `/start` no bot

4. Olhe o Serial Monitor e anote os IDs:
```
Giovanni: 165519061
Maria:    222333444
Pedro:    555666777
```

### Passo 2: Configurar no código

Localize esta seção (linha ~23):
```cpp
String chatIDsAutorizados[] = {
  "123456789",    // Usuário 1
  "987654321",    // Usuário 2
};
```

Substitua pelos IDs coletados:
```cpp
String chatIDsAutorizados[] = {
  "165519061",    // Giovanni (você)
  "222333444",    // Maria (esposa)
  "555666777"     // Pedro (filho)
};
```

### Passo 3: Fazer upload e testar

1. Salve o código
2. Faça upload no ESP8266
3. Cada pessoa testa enviando `/start`
4. Deve funcionar para todos!

---

## ❓ PERGUNTAS FREQUENTES

### P: Quantos usuários posso adicionar?
**R:** Tecnicamente sem limite, mas recomenda-se até 10 usuários para não sobrecarregar o ESP8266.

### P: Como remover um usuário?
**R:** Basta apagar a linha dele do array e fazer upload novamente:
```cpp
String chatIDsAutorizados[] = {
  "165519061",    // Giovanni
  // "222333444", // Maria - REMOVIDA (comente ou delete)
  "555666777"     // Pedro
};
```

### P: O que acontece se alguém não autorizado tentar usar?
**R:** Ele recebe esta mensagem:
```
❌ Acesso Negado

Seu Chat ID: 999888777

Você não está autorizado a usar este bot.
Entre em contato com o administrador.
```

### P: Como saber qual usuário enviou o comando?
**R:** Olhe o Serial Monitor:
```
📩 Nova mensagem:
   De: Maria
   Chat ID: 222333444
   Mensagem: /abrir
   ✓ Usuário autorizado (ID: 2)

📡 Transmitindo código RF...
   Código: Abrir (172021797)
   Solicitado por: Maria
   ✓ Sinal transmitido!
```

### P: Posso ter usuários com permissões diferentes?
**R:** O código atual não tem isso, mas pode ser implementado. Todos os usuários autorizados têm as mesmas permissões.

---

## 🔒 DICAS DE SEGURANÇA

### ✅ Recomendações:
1. **Sempre configure Chat IDs** - nunca deixe aberto para todos
2. **Anote os IDs** dos usuários em um lugar seguro
3. **Revise periodicamente** quem tem acesso
4. **Remova usuários** que não precisam mais de acesso
5. **Não compartilhe** o username do bot publicamente

### ⚠️ NUNCA:
- Deixe o array vazio em produção
- Compartilhe seu Bot Token publicamente
- Adicione IDs de pessoas que você não conhece
- Use o mesmo bot para múltiplas casas/portões diferentes

---

## 📊 EXEMPLO DE LOG NO SERIAL MONITOR

Quando múltiplos usuários usam o bot:

```
📩 Nova mensagem:
   De: Giovanni
   Chat ID: 165519061
   Mensagem: /status
   ✓ Usuário autorizado (ID: 1)

📩 Nova mensagem:
   De: Maria
   Chat ID: 222333444
   Mensagem: /abrir
   ✓ Usuário autorizado (ID: 2)

📡 Transmitindo código RF...
   Código: Abrir (172021797)
   Solicitado por: Maria
   ✓ Sinal transmitido!

📩 Nova mensagem:
   De: Desconhecido
   Chat ID: 999999999
   Mensagem: /start
   ⚠️  Chat ID não autorizado!
```

---

## 🎯 TEMPLATE PARA COPIAR

```cpp
// 👥 CHAT IDs AUTORIZADOS
String chatIDsAutorizados[] = {
  "165519061",    // Usuário 1: Giovanni (você)
  "_________",    // Usuário 2: Nome
  "_________",    // Usuário 3: Nome
  "_________",    // Usuário 4: Nome
  // Adicione mais se necessário
};
int totalUsuariosAutorizados = sizeof(chatIDsAutorizados) / sizeof(chatIDsAutorizados[0]);
```

---

Agora você pode gerenciar múltiplos usuários facilmente! 🎉
