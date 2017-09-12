from PyQt5.QtWidgets import QWidget, QLabel, QHBoxLayout
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

class texture_edit_pane(QWidget):
    def __init__(self, parent, image_path):
        super().__init__(parent)

        self.label = QLabel(self)
        self.label.setPixmap(QPixmap(image_path))
        self.label.setAlignment(Qt.AlignCenter)

        hbox = QHBoxLayout()
        hbox.addWidget(self.label)
        self.setLayout(hbox)
