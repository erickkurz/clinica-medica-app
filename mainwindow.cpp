#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QStatusBar>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

// ============================================================
//  HELPERS INTERNOS
// ============================================================

// Retorna true se a string contiver apenas letras e espacos
static bool nomeValido(const QString &nome)
{
    if (nome.trimmed().isEmpty()) return false;
    QRegularExpression re("^[\\p{L} ]+$");
    return re.match(nome.trimmed()).hasMatch();
}

// ============================================================
//  CONSTRUTOR / DESTRUTOR
// ============================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Sistema de Atendimento - Clínica Médica");
    showMaximized();

    // Campos numéricos: bloqueiam letras na digitação
    ui->editCodigo->setValidator(new QIntValidator(1, 999999, this));
    ui->editIdade->setValidator(new QIntValidator(1, 150, this));

    // Campo nome: aceita apenas letras (incluindo acentos) e espaços
    QRegularExpression reNome("^[\\p{L} ]*$");
    ui->editNome->setValidator(
        new QRegularExpressionValidator(reNome, this));

    // ── Estilo global dark ──────────────────────────────────
    setStyleSheet(R"(
        QMainWindow {
            background: #0f172a;
        }
        QGroupBox {
            color: white;
            font-size: 12pt;
            font-weight: bold;
            border: 1px solid #334155;
            border-radius: 12px;
            margin-top: 10px;
            padding-top: 15px;
            background: #1e293b;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            padding: 0 6px;
            color: #93c5fd;
        }
        QLabel {
            color: white;
        }
        QLineEdit {
            background: #334155;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px;
        }
        QLineEdit:focus {
            border: 1px solid #3b82f6;
        }
        QPushButton {
            background: #2563eb;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 10px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #3b82f6;
        }
        QPushButton:pressed {
            background: #1d4ed8;
        }
        QTableWidget {
            background: #334155;
            color: white;
            border: none;
            gridline-color: #475569;
            border-radius: 8px;
        }
        QTableWidget::item:selected {
            background: #2563eb;
        }
        QHeaderView::section {
            background: #1e40af;
            color: white;
            border: none;
            padding: 8px;
            font-weight: bold;
        }
        QCheckBox {
            color: white;
            spacing: 6px;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border-radius: 4px;
            border: 2px solid #475569;
            background: #334155;
        }
        QCheckBox::indicator:checked {
            background: #2563eb;
            border-color: #2563eb;
        }
        QStatusBar {
            background: #1e293b;
            color: #93c5fd;
            font-weight: bold;
            font-size: 10pt;
        }
        QScrollBar:vertical {
            background: #1e293b;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #475569;
            border-radius: 4px;
        }
    )");

    // ── Colunas das tabelas ─────────────────────────────────
    ui->tablePacientes->setColumnCount(3);
    ui->tableFilaNormal->setColumnCount(3);
    ui->tableFilaPrioritaria->setColumnCount(3);
    ui->tableHistorico->setColumnCount(3);

    ui->tablePacientes->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade");
    ui->tableFilaNormal->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade");
    ui->tableFilaPrioritaria->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade");
    ui->tableHistorico->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade");

    // ── Resize das colunas ──────────────────────────────────
    ui->tablePacientes->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableFilaNormal->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableFilaPrioritaria->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableHistorico->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    // ── Comportamento das tabelas ───────────────────────────
    for (QTableWidget *t : {ui->tablePacientes,
                             ui->tableFilaNormal,
                             ui->tableFilaPrioritaria,
                             ui->tableHistorico})
    {
        t->setSelectionBehavior(QAbstractItemView::SelectRows);
        t->setEditTriggers(QAbstractItemView::NoEditTriggers);
        t->verticalHeader()->setVisible(false);
        t->setAlternatingRowColors(true);
        t->setStyleSheet(t->styleSheet() +
            "QTableWidget { alternate-background-color: #3b4f6b; }");
    }

    // ── Conexões ────────────────────────────────────────────
    connect(ui->btnCadastrar,   &QPushButton::clicked,
            this, &MainWindow::cadastrarPaciente);
    connect(ui->btnAlterar,     &QPushButton::clicked,
            this, &MainWindow::alterarPaciente);
    connect(ui->btnRemover,     &QPushButton::clicked,
            this, &MainWindow::removerPaciente);
    connect(ui->btnBuscar,      &QPushButton::clicked,
            this, &MainWindow::buscarPaciente);
    connect(ui->btnInserirFila, &QPushButton::clicked,
            this, &MainWindow::inserirFila);
    connect(ui->btnChamar,      &QPushButton::clicked,
            this, &MainWindow::chamarPaciente);
    connect(ui->btnUltimo,      &QPushButton::clicked,
            this, &MainWindow::mostrarUltimoAtendimento);
    connect(ui->btnDesfazer,    &QPushButton::clicked,
            this, &MainWindow::desfazerAtendimento);

    // Clique na tabela de pacientes preenche os campos automaticamente
    connect(ui->tablePacientes, &QTableWidget::cellClicked,
            this, &MainWindow::selecionarPacienteDaTabela);

    atualizarTudo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================================
