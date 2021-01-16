#!/usr/bin/python3
from PyQt5.QtWidgets import QApplication
from main import TicTacToe
import sys


app = QApplication(sys.argv)
win = TicTacToe()
win.show()
app.exec_()