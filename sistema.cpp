#include "sistema.h"

Sistema::Sistema()
{
    lista.tamanho = 0;

    filaNormal.inicio  = 0;
    filaNormal.fim     = 0;
    filaNormal.tamanho = 0;

    filaPrioritaria.inicio  = 0;
    filaPrioritaria.fim     = 0;
    filaPrioritaria.tamanho = 0;

    historico.topo = -1;
}

int Sistema::buscarIndicePaciente(int codigo) const
{
    for (int i = 0; i < lista.tamanho; i++)
        if (lista.dados[i].codigo == codigo)
            return i;
    return -1;
}

bool Sistema::cadastrarPaciente(int codigo,
                                const QString &nome,
                                int idade,
                                QString &msg)
{
    if (lista.tamanho >= MAX_LISTA)
    {
        msg = "Lista cheia!";
        return false;
    }
    if (buscarIndicePaciente(codigo) != -1)
    {
        msg = "Código já existente!";
        return false;
    }

    Paciente p;
    p.codigo = codigo;
    p.nome   = nome;
    p.idade  = idade;

    lista.dados[lista.tamanho] = p;
    lista.tamanho++;

    msg = "Paciente cadastrado com sucesso!";
    return true;
}

bool Sistema::alterarPaciente(int codigo,
                              const QString &nome,
                              int idade,
                              QString &msg)
{
    int idx = buscarIndicePaciente(codigo);
    if (idx == -1)
    {
        msg = "Paciente não encontrado!";
        return false;
    }

    lista.dados[idx].nome  = nome;
    lista.dados[idx].idade = idade;

    msg = "Paciente alterado com sucesso!";
    return true;
}

bool Sistema::removerPaciente(int codigo, QString &msg)
{
    int idx = buscarIndicePaciente(codigo);
    if (idx == -1)
    {
        msg = "Paciente não encontrado!";
        return false;
    }

    for (int i = idx; i < lista.tamanho - 1; i++)
        lista.dados[i] = lista.dados[i + 1];

    lista.tamanho--;

    msg = "Paciente removido com sucesso!";
    return true;
}

bool Sistema::buscarPaciente(int codigo, Paciente &paciente)
{
    int idx = buscarIndicePaciente(codigo);
    if (idx == -1)
        return false;

    paciente = lista.dados[idx];
    return true;
}

bool Sistema::enfileirar(Fila &fila, const Paciente &paciente)
{
    if (fila.tamanho >= MAX_FILA)
        return false;

    fila.dados[fila.fim] = paciente;
    fila.fim = (fila.fim + 1) % MAX_FILA;
    fila.tamanho++;
    return true;
}

bool Sistema::desenfileirar(Fila &fila, Paciente &paciente)
{
    if (fila.tamanho == 0)
        return false;

    paciente    = fila.dados[fila.inicio];
    fila.inicio = (fila.inicio + 1) % MAX_FILA;
    fila.tamanho--;
    return true;
}

bool Sistema::empilhar(const Paciente &paciente)
{
    if (historico.topo >= MAX_PILHA - 1)
        return false;

    historico.topo++;
    historico.dados[historico.topo] = paciente;
    return true;
}

bool Sistema::desempilhar(Paciente &paciente)
{
    if (historico.topo == -1)
        return false;

    paciente = historico.dados[historico.topo];
    historico.topo--;
    return true;
}

bool Sistema::inserirNaFila(int codigo,
                            bool prioritario,
                            QString &msg)
{
    int idx = buscarIndicePaciente(codigo);
    if (idx == -1)
    {
        msg = "Paciente não cadastrado!";
        return false;
    }

    if (prioritario)
    {
        if (!enfileirar(filaPrioritaria, lista.dados[idx]))
        {
            msg = "Fila prioritária cheia!";
            return false;
        }
        msg = "Inserido na fila prioritária!";
    }
    else
    {
        if (!enfileirar(filaNormal, lista.dados[idx]))
        {
            msg = "Fila normal cheia!";
            return false;
        }
        msg = "Inserido na fila normal!";
    }
    return true;
}

bool Sistema::chamarPaciente(Paciente &paciente, QString &msg)
{
    if (filaNormal.tamanho == 0 && filaPrioritaria.tamanho == 0)
    {
        msg = "Nenhum paciente aguardando!";
        return false;
    }

    if (filaPrioritaria.tamanho > 0)
        desenfileirar(filaPrioritaria, paciente);
    else
        desenfileirar(filaNormal, paciente);

    empilhar(paciente);

    msg = "Paciente chamado: " + paciente.nome;
    return true;
}

bool Sistema::ultimoAtendimento(Paciente &paciente)
{
    if (historico.topo == -1)
        return false;

    paciente = historico.dados[historico.topo];
    return true;
}

bool Sistema::desfazerAtendimento(QString &msg)
{
    Paciente paciente;
    if (!desempilhar(paciente))
    {
        msg = "Histórico vazio!";
        return false;
    }

    msg = "Atendimento de " + paciente.nome + " desfeito!";
    return true;
}

// Verifica se paciente já está em alguma fila (normal ou prioritária)
bool Sistema::pacienteNaFila(int codigo) const
{
    for (int i = 0; i < filaNormal.tamanho; i++)
    {
        int idx = (filaNormal.inicio + i) % MAX_FILA;
        if (filaNormal.dados[idx].codigo == codigo)
            return true;
    }
    for (int i = 0; i < filaPrioritaria.tamanho; i++)
    {
        int idx = (filaPrioritaria.inicio + i) % MAX_FILA;
        if (filaPrioritaria.dados[idx].codigo == codigo)
            return true;
    }
    return false;
}

// Remove paciente de qualquer fila pelo código (reconstrói a fila sem ele)
bool Sistema::removerDaFila(int codigo, QString &msg)
{
    auto removerDe = [&](Fila &fila) -> bool {
        for (int i = 0; i < fila.tamanho; i++)
        {
            int idx = (fila.inicio + i) % MAX_FILA;
            if (fila.dados[idx].codigo == codigo)
            {
                // Reconstrói a fila pulando o elemento removido
                Fila nova;
                nova.inicio  = 0;
                nova.fim     = 0;
                nova.tamanho = 0;
                for (int j = 0; j < fila.tamanho; j++)
                {
                    int jdx = (fila.inicio + j) % MAX_FILA;
                    if (fila.dados[jdx].codigo != codigo)
                    {
                        nova.dados[nova.fim] = fila.dados[jdx];
                        nova.fim = (nova.fim + 1) % MAX_FILA;
                        nova.tamanho++;
                    }
                }
                fila = nova;
                return true;
            }
        }
        return false;
    };

    if (removerDe(filaNormal))
    {
        msg = "Paciente removido da fila normal.";
        return true;
    }
    if (removerDe(filaPrioritaria))
    {
        msg = "Paciente removido da fila prioritária.";
        return true;
    }

    msg = "Paciente não encontrado em nenhuma fila.";
    return false;
}

Lista Sistema::obterLista() const
{
    return lista;
}

Fila Sistema::obterFilaNormal() const
{
    return filaNormal;
}

Fila Sistema::obterFilaPrioritaria() const
{
    return filaPrioritaria;
}

Pilha Sistema::obterHistorico() const
{
    return historico;
}
