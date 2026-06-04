#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sistema.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cadastrarPaciente();
    void alterarPaciente();
    void removerPaciente();
    void buscarPaciente();
    void inserirFila();
    void chamarPaciente();
    void mostrarUltimoAtendimento();
    void desfazerAtendimento();
    void selecionarPacienteDaTabela(int row, int col);
    void removerDaFilaPorBotao(int codigo);

private:
    Ui::MainWindow *ui;
    Sistema         sistema;

    void atualizarPacientes();
    void atualizarFilaNormal();
    void atualizarFilaPrioritaria();
    void atualizarHistorico();
    void atualizarTudo();
    void limparCampos();
};

#endif // MAINWINDOW_H
