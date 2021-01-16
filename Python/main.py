#!/usr/bin/python3
from PyQt5.QtGui import QPixmap
from PyQt5.QtWidgets import QMainWindow
from PyQt5.uic import loadUi
from PyQt5.QtCore import QObject, pyqtSignal, QEvent
from math import inf as Infinity

# Making the labels clickable


def clickable(widget):

    class Filter(QObject):

        clicked = pyqtSignal()

        def eventFilter(self, obj, event):

            if obj == widget:
                if event.type() == QEvent.MouseButtonRelease:
                    if obj.rect().contains(event.pos()):
                        self.clicked.emit()
                        # The developer can opt for .emit(obj) to get the object within the slot.
                        return True

            return False

    filter = Filter(widget)
    widget.installEventFilter(filter)
    return filter.clicked


class TicTacToe(QMainWindow):
    def __init__(self):
        super(TicTacToe, self).__init__()
        self.set = True
        self.initUI()

    def initUI(self):
        loadUi('mainwindow.ui', self)
        self.grid_obj = [
            [self.l00, self.l01, self.l02],
            [self.l10, self.l11, self.l12],
            [self.l20, self.l21, self.l22]
        ]

        self.isOver = False

        self.grid = [["" for i in range(3)] for j in range(3)]

        self.comp = "X"
        self.player = "O"
        self.scores = {
            "X": 1,
            "O": -1,
            "tie": 0
        }

        clickable(self.l00).connect(lambda: self.create(0, 0))
        clickable(self.l01).connect(lambda: self.create(0, 1))
        clickable(self.l02).connect(lambda: self.create(0, 2))
        clickable(self.l10).connect(lambda: self.create(1, 0))
        clickable(self.l11).connect(lambda: self.create(1, 1))
        clickable(self.l12).connect(lambda: self.create(1, 2))
        clickable(self.l20).connect(lambda: self.create(2, 0))
        clickable(self.l21).connect(lambda: self.create(2, 1))
        clickable(self.l22).connect(lambda: self.create(2, 2))

        self.Reset.clicked.connect(self.reset)

        self.compturn()

    def create(self, i, j):
        if self.isOver:
            return

        if self.grid[i][j] != "":
            return

        image = "Image/" + self.player + ".png"
        pic = QPixmap(image)
        self.grid_obj[i][j].setPixmap(pic)
        self.grid[i][j] = self.player

        winner = self.check_winner()

        self.declareWinner(winner)

        if self.isOver:
            return

        self.compturn()

    def compturn(self):

        bestMove = {"i": -1, "j": -1}
        bestScore = -Infinity
        for i in range(len(self.grid)):
            for j in range(len(self.grid[i])):
                if self.grid[i][j] == "":
                    self.grid[i][j] = self.comp
                    score = self.minimax(0, False)
                    self.grid[i][j] = ""
                    if score > bestScore:
                        bestScore = score
                        bestMove["i"] = i
                        bestMove["j"] = j

        i, j = bestMove["i"], bestMove["j"]
        image = "Image/" + self.comp + ".png"
        pic = QPixmap(image)
        self.grid_obj[i][j].setPixmap(pic)
        self.grid[i][j] = self.comp

        winner = self.check_winner()
        self.declareWinner(winner)

    def check_winner(self):

        winner = None

        # For rows
        for i in range(len(self.grid)):
            if self.grid[i][0] and self.grid[i][0] == self.grid[i][1] and self.grid[i][1] == self.grid[i][2]:
                winner = self.grid[i][0]
                break
        # For Columns
        for i in range(len(self.grid)):
            if self.grid[0][i] and self.grid[0][i] == self.grid[1][i] and self.grid[1][i] == self.grid[2][i]:
                winner = self.grid[0][i]
                break

        # For diagonals
        if self.grid[0][0] and self.grid[0][0] == self.grid[1][1] and self.grid[1][1] == self.grid[2][2]:
            winner = self.grid[0][0]

        if self.grid[0][2] and self.grid[0][2] == self.grid[1][1] and self.grid[1][1] == self.grid[2][0]:
            winner = self.grid[0][2]

        isEmpty = False

        for i in self.grid:
            for j in i:
                if j == "":
                    isEmpty = True
                    break

        if not winner and not isEmpty:
            return "tie"
        else:
            return winner

    def declareWinner(self, winner):
        if winner == None:
            return
        if winner == self.comp:
            self.label.setStyleSheet("color:red")
            self.label.setText("Computer Won")
        elif winner == self.player:
            self.label.setStyleSheet("color:green")
            self.label.setText("You Won")
        else:
            self.label.setStyleSheet("color:Yellow")
            self.label.setText("Tie")
        self.isOver = True

    def minimax(self, depth, isMax):

        winner = self.check_winner()
        if winner != None:
            return self.scores[winner]
        if isMax:
            bestScore = -Infinity
            for i in range(len(self.grid)):
                for j in range(len(self.grid[i])):
                    if self.grid[i][j] == "":
                        self.grid[i][j] = self.comp
                        score = self.minimax(depth + 1, False)
                        self.grid[i][j] = ""
                        bestScore = max(score, bestScore)
            return bestScore
        else:
            bestScore = Infinity
            for i in range(len(self.grid)):
                for j in range(len(self.grid[i])):
                    if self.grid[i][j] == "":
                        self.grid[i][j] = self.player
                        score = self.minimax(depth + 1, True)
                        self.grid[i][j] = ""
                        bestScore = min(score, bestScore)
            return bestScore

    def reset(self):
        self.label.setText("Your turn")
        self.label.setStyleSheet("color:white")
        self.isOver = False
        self.comp = "O"
        self.player = "X"
        self.scores["X"] = -1
        self.scores["O"] = 1

        for i in range(3):
            for j in range(3):
                self.grid[i][j] = ""
                self.grid_obj[i][j].clear()
                self.grid_obj[i][j].setStyleSheet("background:white;")


if __name__ == '__main__':
    print("Run tictactoe.py")
