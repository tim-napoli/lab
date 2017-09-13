from PyQt5.QtWidgets import QWidget, QListView, QLabel, QHBoxLayout, QVBoxLayout, QPushButton
from PyQt5.QtGui import QStandardItem, QStandardItemModel

class textures_list(QWidget):
    def __init__(self, parent, image):
        super().__init__(parent)

        self.image = image
        self.build_list_view()
        self.build_button_bar()

        vbox = QVBoxLayout()
        vbox.addWidget(self.list_view)
        vbox.addWidget(self.button_bar)
        self.setLayout(vbox)

    def add_texture_item(self, texture):
        item = QStandardItem(texture)
        self.model.appendRow(item)

    def build_list_view(self):
        self.list_view = QListView(self)
        self.model = QStandardItemModel(self.list_view)
        for texture in self.image.textures:
            self.add_texture_item(texture)
        self.list_view.setModel(self.model)

    def build_button_bar(self):
        add_button = QPushButton("Add", self)
        remove_button = QPushButton("Remove", self)
        up_button = QPushButton("Move up", self)
        down_button = QPushButton("Move down", self)

        self.button_bar = QWidget(self)
        hbox = QHBoxLayout()
        hbox.addWidget(add_button)
        hbox.addWidget(remove_button)
        hbox.addWidget(up_button)
        hbox.addWidget(down_button)
        self.button_bar.setLayout(hbox)

class hot_point_editor(QWidget):
    pass

class image_edit_pane(QWidget):
    def __init__(self, parent, image):
        super().__init__(parent)

        list_view = textures_list(self, image)

        vbox = QVBoxLayout()
        vbox.addWidget(list_view)
        self.setLayout(vbox)
