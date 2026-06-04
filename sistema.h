#ifndef SISTEMA_H
#define SISTEMA_H

#include <QString>

const int MAX_LISTA = 100;
const int MAX_FILA  = 50;
const int MAX_PILHA = 100;

// ============================================================
//  STRUCT PACIENTE
// ============================================================
struct Paciente
{
    int     codigo;
    QString nome;
    int     idade;
};

// ============================================================
//  LISTA LINEAR
// ============================================================
struct Lista
{
    Paciente dados[MAX_LISTA];
    int      tamanho;
};

// ============================================================
//  FILA LINEAR (circular)
// ============================================================
struct Fila
{
    Paciente dados[MAX_FILA];
    int      inicio;
    int      fim;
    int      tamanho;
};

// ============================================================
//  PILHA LINEAR
// ============================================================
struct Pilha
{
    Paciente dados[MAX_PILHA];
    int      topo;
};

// ============================================================
//  CLASSE SISTEMA
// ============================================================
class Sistema
{
public:
    Sistema();

    // -- Pacientes --
    bool cadastrarPaciente(int codigo,
                           const QString &nome,
                           int idade,
                           QString &msg);

    bool alterarPaciente(int codigo,
                         const QString &nome,
                         int idade,
                         QString &msg);

    bool removerPaciente(int codigo,
                         QString &msg);

    bool buscarPaciente(int codigo,
                        Paciente &paciente);

    // -- Filas --
    bool inserirNaFila(int codigo,
                       bool prioritario,
                       QString &msg);

    bool chamarPaciente(Paciente &paciente,
                        QString &msg);

    // -- Histórico --
    bool ultimoAtendimento(Paciente &paciente);

    bool desfazerAtendimento(QString &msg);
    bool pacienteNaFila(int codigo) const;
    bool removerDaFila(int codigo, QString &msg);

    // -- Getters (cópia segura para leitura) --
    Lista  obterLista()            const;
    Fila   obterFilaNormal()       const;
    Fila   obterFilaPrioritaria()  const;
    Pilha  obterHistorico()        const;

private:
    Lista  lista;
    Fila   filaNormal;
    Fila   filaPrioritaria;
    Pilha  historico;

    int  buscarIndicePaciente(int codigo) const;
    bool enfileirar(Fila &fila, const Paciente &paciente);
    bool desenfileirar(Fila &fila, Paciente &paciente);
    bool empilhar(const Paciente &paciente);
    bool desempilhar(Paciente &paciente);
};

#endif // SISTEMA_H