//  SLOTS — PACIENTES
// ============================================================
void MainWindow::cadastrarPaciente()
{
    bool codigoOk = false;
    int  codigo   = ui->editCodigo->text().trimmed().toInt(&codigoOk);
    QString nome  = ui->editNome->text().trimmed();
    bool idadeOk  = false;
    int  idade    = ui->editIdade->text().trimmed().toInt(&idadeOk);

    if (!codigoOk || ui->editCodigo->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Campo inválido",
            "O campo <b>Código</b> é obrigatório e deve ser um número inteiro.");
        ui->editCodigo->setFocus();
        return;
    }
    if (!nomeValido(nome))
    {
        QMessageBox::warning(this, "Campo inválido",
            "O campo <b>Nome</b> é obrigatório e deve conter apenas letras.");
        ui->editNome->setFocus();
        return;
    }
    if (!idadeOk || idade <= 0)
    {
        QMessageBox::warning(this, "Campo inválido",
            "O campo <b>Idade</b> é obrigatório e deve ser maior que zero.");
        ui->editIdade->setFocus();
        return;
    }

    QString msg;
    bool ok = sistema.cadastrarPaciente(codigo, nome, idade, msg);

    if (!ok)
    {
        QMessageBox::warning(this, "Erro ao cadastrar", msg);
        return;
    }

    statusBar()->showMessage("✔  " + msg);

    // Pergunta se deseja inserir na fila imediatamente
    QMessageBox boxFila(this);
    boxFila.setWindowTitle("Inserir na Fila?");
    boxFila.setIcon(QMessageBox::Question);
    boxFila.setText(
        QString("Paciente <b>%1</b> cadastrado com sucesso!<br><br>"
                "Deseja inserir na fila de atendimento agora?").arg(nome));

    QPushButton *btnPrioritaria = boxFila.addButton("Fila Prioritária", QMessageBox::YesRole);
    QPushButton *btnNormal      = boxFila.addButton("Fila Normal",       QMessageBox::AcceptRole);
    QPushButton *btnNao         = boxFila.addButton("Não, só cadastrar",  QMessageBox::RejectRole);
    boxFila.setDefaultButton(btnNao);
    boxFila.exec();

    QAbstractButton *clicado = boxFila.clickedButton();

    if (clicado == btnPrioritaria || clicado == btnNormal)
    {
        bool prioritario = (clicado == btnPrioritaria);
        QString msgFila;
        bool okFila = sistema.inserirNaFila(codigo, prioritario, msgFila);
        statusBar()->showMessage(okFila ? "✔  " + msgFila : "✘  " + msgFila);
    }
    else
    {
        statusBar()->showMessage("✔  Paciente cadastrado e mantido apenas na lista.");
    }

    limparCampos();
    atualizarTudo();
}

void MainWindow::alterarPaciente()
{
    bool codigoOk = false;
    int  codigo   = ui->editCodigo->text().trimmed().toInt(&codigoOk);
    QString nome  = ui->editNome->text().trimmed();
    bool idadeOk  = false;
    int  idade    = ui->editIdade->text().trimmed().toInt(&idadeOk);

    if (!codigoOk || ui->editCodigo->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Campo inválido",
            "Informe o <b>Código</b> do paciente a alterar.");
        ui->editCodigo->setFocus();
        return;
    }
    if (!nomeValido(nome))
    {
        QMessageBox::warning(this, "Campo inválido",
            "O campo <b>Nome</b> é obrigatório e deve conter apenas letras.");
        ui->editNome->setFocus();
        return;
    }
    if (!idadeOk || idade <= 0)
    {
        QMessageBox::warning(this, "Campo inválido",
            "O campo <b>Idade</b> é obrigatório e deve ser maior que zero.");
        ui->editIdade->setFocus();
        return;
    }

    QString msg;
    bool ok = sistema.alterarPaciente(codigo, nome, idade, msg);
    if (!ok)
        QMessageBox::warning(this, "Erro ao alterar", msg);
    else
        statusBar()->showMessage("✔  " + msg);
    if (ok) limparCampos();
    atualizarTudo();
}

