#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QPixmap>
#include <QString>
#include <string>
#include <math.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scores.insert(pair<string, int>("X", -1));
    scores.insert(pair<string, int>("O", 1));
    scores.insert(pair<string, int>("tie", 0));

    // Storing the lables in a 2d array
    grid_obj[0][0] = ui->l00;
    grid_obj[0][1] = ui->l01;
    grid_obj[0][2] = ui->l02;
    grid_obj[1][0] = ui->l10;
    grid_obj[1][1] = ui->l11;
    grid_obj[1][2] = ui->l12;
    grid_obj[2][0] = ui->l20;
    grid_obj[2][1] = ui->l21;
    grid_obj[2][2] = ui->l22;

    // Connecting the pressed event on the label
    connect(ui->l00, &myqlabel ::Mouse_Pressed, this, [this] { pressed(0, 0); });
    connect(ui->l01, &myqlabel ::Mouse_Pressed, this, [this] { pressed(0, 1); });
    connect(ui->l02, &myqlabel ::Mouse_Pressed, this, [this] { pressed(0, 2); });
    connect(ui->l10, &myqlabel ::Mouse_Pressed, this, [this] { pressed(1, 0); });
    connect(ui->l11, &myqlabel ::Mouse_Pressed, this, [this] { pressed(1, 1); });
    connect(ui->l12, &myqlabel ::Mouse_Pressed, this, [this] { pressed(1, 2); });
    connect(ui->l20, &myqlabel ::Mouse_Pressed, this, [this] { pressed(2, 0); });
    connect(ui->l21, &myqlabel ::Mouse_Pressed, this, [this] { pressed(2, 1); });
    connect(ui->l22, &myqlabel ::Mouse_Pressed, this, [this] { pressed(2, 2); });

    // Initializing with empty string
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            grid[i][j] = "";
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pressed(int i, int j)
{
    // Checking if the game is over
    if (isOver)
        return;
    // Preventing the user from clicking already filled label
    if (grid[i][j] != "")
        return;

    humanTurn(i, j);
}

void MainWindow::humanTurn(int i, int j)
{
    string image = ":/Image/" + player + ".png";
    QPixmap pic(image.c_str());
    grid_obj[i][j]->setPixmap(pic);
    grid[i][j] = player;

    string winner = checkWinner();

    declareWinner(winner);

    compturn();
}

bool equal(string a, string b, string c)
{
    if (a != "" && a == b && b == c)
        return true;
    return false;
}

string MainWindow ::checkWinner()
{
    string winner = "0";

    // For rows
    for (int i = 0; i < row; i++)
    {
        if (equal(grid[i][0], grid[i][1], grid[i][2]))
        {
            winner = grid[i][0];
            break;
        }
    }
    // For Colums
    for (int i = 0; i < col; i++)
    {
        if (equal(grid[0][i], grid[1][i], grid[2][i]))
        {
            winner = grid[0][i];
            break;
        }
    }

    // For Diagnal

    if (equal(grid[0][0], grid[1][1], grid[2][2]))
    {
        winner = grid[0][0];
    }

    if (equal(grid[0][2], grid[1][1], grid[2][0]))
    {
        winner = grid[0][2];
    }

    bool isEmpty = false;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] == "")
            {
                isEmpty = true;
                break;
            }
        }
    }

    if (winner == "0" && !isEmpty)
        return "tie";
    else
        return winner;
}

void MainWindow::declareWinner(string winner)
{
    if (winner == "0")
        return;
    if (winner == comp)
    {
        ui->label->setStyleSheet("color:red");
        ui->label->setText("Computer Won");
    }
    else if (winner == player)
    {
        ui->label->setStyleSheet("color:green");
        ui->label->setText("You Won");
    }
    else if (winner == "tie")
    {
        ui->label->setStyleSheet("color:yellow");
        ui->label->setText("Tie");
    }
    isOver = true;
}

void MainWindow::compturn()
{
    int bestScore = -INFINITY;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] == "")
            {
                grid[i][j] = comp;
                int score = minimax(0, false);
                grid[i][j] = "";
                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove.i = i;
                    bestMove.j = j;
                }
            }
        }
    }

    string image = ":/Image/" + comp + ".png";
    QPixmap pic(image.c_str());
    grid_obj[bestMove.i][bestMove.j]->setPixmap(pic);
    grid[bestMove.i][bestMove.j] = comp;

    string winner = checkWinner();
    declareWinner(winner);
}
// Minimax Algorithm
int MainWindow::minimax(int depth, bool isMax)
{
    string winner = checkWinner();
    if (winner != "0")
    {
        return scores[winner];
    }

    if (isMax)
    {
        int bestScore = -INFINITY;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (grid[i][j] == "")
                {
                    grid[i][j] = comp;
                    int score = minimax(depth + 1, false);
                    grid[i][j] = "";
                    if (score > bestScore)
                        bestScore = score;
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = INFINITY;
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                if (grid[i][j] == "")
                {
                    grid[i][j] = player;
                    int score = minimax(depth + 1, true);
                    grid[i][j] = "";
                    if (score < bestScore)
                        bestScore = score;
                }
            }
        }
        return bestScore;
    }
}

void MainWindow::on_Reset_clicked()
{
    isOver = false;
    ui->label->setStyleSheet("color:white");
    ui->label->setText("Your Turn");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            grid_obj[i][j]->clear();
            grid[i][j] = "";
        }
    }
}
