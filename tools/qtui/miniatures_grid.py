from PyQt5.QtWidgets import QWidget, QGridLayout
from PyQt5.QtCore import pyqtSignal, QEvent

class miniature(QWidget):
    clicked = pyqtSignal(object)

    def __init__(self, parent):
        super().__init__(parent)

    def event(self, evt):
        if evt.type() == QEvent.MouseButtonPress:
            self.clicked.emit(self)
        return super().event(evt)

class miniatures_grid(QWidget):
    def __init__(self, parent, miniatures,
                 miniatures_per_row,
                 on_miniature_click_cb = None):
        super().__init__(parent)

        self.miniatures_per_row = miniatures_per_row
        self.on_miniature_click_cb = on_miniature_click_cb
        self.cursor = 0

        self.grid = QGridLayout()
        self.setLayout(self.grid)

    def add_miniature(self, miniature):
        if self.on_miniature_click_cb != None:
            miniature.clicked.connect(self.on_miniature_click_cb)
        y = self.cursor // self.miniatures_per_row
        x = self.cursor % self.miniatures_per_row
        self.grid.addWidget(miniature, y, x)
        self.cursor = self.cursor + 1
