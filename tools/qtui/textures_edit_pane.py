from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout, QGridLayout, QGroupBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

from qtui import pixmap_label

class texture_miniature(QWidget):
    def __init__(self, parent, texture_name, texture_path):
        super().__init__(parent)

        self.image = pixmap_label.pixmap_label(self, QPixmap(texture_path))

        self.label = QLabel(texture_name, self)
        self.label.setAlignment(Qt.AlignCenter)

        layout = QVBoxLayout()
        layout.addWidget(self.image)
        layout.addWidget(self.label)
        self.setLayout(layout)

class textures_edit_pane(QWidget):
    def __init__(self, parent, textures):
        super().__init__(parent)

        self.miniatures = [
            texture_miniature(self, *texture)
            for texture in textures
        ]

        grid = QGridLayout()
        current_column = 0
        current_row = 0
        for miniature in self.miniatures:
            grid.addWidget(miniature, current_row, current_column)
            current_column = current_column + 1
            if current_column == 5:
                current_column = 0
                current_row = current_row + 1

        self.setLayout(grid)

