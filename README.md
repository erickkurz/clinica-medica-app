# 🏥 Clínica Médica App — Qt/C++

Aplicação desktop com **interface gráfica** para gerenciamento de atendimento de clínica médica, desenvolvida como evolução do [sistema de terminal em C++](https://github.com/KrzDev-debug/clinica-medica-cpp).

Projeto desenvolvido durante o curso de **Licenciatura em Computação** no **Instituto Federal Sul-Rio-Grandense — Campus Pelotas**.

---

## 📋 Sobre o Projeto

Esta aplicação é a versão visual do sistema de clínica médica originalmente desenvolvido em terminal puro. Mantendo as mesmas estruturas de dados implementadas manualmente (lista, fila e pilha), o projeto foi reescrito com interface gráfica usando o framework **Qt 6**.

---

## 🖥️ Interface

A janela principal exibe em tempo real:

- **Painel de cadastro** — campos de código, nome e idade com botões de Cadastrar, Alterar, Remover e Buscar
- **Controles de fila** — checkbox de prioridade, botões para inserir na fila, chamar próximo, ver último atendimento e desfazer
- **Tabela de pacientes cadastrados**
- **Fila prioritária e fila normal** lado a lado
- **Histórico de atendimentos**
- **Último paciente atendido** exibido na barra inferior

---

## 🗂️ Estruturas de Dados

As mesmas estruturas do projeto original, implementadas manualmente em C++:

| Estrutura | Uso |
|-----------|-----|
| **Lista Linear Estática** | Armazenamento dos pacientes cadastrados |
| **Fila Circular** | Fila normal e fila prioritária de atendimento |
| **Pilha Linear** | Histórico de atendimentos com suporte a desfazer |

---

## ⚙️ Funcionalidades

- Cadastro, edição, remoção e busca de pacientes com atualização visual imediata
- Inserção na fila normal ou prioritária via checkbox
- Chamada do próximo paciente (prioridade automática para a fila prioritária)
- Histórico completo de atendimentos
- Desfazer o último atendimento
- Tabelas atualizadas em tempo real após cada operação
- Validação de entradas com mensagens de erro via `QMessageBox`

---

## 🚀 Como Compilar e Executar

### Pré-requisitos

- Qt 6.x instalado ([download](https://www.qt.io/download))
- Qt Creator (recomendado) ou qmake via terminal

### Via Qt Creator

1. Abra o Qt Creator
2. Vá em **File → Open File or Project**
3. Selecione o arquivo `.pro` do projeto
4. Clique em **Run** (▶)

### Via terminal

```bash
qmake
make
./ClinicaMedica
```

---

## 🛠️ Tecnologias

- **Linguagem:** C++17
- **Framework GUI:** Qt 6.11
- **Widgets utilizados:** `QMainWindow`, `QTableWidget`, `QLineEdit`, `QPushButton`, `QCheckBox`, `QGroupBox`, `QMessageBox`
- **Build system:** qmake
- **IDE:** Qt Creator

---

## 📁 Estrutura do Projeto

```
├── main.cpp            # Entry point
├── mainwindow.h        # Declaração da janela principal e slots
├── mainwindow.cpp      # Lógica da interface e conexão com as estruturas
├── mainwindow.ui       # Layout da interface (Qt Designer)
├── sistema.h           # Declaração das estruturas de dados (Lista, Fila, Pilha)
└── sistema.cpp         # Implementação das estruturas de dados
```

---

## 🔗 Projeto Anterior

Este app é a evolução do sistema de terminal:
👉 [Clínica Médica — C++ Terminal](https://github.com/KrzDev-debug/clinica-medica-cpp)
