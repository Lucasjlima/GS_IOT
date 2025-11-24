# ⚖️ BalanceMe IoT Station

**Disruptive Architectures: IoT, IoB & Generative IA**  
Solução IoT aplicada ao tema: _O Futuro do Trabalho._

---

## 📋 Sobre o Projeto

O **BalanceMe IoT Station** é um protótipo de monitoramento para ambientes de trabalho remoto e híbrido. Em um cenário onde as fronteiras entre vida pessoal e profissional se misturam, este dispositivo atua como um _"guardião"_ do bem-estar do colaborador.

A solução integra conceitos de **IoT** (Monitoramento ambiental) e **IoB** (Internet of Behavior - simulando dados fisiológicos) para sugerir pausas e evitar o _burnout_ ou desconforto físico durante a jornada de trabalho.

---

## 🎯 Objetivos

- **Monitoramento Ambiental**: Coletar temperatura e umidade da estação de trabalho.
- **Monitoramento Fisiológico (Simulado)**: Simular a coleta de batimentos cardíacos (via smartwatch fictício) para detectar estresse.
- **Feedback em Tempo Real**: Alertar o usuário via Dashboard e LED físico quando os parâmetros excederem limites saudáveis.

---

## 🛠️ Arquitetura e Tecnologias

A solução foi desenvolvida utilizando uma arquitetura leve e descentralizada, baseada em **MQTT**, eliminando a necessidade de servidores locais complexos.

### Hardware (Simulado no Wokwi)

- **Microcontrolador**: ESP32
- **Sensores**:
  - DHT22: Para leitura de Temperatura e Umidade
  - Potenciômetro: Para simular a variação de batimentos cardíacos (BPM)
- **Atuadores**:
  - LED: Indicador visual de alerta na mesa do usuário

### Software e Protocolos

- **Linguagem**: C++ (Arduino Framework)
- **Protocolo de Comunicação**: MQTT (Message Queuing Telemetry Transport)
- **Broker**: HiveMQ Public Broker (`broker.hivemq.com`)
- **Front-End / Dashboard**: HTML5, JavaScript (Paho MQTT Client) e Chart.js para visualização de dados

---

## ⚙️ Configuração e Execução

### 1. Simulação do Hardware (Wokwi)

Este projeto foi desenhado para rodar no simulador **Wokwi**.

1. Acesse o [Wokwi](https://wokwi.com)
2. Crie um novo projeto para ESP32
3. **Código**: Copie o conteúdo de `sketch.ino` para o editor
4. **Circuito**: Copie o conteúdo de `diagram.json` para a aba de diagrama
5. **Bibliotecas**: Instale as seguintes bibliotecas no gerenciador do Wokwi:
   - `PubSubClient`
   - `DHT sensor library`
   - `ArduinoJson`
6. Inicie a simulação (Play)

### 2. Executando o Dashboard

O painel de controle é uma página web estática que se conecta diretamente ao Broker MQTT via WebSockets.

1. Baixe o arquivo `dashboard.html` deste repositório
2. Abra o arquivo em qualquer navegador moderno (Chrome, Edge, Firefox)
3. Aguarde a conexão (o status mudará para _"Conectado via MQTT"_)
4. Os dados começarão a aparecer em tempo real conforme a simulação no Wokwi roda

---

## 📡 Fluxo de Dados e MQTT

O dispositivo ESP32 publica periodicamente um pacote JSON no tópico MQTT. O Front-end assina este mesmo tópico para atualizar a interface.

- **Tópico**: `balanceme/fiap/data`
- **Broker**: `broker.hivemq.com`
- **Porta TCP (ESP32)**: `1883`
- **Porta WebSocket (Dashboard)**: `8000`

**Exemplo de Payload JSON**:

```json
{
  "temperatura": 24.5,
  "umidade": 60.0,
  "bpm": 85,
  "alerta": "Normal"
}


## 🧪 Testes Realizados

Para validar a solução, siga o roteiro de testes no Wokwi:

### Estado Normal:
- **Ação**: Mantenha o potenciômetro no centro (aprox. 80 BPM) e Temperatura a 24°C
- **Resultado**: Dashboard mostra status "Normal" (Verde). LED apagado

### Simulação de Estresse:
- **Ação**: Gire o potenciômetro para a direita (acima de 120 BPM)
- **Resultado**: Dashboard alerta "Nível de Estresse Alto" (Vermelho). LED acende

### Simulação de Calor Excessivo:
- **Ação**: Clique no sensor DHT22 e aumente a temperatura para 32°C
- **Resultado**: Dashboard alerta "Desconforto Térmico". LED acende

---

## 👥 Integrantes do Grupo

- [Lucas José Lima] – RM: [561160]
- [Rangel Bernardi Jordao] – RM: [560547]
- [Jhonatta Lima Sandes de Oliveira] – RM: [560277]

---

## 🔗 Links

# https://youtu.be/XTzzVgxmQsU

- **Vídeo de Demonstração**: [Insira o link do YouTube aqui]
- **Simulação Wokwi**: [Insira o link do projeto Wokwi aqui, se houver]

> Projeto desenvolvido para a disciplina de **Disruptive Architectures - FIAP**.
```
