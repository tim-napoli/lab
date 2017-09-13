from PyQt5.QtWidgets import QTreeView, QAbstractItemView, QMenu, QFileDialog, QMessageBox
from PyQt5.QtGui import QStandardItemModel, QStandardItem
from PyQt5.QtCore import Qt, pyqtSignal, QModelIndex, QVariant

class item_model(QStandardItemModel):
    on_rename = pyqtSignal(QModelIndex, QVariant, QVariant)

    def __init__(self):
        super().__init__()

    def setData(self, index, value, role = Qt.EditRole):
            before_value = self.data(index, role)
            self.on_rename.emit(index, before_value, value)
            return super().setData(index, value, role)

class navigation_pane(QTreeView):
    def __init__(self, parent, manager):
        super().__init__(parent)

        self.manager = manager

        self.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.build_model()
        self.setModel(self.model)
        self.setUniformRowHeights(True)

        self.setContextMenuPolicy(Qt.CustomContextMenu)
        self.customContextMenuRequested.connect(self.on_context_menu)

        self.model.on_rename.connect(self.on_rename)

    def build_model(self):
        self.model = item_model()
        self.model.setHorizontalHeaderLabels(['Data'])
        self.add_textures_node()
        self.add_images_node()

# Textures management ---------------------------------------------------------
    def add_texture_item(self, name):
        self.textures_item.appendRow(QStandardItem(name))

    def remove_texture_item(self, item):
        self.model.removeRows(item.row(), 1, self.textures_node)

    def add_textures_node(self):
        self.textures_item = QStandardItem("Textures")
        self.textures_item.setEditable(False)
        for texture in self.manager.manifest.textures:
            self.add_texture_item(texture)
        self.model.appendRow(self.textures_item)
        self.textures_node = self.model.indexFromItem(self.textures_item)

    def import_texture(self):
        path, _ = QFileDialog.getOpenFileName(self)
        new_item = self.manager.textures.create(path)
        self.add_texture_item(new_item)

    def remove_texture(self, item):
        button = QMessageBox.warning(
            self, "Texture deletion",
            "Are you sure you want to delete the texture ?",
            QMessageBox.Yes | QMessageBox.No, QMessageBox.No
        )
        if button == QMessageBox.Yes:
            name = item.text()
            self.manager.textures.delete(name)
            self.remove_texture_item(item)

    def show_textures_menu(self, point):
        menu = QMenu()
        menu.addAction("Import", self.import_texture)
        menu.exec_(self.mapToGlobal(point))

    def show_texture_menu(self, point):
        item_index = self.indexAt(point)
        item = self.model.itemFromIndex(item_index)

        menu = QMenu()
        menu.addAction("Remove", lambda: self.remove_texture(item))
        menu.exec_(self.mapToGlobal(point))

# Images management -----------------------------------------------------------
    def add_image_item(self, name):
        item = QStandardItem(name)
        self.images_item.appendRow(item)
        return item

    def add_images_node(self):
        self.images_item = QStandardItem("Images")
        self.images_item.setEditable(False)
        for image in self.manager.manifest.images:
            self.add_image_item(image)
        self.model.appendRow(self.images_item)
        self.images_node = self.model.indexFromItem(self.images_item)

    def create_image(self):
        name = self.manager.manifest.get_new_image_name()
        self.manager.images.create(name)
        item = self.add_image_item(name)
        index = self.model.indexFromItem(item)
        self.setCurrentIndex(index)

    def show_images_menu(self, point):
        menu = QMenu()
        menu.addAction("Create", self.create_image)
        menu.exec_(self.mapToGlobal(point))

# -----------------------------------------------------------------------------
    def on_context_menu(self, point):
        item = self.indexAt(point)
        if item == self.textures_node:
            self.show_textures_menu(point)
        elif item == self.images_node:
            self.show_images_menu(point)
        else:
            parent = self.model.parent(item)
            if parent == self.textures_node:
                self.show_texture_menu(point)

    def on_rename(self, index, previous_name, new_name):
        parent = self.model.parent(index)
        if parent == self.textures_node:
            self.manager.textures.rename(previous_name, new_name)
        elif parent == self.images_node:
            self.manager.images.rename(previous_name, new_name)

