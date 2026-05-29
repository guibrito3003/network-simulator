# 🌐 Simulador de Tráfego de Rede

**Disciplina:** Estrutura de Dados Lineares — 2°GQ  
**Professora:** Monique Soares  
**Instituição:** UNICAP  
**Autores:** Arthur Guilherme & Hellyne Andrade  

---

## 📋 Descrição

Simulador de tráfego de rede que gerencia o fluxo de pacotes de dados entre diferentes nós, prioriza mensagens urgentes e mantém um histórico (cache) de endereços acessados recentemente para otimizar a busca.

---

## 🗂️ Estrutura de Arquivos

```
Network_simulator/
├── include/
│   ├── packet.h           → TAD Pacote (tipo base)
│   ├── priority_queue.h   → TAD Fila de Prioridade (Max-Heap)
│   ├── hash_table.h       → TAD Tabela Hash (Cache)
│   ├── linked_list.h      → TAD Lista Duplamente Encadeada (Histórico)
│   └── simulator.h        → Orquestrador (integra os três TADs)
├── src/
│   ├── packet.c
│   ├── priority_queue.c
│   ├── hash_table.c
│   ├── linked_list.c
│   ├── simulator.c
│   └── main.c
├── tests/
│   ├── input.txt          → Arquivo de teste principal
│   └── teste.txt          → Arquivo de teste alternativo
└── Makefile
```

---

## 🏗️ Estruturas de Dados Implementadas

### 1. Fila de Prioridade — Max-Heap Binário
Arquivo: `include/priority_queue.h` / `src/priority_queue.c`

Implementação manual de um heap máximo sobre um array dinâmico.

**Critério de prioridade:**
- Pacotes `PRIO_HIGH` (Voz/Vídeo) sempre saem antes de `PRIO_NORMAL` (Downloads)
- Em caso de empate: menor ID sai primeiro (FIFO justo)

**Complexidade:**
| Operação | Complexidade |
|----------|-------------|
| Inserção (`pq_enqueue`) | O(log n) |
| Remoção (`pq_dequeue`) | O(log n) |
| Consulta do topo (`pq_peek`) | O(1) |

---

### 2. Tabela Hash — Cache de Destinos
Arquivo: `include/hash_table.h` / `src/hash_table.c`

**Função de espalhamento (djb2 adaptada):**
```
hash = 5381
para cada caractere c: hash = hash * 31 + c
índice = hash % 17
```
Tamanho 17 (primo) para minimizar colisões.

**Tratamento de Colisão: Encadeamento Exterior (Separate Chaining)**  
Cada bucket é uma lista ligada simples. Quando dois IPs geram o mesmo índice, o novo nó é inserido no início da lista desse bucket.

**Complexidade:**
| Operação | Complexidade |
|----------|-------------|
| Inserção (`ht_insert`) | O(1) amortizado |
| Busca (`ht_search`) | O(1) amortizado |
| Remoção (`ht_remove`) | O(1) amortizado |

---

### 3. Lista Duplamente Encadeada — Histórico
Arquivo: `include/linked_list.h` / `src/linked_list.c`

Cada nó aponta para o próximo (`next`) e para o anterior (`prev`), permitindo percorrer o histórico nos dois sentidos.

**Complexidade:**
| Operação | Complexidade |
|----------|-------------|
| Inserção no final (`ll_append`) | O(1) |
| Travessia para frente | O(n) |
| Travessia para trás | O(n) |

---

## ⚙️ Como Compilar e Executar

### Compilar
```bash
gcc -Iinclude \
  src/packet.c \
  src/priority_queue.c \
  src/hash_table.c \
  src/linked_list.c \
  src/simulator.c \
  src/main.c \
  -o netsim
```

### Executar com arquivo de teste
```bash
./netsim tests/input.txt
```

### Executar em modo interativo
```bash
./netsim
```

---

## 📝 Comandos Disponíveis

| Comando | Formato | Descrição |
|---------|---------|-----------|
| `ADD` | `ADD;ID;IP;PRIORIDADE;CONTEUDO` | Insere pacote na fila |
| `PROCESS` | `PROCESS` | Processa o pacote de maior prioridade |
| `SHOW_CACHE` | `SHOW_CACHE` | Exibe o estado da tabela hash |
| `SHOW_HISTORY` | `SHOW_HISTORY` | Exibe o histórico completo |
| `REPORT` | `REPORT` | Exibe relatório final de tráfego |

**Prioridade:** `1` = Alta (Voz/Vídeo) | `0` = Normal (Dados)

---

## 📄 Exemplo de Entrada (`input.txt`)

```
ADD;101;192.168.0.1;0;Download_Arquivo_PDF
ADD;102;10.0.0.5;1;Chamada_Video_Teams
ADD;103;192.168.0.1;1;Streaming_Audio
ADD;104;172.16.254.1;0;Email_Trabalho
PROCESS
PROCESS
ADD;105;10.0.0.5;0;Sincronizacao_Nuvem
SHOW_CACHE
PROCESS
SHOW_HISTORY
```

---

## 💻 Exemplo de Saída

```
[SISTEMA INICIALIZADO - Tabela Hash de tamanho 17]

>> Lendo comando: ADD (Pacote 101 - Prioridade: Normal)
Status: Pacote 101 inserido na fila.

>> Lendo comando: PROCESS
Executando: Pacote 102 (Chamada_Video_Teams) - Prioridade ALTA.
Cache: IP 10.0.0.5 nao encontrado. Mapeando novo destino na Hash...
Historico: Pacote 102 adicionado a Lista de Eventos.

>> Lendo comando: SHOW_HISTORY
--- HISTORICO DE PROCESSAMENTO (LISTA ENCADEADA) ---
[INICIO] -> [ID: 102 | IP: 10.0.0.5] -> [ID: 103 | IP: 192.168.0.1] -> [FIM]
```

---

## 🔄 Fluxo de Integração (PROCESS)

```
1. pq_dequeue()  → Remove o pacote de maior prioridade do heap
2. ht_search()   → Consulta o IP destino no cache
     ├── HIT  → destino conhecido → acesso rápido
     └── MISS → aprende a rota → ht_insert()
3. ll_append()   → Registra o pacote no histórico
```

---

## 🧠 Escolhas Algorítmicas

- **Heap vs Lista Ordenada:** O heap garante O(log n) para inserção e remoção, enquanto uma lista ordenada custaria O(n) para inserção. Para filas de prioridade, o heap é a escolha padrão.

- **Encadeamento vs Endereçamento Aberto:** O encadeamento exterior é mais robusto quando o fator de carga pode variar, pois não causa degradação em cascata.

- **Tamanho primo (17) na hash:** Tamanhos primos reduzem o agrupamento de colisões na função modular.

- **Lista dupla vs simples:** A lista dupla permite travessia bidirecional, útil para exibir o histórico do mais recente para o mais antigo.