void MainWindow::removerPaciente()
{
    bool codigoOk = false;
    int  codigo   = ui->editCodigo->text().trimmed().toInt(&codigoOk);

    if (!codigoOk || ui->editCodigo->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Campo inválido",
            "Informe o <b>Código</b> do paciente a remover.");
        ui->editCodigo->setFocus();
        return;
    }

    QString msg;
    bool ok = sistema.removerPaciente(codigo, msg);
    if (!ok)
        QMessageBox::warning(this, "Erro ao remover", msg);
    else
        statusBar()->showMessage("✔  " + msg);
    if (ok) limparCampos();
    atualizarTudo();
}

void MainWindow::buscarPaciente()
{
    bool codigoOk = false;
    int  codigo   = ui->editCodigo->text().trimmed().toInt(&codigoOk);

    if (!codigoOk || ui->editCodigo->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Campo inválido",
            "Informe o <b>Código</b> do paciente a buscar.");
        ui->editCodigo->setFocus();
        return;
    }

    Paciente paciente;
    if (!sistema.buscarPaciente(codigo, paciente))
    {
        statusBar()->showMessage(
            QString("✘  Paciente com código %1 não encontrado.").arg(codigo));
        return;
    }

    // Preenche os campos com os dados encontrados para facilitar edição
    ui->editNome->setText(paciente.nome);
    ui->editIdade->setText(QString::number(paciente.idade));
    statusBar()->showMessage(
        QString("✔  Paciente encontrado: %1 — %2 anos.")
            .arg(paciente.nome)
            .arg(paciente.idade));
}

// ============================================================
//  SLOTS — FILAS / ATENDIMENTO
// ============================================================
void MainWindow::inserirFila()
{
    bool codigoOk = false;
    int  codigo   = ui->editCodigo->text().trimmed().toInt(&codigoOk);

    if (!codigoOk || ui->editCodigo->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Campo inválido",
            "Informe o <b>Código</b> do paciente para inserir na fila.");
        ui->editCodigo->setFocus();
        return;
    }

    if (sistema.pacienteNaFila(codigo))
    {
        QMessageBox::warning(this, "Já na fila",
            "Este paciente já está em uma fila de atendimento.<br>"
            "Remova-o da fila atual antes de inserir novamente.");
        return;
    }

    bool prioritario = ui->checkPrioritario->isChecked();
    QString msg;
    bool ok = sistema.inserirNaFila(codigo, prioritario, msg);
    statusBar()->showMessage(ok ? "✔  " + msg : "✘  " + msg);
    if (ok) ui->checkPrioritario->setChecked(false);
    atualizarTudo();
}

void MainWindow::chamarPaciente()
{
    Paciente paciente;
    QString  msg;
    bool ok = sistema.chamarPaciente(paciente, msg);

    if (!ok)
    {
        statusBar()->showMessage("✘  " + msg);
        return;
    }

    statusBar()->showMessage("✔  " + msg);
    atualizarTudo();
}

void MainWindow::mostrarUltimoAtendimento()
{
    Paciente paciente;
    if (!sistema.ultimoAtendimento(paciente))
    {
        statusBar()->showMessage(
            "✘  Nenhum atendimento registrado no histórico.");
        return;
    }

    QMessageBox box(this);
    box.setWindowTitle("Último Atendimento");
    box.setText(
        QString("<b>Código:</b> %1<br>"
                "<b>Nome:</b> %2<br>"
                "<b>Idade:</b> %3 anos")
            .arg(paciente.codigo)
            .arg(paciente.nome)
            .arg(paciente.idade));
    box.setIcon(QMessageBox::Information);
    box.exec();

    statusBar()->showMessage(
        QString("✔  Último atendimento: %1 (cód. %2).")
            .arg(paciente.nome)
            .arg(paciente.codigo));
}

