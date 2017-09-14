from PyQt5.QtWidgets import QDialog, QPushButton, QVBoxLayout

from qtui.miniatures_grid import miniatures_grid

class miniature_selection_popup(QDialog):
    def __init__(self, name, miniatures, miniatures_per_row):
        super().__init__()
        self.setWindowTitle(name)
        self.clicked_miniature = None

        self.grid = miniatures_grid(
            self, miniatures, miniatures_per_row, self.on_click
        )

        cancel_button = QPushButton("Cancel", self)
        cancel_button.clicked.connect(self.reject)

        vbox = QVBoxLayout()
        vbox.addWidget(self.grid)
        vbox.addWidget(cancel_button)
        self.setLayout(vbox)

    def show(self):
        return self.exec_()

    def on_click(self, miniature):
        self.clicked_miniature = miniature
        self.accept()

    def get_result(self):
        return self.clicked_miniature

    def add_miniature(self, miniature):
        self.grid.add_miniature(miniature)
