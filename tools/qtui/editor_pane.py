from PyQt5.QtWidgets import QWidget, QHBoxLayout, QLabel

from qtui import navigation_pane

class editor_pane(QWidget):
    def __init__(self, parent, manager):
        super().__init__(parent)

        hbox = QHBoxLayout()
        hbox.addWidget(navigation_pane.navigation_pane(self, manager), 1)
        hbox.addWidget(QLabel("Data Editor pane", self), 5)
        self.setLayout(hbox)

