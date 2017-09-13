from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout, QGridLayout, QGroupBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt, QEvent, pyqtSignal

from qtui import pixmap_label

class texture_miniature(QWidget):
    clicked = pyqtSignal(object)

    def __init__(self, parent, texture_name, texture_path):
        super().__init__(parent)

        self.texture = texture_name
        self.image = pixmap_label.pixmap_label(self, QPixmap(texture_path))

        self.label = QLabel(texture_name, self)
        self.label.setAlignment(Qt.AlignCenter)

        layout = QVBoxLayout()
        layout.addWidget(self.image)
        layout.addWidget(self.label)
        self.setLayout(layout)

    def event(self, evt):
        if evt.type() == QEvent.MouseButtonPress:
            self.clicked.emit(self)
        return super().event(evt)

class textures_edit_pane(QWidget):
    def __init__(self, parent, textures, on_miniature_click_cb = None):
        super().__init__(parent)

        self.miniatures = [
            texture_miniature(self, *texture)
            for texture in textures
        ]

        grid = QGridLayout()
        current_column = 0
        current_row = 0
        for miniature in self.miniatures:
            if on_miniature_click_cb != None:
                miniature.clicked.connect(on_miniature_click_cb)
            grid.addWidget(miniature, current_row, current_column)
            current_column = current_column + 1
            if current_column == 5:
                current_column = 0
                current_row = current_row + 1

        self.setLayout(grid)

