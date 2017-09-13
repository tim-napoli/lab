from PyQt5.QtWidgets import QLabel
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt

class pixmap_label(QLabel):
    """Special type of label used to store image. This label will resize
    itx pixmap in order to preserve its ratio.
    """
    def __init__(self, parent, pixmap):
        super().__init__(parent)
        self.pixmap = pixmap
        self.setAlignment(Qt.AlignCenter)
        self.setScaledContents(False)
        self.setPixmap(pixmap)

    def setPixmap(self, pixmap):
        new_pixmap = self.pixmap.scaled(self.size(), Qt.KeepAspectRatio)
        super().setPixmap(new_pixmap)

    def resizeEvent(self, event):
        self.setPixmap(self.pixmap)
