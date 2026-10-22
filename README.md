# Sistema de Rastreamento Solar Automatizado

Rastreador solar de 2 eixos (horizontal e vertical) controlado por Arduino, que orienta um painel (ou célula de teste) na direção de maior incidência de luz usando 4 sensores LDR e algoritmo com **histerese adaptativa**.

> Projeto acadêmico – ETEC Professor Adhemar Batista Hemérias (2025)

## Como funciona

O sistema lê continuamente a iluminância em 4 pontos (superior-esquerdo, superior-direito, inferior-esquerdo, inferior-direito) e calcula as médias por região:

| Leitura | Significado |
|---|---|
| `avt` | média superior (`(lt + rt) / 2`) |
| `avd` | média inferior (`(ld + rd) / 2`) |
| `avl` | média esquerda (`(lt + ld) / 2`) |
| `avr` | média direita (`(rt + rd) / 2`) |

A diferença entre regiões opostas (`dvert` e `dhoriz`) indica para onde a luz está mais forte. Se a diferença ultrapassar a **tolerância**, o servo correspondente gira 1° na direção da maior incidência — isso é a **histerese**: o motor só é acionado quando o desvio é significativo, evitando oscilação e desgaste.

A tolerância e o atraso são **adaptativos**: conforme a luminosidade global (`veg`), o sistema ajusta a sensibilidade — mais sensível no crepúsculo, mais estável sob luz intensa.

## Componentes

- Arduino UNO
- 2x Servos (rotação 0–180°)
- 4x Módulos fotorresistores (LDR)
- Estrutura mecânica de 2 eixos

## Ligações

| Componente | Pino Arduino |
|---|---|
| Servo horizontal | 5 (PWM) |
| Servo vertical | 6 (PWM) |
| LDR superior esquerdo | A0 |
| LDR superior direito | A1 |
| LDR inferior esquerdo | A2 |
| LDR inferior direito | A3 |

## Parâmetros de ajuste

| Variável | Padrão | Descrição |
|---|---|---|
| `tolerancia` | adaptativa | Diferença mínima de leitura para acionar o servo (evita tremulação) |
| `tempo_atraso` | adaptativo | Pausa entre leituras, em ms (define a velocidade de resposta) |
| `servo*LimitHigh/Low` | — | Limites de rotação de cada eixo |

A depuração é feita pelo **Monitor Serial** (9600 baud), que exibe as leituras brutas dos LDRs, a média global, a tolerância e o atraso atuais — os dados coletados apoiaram a análise e a monografia.

## Registro de decisões de projeto

- **Histerese em vez de controle proporcional**: para servos hobby, acionar 1° por ciclo com limiar de tolerância é suficiente e muito mais simples de calibrar do que um PID.
- **Tolerância adaptativa**: com luminosidade baixa, o ruído dos LDRs domina a leitura; o `map()` reduz a tolerância justamente quando o sinal é fraco, e aumenta quando há luz plena.
- **Registro de ponto incremental**: os ângulos são ajustados de 1 em 1 grau, sem "saltos", o que suaviza o movimento e protege a estrutura mecânica.
- **Flag de operação**: permite desligar o rastreamento por software sem reiniciar o Arduino.

## Resultados

*(Adicione aqui fotos do protótipo e do monitoramento — veja a pasta `docs/`)*

## Como executar

1. Abra `rastreador_solar/rastreador_solar.ino` na Arduino IDE.
2. Selecione a placa e a porta corretas.
3. Faça o upload e abra o Monitor Serial (9600 baud) para acompanhar as leituras.

## Licença

MIT
