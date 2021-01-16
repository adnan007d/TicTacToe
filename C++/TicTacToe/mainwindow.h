#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <string>
#include <QLabel>
#include "myqlabel.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Move
{
    int i;
    int j;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void humanTurn(int, int);
    string checkWinner();
    void declareWinner(string);
    void compturn();
    int minimax(int depth, bool isMax);
private slots:
    void pressed(int, int);

    void on_Reset_clicked();

private:
    Ui::MainWindow *ui;
    string comp = "O";
    string player = "X";
    bool isOver = false;
    map <string, int> scores;
    static const int row = 3;
    static const int col = 3;
    myqlabel* grid_obj[row][col];
    string grid[row][col];
    struct Move bestMove;

};
#endif // MAINWINDOW_H
