from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QListView, QPushButton, QHBoxLayout
)
from PyQt5.QtGui import QStandardItem, QStandardItemModel
from PyQt5.QtCore import pyqtSignal

class data_list(QWidget):
    data_changed = pyqtSignal()

    def __init__(self, parent, list_data):
        super().__init__(parent)

        self.build_list_view(list_data)
        self.build_button_bar()

        vbox = QVBoxLayout()
        vbox.addWidget(self.list_view)
        vbox.addWidget(self.button_bar)
        self.setLayout(vbox)

    def add_data_item(self, data):
        item = QStandardItem(data)
        item.setEditable(False)
        self.model.appendRow(item)

    def remove_data_item(self, item):
        self.model.removeRows(item.row(), 1)

    def move_up_data_item(self, item):
        row = item.row()
        if row > 0:
            self.model.takeRow(row)
            self.model.insertRow(row - 1, item)
            index = self.model.indexFromItem(item)
            self.list_view.setCurrentIndex(index)

    def move_down_data_item(self, item):
        row = item.row()
        if row < self.model.rowCount() - 1:
            self.model.takeRow(row)
            self.model.insertRow(row + 1, item)
            index = self.model.indexFromItem(item)
            self.list_view.setCurrentIndex(index)

    def build_list_view(self, list_data):
        self.list_view = QListView(self)
        self.model = QStandardItemModel(self.list_view)
        for data in list_data:
            self.add_data_item(data)
        self.list_view.setModel(self.model)

    def build_button_bar(self):
        add_button = QPushButton("Add", self)
        add_button.clicked.connect(self.__add_data)
        remove_button = QPushButton("Remove", self)
        remove_button.clicked.connect(self.__remove_data)
        up_button = QPushButton("Move up", self)
        up_button.clicked.connect(self.__move_up_data)
        down_button = QPushButton("Move down", self)
        down_button.clicked.connect(self.__move_down_data)

        self.button_bar = QWidget(self)
        hbox = QHBoxLayout()
        hbox.addWidget(add_button)
        hbox.addWidget(remove_button)
        hbox.addWidget(up_button)
        hbox.addWidget(down_button)
        self.button_bar.setLayout(hbox)

    def get_selected_item(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        return item

    def get_selected_data(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        if item != None:
            return item.text()
        return None

    def __add_data(self):
        result = self.add_data()
        if result != None:
            self.add_data_item(result)
            self.data_changed.emit()

    def __remove_data(self):
        if self.get_selected_item() == None:
            return
        if self.remove_data():
            self.remove_data_item(self.get_selected_item())
            self.data_changed.emit()

    def __move_up_data(self):
        if self.get_selected_item() == None:
            return
        if self.move_up_data():
            self.move_up_data_item(self.get_selected_item())
            self.data_changed.emit()

    def __move_down_data(self):
        if self.get_selected_item() == None:
            return
        if self.move_down_data():
            self.move_down_data_item(self.get_selected_item())
            self.data_changed.emit()

    def add_data(self):
        """Called when the user click the "Add" button. Must return the
        new data name.
        """
        raise Exception("Not implemented")

    def remove_data(self):
        """Called when the user click the "Remove" button and a data is
        currently selected in the list. Must return True is the data is
        effectively suppressed.
        """
        raise Exception("Not implemented")

    def move_up_data(self):
        """Called when the user click the "Move Up" button and a data is
        currently selected in the list. Must return True is the data is
        effectively moved up.
        """
        raise Exception("Not implemented")

    def move_down_data(self):
        """Called when the user click the "Move Down" button and a data is
        currently selected in the list. Must return True is the data is
        effectively moved down.
        """
        raise Exception("Not implemented")