void MainWindow::desfazerAtendimento()
{
    // Confirma antes de desfazer
    Paciente paciente;
    if (!sistema.ultimoAtendimento(paciente))
    {
        statusBar()->showMessage("✘  Histórico vazio, nada a desfazer.");
        return;
    }

    int resposta = QMessageBox::question(
        this,
        "Desfazer Atendimento",
        QString("Deseja remover o atendimento de <b>%1</b> do histórico?")
            .arg(paciente.nome),
        QMessageBox::Yes | QMessageBox::No);

    if (resposta != QMessageBox::Yes) return;

    QString msg;
    bool ok = sistema.desfazerAtendimento(msg);
    statusBar()->showMessage(ok ? "✔  " + msg : "✘  " + msg);
    atualizarTudo();
}

// ============================================================
//  REMOVER DA FILA PELO BOTÃO X
// ============================================================
void MainWindow::removerDaFilaPorBotao(int codigo)
{
    Paciente p;
    QString nome = "";
    if (sistema.buscarPaciente(codigo, p)) nome = p.nome;

    int resp = QMessageBox::question(
        this,
        "Remover da Fila",
        QString("Remover <b>%1</b> da fila de atendimento?").arg(
            nome.isEmpty() ? QString::number(codigo) : nome),
        QMessageBox::Yes | QMessageBox::No);

    if (resp != QMessageBox::Yes) return;

    QString msg;
    bool ok = sistema.removerDaFila(codigo, msg);
    statusBar()->showMessage(ok ? "✔  " + msg : "✘  " + msg);
    atualizarTudo();
}

// ============================================================
//  SELEÇÃO POR CLIQUE NA TABELA
// ============================================================
void MainWindow::selecionarPacienteDaTabela(int row, int /*col*/)
{
    QTableWidgetItem *itemCodigo = ui->tablePacientes->item(row, 0);
    QTableWidgetItem *itemNome   = ui->tablePacientes->item(row, 1);
    QTableWidgetItem *itemIdade  = ui->tablePacientes->item(row, 2);

    if (!itemCodigo) return;

    ui->editCodigo->setText(itemCodigo->text());
    ui->editNome->setText(itemNome ? itemNome->text() : "");
    ui->editIdade->setText(itemIdade ? itemIdade->text() : "");

    statusBar()->showMessage(
        QString("✔  Paciente selecionado: %1 — clique em Inserir na Fila, Alterar ou Remover.")
            .arg(itemNome ? itemNome->text() : ""));
}

// ============================================================
//  ATUALIZAÇÃO DAS TABELAS
// ============================================================
void MainWindow::atualizarPacientes()
{
    Lista lista = sistema.obterLista();
    ui->tablePacientes->setRowCount(lista.tamanho);

    for (int i = 0; i < lista.tamanho; i++)
    {
        ui->tablePacientes->setItem(i, 0,
            new QTableWidgetItem(
                QString::number(lista.dados[i].codigo)));
        ui->tablePacientes->setItem(i, 1,
            new QTableWidgetItem(lista.dados[i].nome));
        ui->tablePacientes->setItem(i, 2,
            new QTableWidgetItem(
                QString::number(lista.dados[i].idade)));
    }
}

void MainWindow::atualizarFilaNormal()
{
    Fila fila = sistema.obterFilaNormal();
    ui->tableFilaNormal->setColumnCount(4);
    ui->tableFilaNormal->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade" << "");
    ui->tableFilaNormal->horizontalHeader()
        ->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableFilaNormal->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableFilaNormal->horizontalHeader()
        ->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableFilaNormal->horizontalHeader()
        ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableFilaNormal->setColumnWidth(3, 36);
    ui->tableFilaNormal->setRowCount(fila.tamanho);

    for (int i = 0; i < fila.tamanho; i++)
    {
        int idx = (fila.inicio + i) % MAX_FILA;
        int cod = fila.dados[idx].codigo;

        ui->tableFilaNormal->setItem(i, 0,
            new QTableWidgetItem(QString::number(cod)));
        ui->tableFilaNormal->setItem(i, 1,
            new QTableWidgetItem(fila.dados[idx].nome));
        ui->tableFilaNormal->setItem(i, 2,
            new QTableWidgetItem(QString::number(fila.dados[idx].idade)));

        QPushButton *btnX = new QPushButton("✕");
        btnX->setFixedSize(28, 28);
        btnX->setStyleSheet(
            "QPushButton { background:#dc2626; color:white; border-radius:6px;"
            " font-weight:bold; font-size:11px; padding:0; }"
            "QPushButton:hover { background:#ef4444; }"
            "QPushButton:pressed { background:#b91c1c; }");
        connect(btnX, &QPushButton::clicked, this,
            [this, cod]() { removerDaFilaPorBotao(cod); });

        QWidget *cell = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(cell);
        lay->addWidget(btnX);
        lay->setAlignment(Qt::AlignCenter);
        lay->setContentsMargins(2,2,2,2);
        ui->tableFilaNormal->setCellWidget(i, 3, cell);
    }
}

