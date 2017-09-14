from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout, QGridLayout, QGroupBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt, QEvent, pyqtSignal

from qtui import pixmap_label
from qtui.miniatures_grid import miniature, miniatures_grid

class texture_miniature(miniature):
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

class textures_edit_pane(miniatures_grid):
    def __init__(self, parent, textures, on_miniature_click_cb = None):
        super().__init__(parent, [], 5, on_miniature_click_cb)
        for texture in textures:
            self.add_miniature(texture_miniature(self, *texture))

