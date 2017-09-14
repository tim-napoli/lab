from PyQt5.QtWidgets import QWidget, QHBoxLayout, QLabel

from qtui import navigation_pane
from qtui import texture_edit_pane
from qtui import textures_edit_pane
from qtui import image_edit_pane

class editor_pane(QWidget):
    def __init__(self, parent, manager):
        super().__init__(parent)

        self.manager = manager

        self.navigation_pane = navigation_pane.navigation_pane(self, manager)
        self.navigation_pane.clicked.connect(self.on_navigation_pane_item_click)
        self.navigation_pane.data_rename.connect(self.on_data_rename)

        self.edit_pane = QLabel("Data Editor pane", self)

        self.hbox = QHBoxLayout()
        self.hbox.addWidget(self.navigation_pane, 1)
        self.hbox.addWidget(self.edit_pane, 5)
        self.setLayout(self.hbox)

    def remove_edit_pane(self):
        if self.edit_pane != None:
            self.edit_pane.deleteLater()
            self.edit_pane.close()
            self.edit_pane = None

    def emplace_edit_pane(self, pane):
        self.remove_edit_pane()
        self.hbox.addWidget(pane, 5)
        self.edit_pane = pane

    def start_texture_edit_pane(self, texture):
        texture_path = self.manager["textures"].load(texture)
        pane = texture_edit_pane.texture_edit_pane(self, texture_path)
        self.emplace_edit_pane(pane)

    def start_textures_edit_pane(self):
        textures = self.manager["textures"].load_all()
        pane = textures_edit_pane.textures_edit_pane(self, textures)
        self.emplace_edit_pane(pane)

    def start_image_edit_pane(self, name):
        image = self.manager["images"].load(name)
        pane = image_edit_pane.image_edit_pane(self, self.manager, name, image)
        self.emplace_edit_pane(pane)

    def on_navigation_pane_item_click(self, index):
        item_name = self.navigation_pane.get_item_name(index)
        if item_name == "textures":
            self.start_textures_edit_pane()
        elif item_name == "images":
            return
        else:
            data_name = self.navigation_pane.get_item_name(index)
            parent = self.navigation_pane.get_parent_name(index)
            if parent == "textures":
                self.start_texture_edit_pane(data_name)
            elif parent == "images":
                self.start_image_edit_pane(data_name)

    def on_data_rename(self, parent, new_name):
        if type(self.edit_pane) == image_edit_pane.image_edit_pane:
            self.start_image_edit_pane(new_name)