void MainWindow::atualizarFilaPrioritaria()
{
    Fila fila = sistema.obterFilaPrioritaria();
    ui->tableFilaPrioritaria->setColumnCount(4);
    ui->tableFilaPrioritaria->setHorizontalHeaderLabels(
        QStringList() << "Código" << "Nome" << "Idade" << "");
    ui->tableFilaPrioritaria->horizontalHeader()
        ->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableFilaPrioritaria->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableFilaPrioritaria->horizontalHeader()
        ->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableFilaPrioritaria->horizontalHeader()
        ->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableFilaPrioritaria->setColumnWidth(3, 36);
    ui->tableFilaPrioritaria->setRowCount(fila.tamanho);

    for (int i = 0; i < fila.tamanho; i++)
    {
        int idx = (fila.inicio + i) % MAX_FILA;
        int cod = fila.dados[idx].codigo;

        ui->tableFilaPrioritaria->setItem(i, 0,
            new QTableWidgetItem(QString::number(cod)));
        ui->tableFilaPrioritaria->setItem(i, 1,
            new QTableWidgetItem(fila.dados[idx].nome));
        ui->tableFilaPrioritaria->setItem(i, 2,
            new QTableWidgetItem(QString::number(fila.dados[idx].idade)));

        QPushButton *btnX = new QPushButton("✕");
        btnX->setFixedSize(28, 28);
        btnX->setStyleSheet(
            "QPushButton { background:#dc2626; color:white; border-radius:6px;"
            " font-weight:bold; font-size:11px; padding:0; }"
            "QPushButton:hover { background:#ef4444; }"
            "QPushButton:pressed { background:#b91c1c; }");
        connect(btnX, &QPushButton::clicked, this,
            [this, cod]() { removerDaFilaPorBotao(cod); });

        QWidget *cell = new QWidget();
        QHBoxLayout *lay = new QHBoxLayout(cell);
        lay->addWidget(btnX);
        lay->setAlignment(Qt::AlignCenter);
        lay->setContentsMargins(2,2,2,2);
        ui->tableFilaPrioritaria->setCellWidget(i, 3, cell);
    }
}

void MainWindow::atualizarHistorico()
{
    Pilha historico = sistema.obterHistorico();
    int linhas = historico.topo + 1;
    ui->tableHistorico->setRowCount(linhas);

    int row = 0;
    for (int i = historico.topo; i >= 0; i--)
    {
        ui->tableHistorico->setItem(row, 0,
            new QTableWidgetItem(
                QString::number(historico.dados[i].codigo)));
        ui->tableHistorico->setItem(row, 1,
            new QTableWidgetItem(historico.dados[i].nome));
        ui->tableHistorico->setItem(row, 2,
            new QTableWidgetItem(
                QString::number(historico.dados[i].idade)));
        row++;
    }
}

void MainWindow::atualizarTudo()
{
    atualizarPacientes();
    atualizarFilaNormal();
    atualizarFilaPrioritaria();
    atualizarHistorico();

    Paciente paciente;
    if (sistema.ultimoAtendimento(paciente))
    {
        ui->labelUltimoPaciente->setText(
            QString("Cód: %1  —  %2  —  %3 anos")
                .arg(paciente.codigo)
                .arg(paciente.nome)
                .arg(paciente.idade));
    }
    else
    {
        ui->labelUltimoPaciente->setText("Nenhum atendimento registrado");
    }
}

void MainWindow::limparCampos()
{
    ui->editCodigo->clear();
    ui->editNome->clear();
    ui->editIdade->clear();
}
