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
    data_rename = pyqtSignal(str, str)

    def __init__(self, parent, manager):
        super().__init__(parent)

        self.manager = manager
        self.root_items = {}
        self.root_indexes = {}

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
        for name, node in self.manager.manifest.nodes.items():
            self.add_data_node(node)

# Node management -------------------------------------------------------------
    def add_data_item(self, parent, name):
        self.root_items[parent].appendRow(QStandardItem(name))

    def remove_data_item(self, parent, item):
        self.model.removeRows(item.row(), 1, self.root_indexes[parent])

    def add_data_node(self, manifest_node):
        name = manifest_node.name
        self.root_items[name] = QStandardItem(name)
        self.root_items[name].setEditable(False)
        for data in manifest_node.data_list:
            self.add_data_item(name, data)
        self.model.appendRow(self.root_items[name])
        self.root_indexes[name] = self.model.indexFromItem(
            self.root_items[name]
        )

    def create_data(self, parent):
        name = self.manager.manifest.nodes[parent].get_new_data_name()
        self.manager.managers[parent].create(name)
        item = self.add_data_item(parent, name)
        index = self.model.indexFromItem(item)
        self.setCurrentIndex(index)
        self.manager.manifest.save()

    def remove_data(self, parent, item):
        button = QMessageBox.warning(
            self, "{} deletion".format(parent),
            "Are you sure you want to delete the data ?",
            QMessageBox.Yes | QMessageBox.No, QMessageBox.No
        )
        if button == QMessageBox.Yes:
            name = item.text()
            self.manager.managers[parent].delete(name)
            self.remove_data_item(parent, item)
            self.manager.manifest.save()

    def show_data_list_menu(self, point):
        item_index = self.indexAt(point)
        item = self.model.itemFromIndex(item_index)

        menu = QMenu()
        menu.addAction("Create", lambda: self.create_data(item.text()))
        menu.exec_(self.mapToGlobal(point))

    def show_data_menu(self, point):
        item_index = self.indexAt(point)
        parent_index = self.model.parent(item_index)
        item = self.model.itemFromIndex(item_index)
        parent = self.model.itemFromIndex(parent_index)

        menu = QMenu()
        menu.addAction("Remove", lambda: self.remove_data(parent.text(), item))
        menu.exec_(self.mapToGlobal(point))

# textures --------------------------------------------------------------------
    def create_texture(self):
        name = QFileDialog.getOpenFileName(self, "Choose a texture file")[0]
        name = self.manager.managers["textures"].create(name)
        item = self.add_data_item("textures", name)
        index = self.model.indexFromItem(item)
        self.setCurrentIndex(index)
        self.manager.manifest.save()

    def show_texture_popup(self, point):
        menu = QMenu()
        menu.addAction("Import", self.create_texture)
        menu.exec_(self.mapToGlobal(point))

# -----------------------------------------------------------------------------
    def is_item_index_a_root(self, index):
        for _, root_index in self.root_indexes.items():
            if index == root_index:
                return True
        return False

    def get_item_name(self, index):
        item = self.model.itemFromIndex(index)
        return item.text()

    def get_parent_name(self, index):
        parent_index = self.model.parent(index)
        parent = self.model.itemFromIndex(parent_index)
        return parent.text()

    def on_context_menu(self, point):
        item_index = self.indexAt(point)
        if self.is_item_index_a_root(item_index):
            item = self.model.itemFromIndex(item_index)
            if item.text() == "textures":
                self.show_texture_popup(point)
            else:
                self.show_data_list_menu(point)
        else:
            self.show_data_menu(point)

    def on_rename(self, index, previous_name, new_name):
        parent_index = self.model.parent(index)
        parent = self.model.itemFromIndex(parent_index).text()
        self.manager.managers[parent].rename(previous_name, new_name)
        self.manager.manifest.save()
        self.data_rename.emit(parent, new_name)

