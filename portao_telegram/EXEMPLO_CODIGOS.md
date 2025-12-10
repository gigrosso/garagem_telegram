# 📝 EXEMPLO PRÁTICO - Como Adicionar Seus Códigos

## 🎯 CENÁRIO EXEMPLO

Vamos supor que você capturou os seguintes códigos do seu controle:

```
Botão 1 (Abrir):
═══════════════════════════════
Código: 8234567 (0x7DB107)
Bits: 24
Protocolo: 1
Atraso: 350
═══════════════════════════════

Botão 2 (Fechar):
═══════════════════════════════
Código: 8234568 (0x7DB108)
Bits: 24
Protocolo: 1
Atraso: 350
═══════════════════════════════
```

## ✏️ PASSO A PASSO PARA CONFIGURAR

### 1️⃣ Abra o arquivo portao_telegram.ino

### 2️⃣ Localize esta seção no código:

```cpp
// 🔧 CONFIGURE AQUI OS CÓDIGOS DO SEU CONTROLE:
CodigoRF codigos[] = {
  {5393, 24, 1, 189, "Abrir"},      // Exemplo - SUBSTITUA!
  {5396, 24, 1, 189, "Fechar"},     // Exemplo - SUBSTITUA!
  {5412, 24, 1, 189, "Parar"},      // Exemplo - SUBSTITUA!
};
```

### 3️⃣ Substitua pelos seus códigos capturados:

```cpp
// 🔧 CONFIGURE AQUI OS CÓDIGOS DO SEU CONTROLE:
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},   // Botão 1 do seu controle
  {8234568, 24, 1, 350, "Fechar"},  // Botão 2 do seu controle
};
```

**Formato:**
```cpp
{CÓDIGO, BITS, PROTOCOLO, ATRASO, "NOME"}
```

## 📋 MAIS EXEMPLOS

### Exemplo 1: Controle com 2 botões
```cpp
CodigoRF codigos[] = {
  {1234567, 24, 1, 320, "Abrir"},
  {1234568, 24, 1, 320, "Fechar"},
};
```

### Exemplo 2: Controle com 3 botões
```cpp
CodigoRF codigos[] = {
  {9876543, 24, 1, 400, "Abrir"},
  {9876544, 24, 1, 400, "Fechar"},
  {9876545, 24, 1, 400, "Parar"},
};
```

### Exemplo 3: Controle com 4 botões
```cpp
CodigoRF codigos[] = {
  {5551234, 24, 1, 189, "Abrir"},
  {5551235, 24, 1, 189, "Fechar"},
  {5551236, 24, 1, 189, "Parar"},
  {5551237, 24, 1, 189, "Luz"},
};
```

### Exemplo 4: Múltiplos controles/portões
```cpp
CodigoRF codigos[] = {
  // Portão Principal
  {1111111, 24, 1, 300, "Portão Principal - Abrir"},
  {1111112, 24, 1, 300, "Portão Principal - Fechar"},
  
  // Garagem
  {2222221, 24, 1, 350, "Garagem - Abrir"},
  {2222222, 24, 1, 350, "Garagem - Fechar"},
  
  // Portão Lateral
  {3333331, 24, 1, 280, "Lateral - Abrir"},
};
```

## 🔄 DIFERENTES PROTOCOLOS

Se seus códigos têm protocolos diferentes:

```cpp
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},      // Protocolo 1
  {7654321, 32, 2, 450, "Luz"},        // Protocolo 2
  {1234567, 25, 3, 200, "Campainha"},  // Protocolo 3
};
```

## 📝 TEMPLATE PARA COPIAR

Use este template e preencha com seus dados:

```cpp
CodigoRF codigos[] = {
  // Cole aqui os códigos que você capturou
  // Formato: {código, bits, protocolo, atraso, "nome"}
  
  {_______, ___, _, ___, "Abrir"},   // <- Preencha aqui
  {_______, ___, _, ___, "Fechar"},  // <- Preencha aqui
  // Adicione mais se necessário
};
```

## ✅ CHECKLIST DE VERIFICAÇÃO

Antes de fazer upload, confirme:

- [ ] Substituí TODOS os códigos de exemplo pelos meus
- [ ] Os números de BITS estão corretos (geralmente 24)
- [ ] Os números de PROTOCOLO estão corretos (geralmente 1)
- [ ] Os valores de ATRASO estão corretos
- [ ] Os nomes estão entre aspas "Nome"
- [ ] Cada linha termina com vírgula, EXCETO a última
- [ ] Fechei com `};` no final

## ⚠️ ERROS COMUNS

### ❌ ERRADO - Vírgula na última linha:
```cpp
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},
  {8234568, 24, 1, 350, "Fechar"},  // <- Vírgula aqui vai dar erro!
};
```

### ✅ CORRETO - Sem vírgula na última:
```cpp
CodigoRF codigos[] = {
  {8234567, 24, 1, 350, "Abrir"},
  {8234568, 24, 1, 350, "Fechar"}   // <- Sem vírgula
};
```

### ❌ ERRADO - Faltou aspas no nome:
```cpp
{8234567, 24, 1, 350, Abrir}  // <- Faltam aspas
```

### ✅ CORRETO - Com aspas:
```cpp
{8234567, 24, 1, 350, "Abrir"}  // <- Com aspas
```

## 🧪 TESTAR DEPOIS DE CONFIGURAR

1. Faça upload do código
2. Abra Serial Monitor (115200 baud)
3. Você deve ver seus códigos listados:
```
📋 Códigos RF carregados:
  [1] Abrir - Código: 8234567 (0x7DB107)
  [2] Fechar - Código: 8234568 (0x7DB108)
```

4. No Telegram, digite `/codigos`
5. Confirme que aparecem seus códigos corretos

## 💾 SALVAR SEUS CÓDIGOS

**Dica:** Salve seus códigos em um arquivo de texto para não perder!

```
MEUS CÓDIGOS RF - Controle Portão Principal
═══════════════════════════════════════════

Botão 1 (Abrir):
Código: 8234567
Bits: 24
Protocolo: 1
Atraso: 350

Botão 2 (Fechar):
Código: 8234568
Bits: 24
Protocolo: 1
Atraso: 350

Data da captura: 10/12/2024
Controle: Modelo XYZ123
```

---

Agora é só configurar seus códigos e testar! 🚀
