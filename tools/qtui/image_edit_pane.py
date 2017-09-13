from PyQt5.QtWidgets import QWidget, QListView, QLabel, QHBoxLayout, QVBoxLayout, QPushButton, QDialog
from PyQt5.QtGui import QStandardItem, QStandardItemModel

from qtui import textures_edit_pane

class texture_popup(QDialog):
    def __init__(self, textures):
        super().__init__()
        self.setWindowTitle("Image's texture selection")
        self.clicked_texture = None

        textures_grid = textures_edit_pane.textures_edit_pane(
            self, textures, self.on_click
        )

        select_button = QPushButton("Cancel", self)
        select_button.clicked.connect(self.reject)

        vbox = QVBoxLayout()
        vbox.addWidget(textures_grid)
        vbox.addWidget(select_button)
        self.setLayout(vbox)

    def show(self):
        return self.exec_()

    def on_click(self, miniature):
        self.clicked_texture = miniature.texture
        self.accept()

    def get_result(self):
        return self.clicked_texture

class textures_list(QWidget):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        self.manager = manager
        self.image_name = image_name
        self.image = image
        self.build_list_view()
        self.build_button_bar()

        vbox = QVBoxLayout()
        vbox.addWidget(self.list_view)
        vbox.addWidget(self.button_bar)
        self.setLayout(vbox)

    def add_texture_item(self, texture):
        item = QStandardItem(texture)
        item.setEditable(False)
        self.model.appendRow(item)

    def remove_texture_item(self, item):
        self.model.removeRows(item.row(), 1)

    def build_list_view(self):
        self.list_view = QListView(self)
        self.model = QStandardItemModel(self.list_view)
        for texture in self.image.textures:
            self.add_texture_item(texture)
        self.list_view.setModel(self.model)

    def build_button_bar(self):
        add_button = QPushButton("Add", self)
        add_button.clicked.connect(self.add_texture)
        remove_button = QPushButton("Remove", self)
        remove_button.clicked.connect(self.remove_texture)
        up_button = QPushButton("Move up", self)
        down_button = QPushButton("Move down", self)

        self.button_bar = QWidget(self)
        hbox = QHBoxLayout()
        hbox.addWidget(add_button)
        hbox.addWidget(remove_button)
        hbox.addWidget(up_button)
        hbox.addWidget(down_button)
        self.button_bar.setLayout(hbox)

    def add_texture(self):
        textures = self.manager.textures.load_all()
        dialog = texture_popup(textures)
        if dialog.show() == QDialog.Accepted:
            texture = dialog.get_result()
            self.add_texture_item(texture)
            self.image.add_texture(texture)
            self.manager.images.save(self.image_name, self.image)

    def remove_texture(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        if item != None:
            texture = item.text()
            self.remove_texture_item(item)
            self.image.remove_texture(texture)
            self.manager.images.save(self.image_name, self.image)

class hot_point_editor(QWidget):
    pass

class image_edit_pane(QWidget):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        list_view = textures_list(self, manager, image_name, image)

        vbox = QVBoxLayout()
        vbox.addWidget(list_view)
        self.setLayout(vbox)
