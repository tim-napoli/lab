from PyQt5.QtWidgets import (
    QWidget, QLabel, QVBoxLayout, QTableView, QHBoxLayout, QPushButton,
    QDialog, QErrorMessage
)
from PyQt5.QtGui import QStandardItemModel, QStandardItem, QPixmap
from PyQt5.QtCore import Qt

from qtui.pixmap_label import pixmap_label
from qtui.miniatures_grid import miniature
from qtui.miniature_selection_popup import miniature_selection_popup
from pylab.gfx.animation import animation_frame

# image_miniature -------------------------------------------------------------
class image_miniature(miniature):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        self.image_name = image_name

        vbox = QVBoxLayout()
        if len(image.textures) > 0:
            texture = manager["textures"].load(image.textures[0])
            pixmap = QPixmap(texture)
            label = pixmap_label(self, pixmap)
            vbox.addWidget(label)
        title_label = QLabel(image_name, self)
        title_label.setAlignment(Qt.AlignCenter)
        vbox.addWidget(title_label)
        self.setLayout(vbox)

# image_selection_popup -------------------------------------------------------
class image_selection_popup(miniature_selection_popup):
    def __init__(self, manager, images):
        super().__init__("Image selection", [], 5)
        for image in images:
            miniature = image_miniature(self, manager, *image)
            self.add_miniature(miniature)

# frames_table ----------------------------------------------------------------
class frames_table(QWidget):
    def __init__(self, parent, manager, animation_name, animation):
        super().__init__(parent)

        self.manager = manager
        self.animation_name = animation_name
        self.animation = animation

        self.build_table_view()
        self.build_button_bar_view()

        vbox = QVBoxLayout()
        vbox.addWidget(self.table_view)
        vbox.addWidget(self.button_bar)
        self.setLayout(vbox)

    def build_button_bar_view(self):
        self.button_bar = QWidget(self)
        hbox = QHBoxLayout()
        add_button = QPushButton("Add", self)
        add_button.clicked.connect(self.add_data)
        remove_button = QPushButton("Remove", self)
        remove_button.clicked.connect(self.remove_data)
        move_up_button = QPushButton("Move Up", self)
        move_up_button.clicked.connect(self.move_up_data)
        move_down_button = QPushButton("Move Down", self)
        move_down_button.clicked.connect(self.move_down_data)
        hbox.addWidget(add_button)
        hbox.addWidget(remove_button)
        hbox.addWidget(move_up_button)
        hbox.addWidget(move_down_button)
        self.button_bar.setLayout(hbox)

    def build_table_view(self):
        self.table_view = QTableView(self)
        self.model = QStandardItemModel(self.table_view)
        self.model.setHorizontalHeaderLabels(["Image", "Tick duration"])
        self.model.itemChanged.connect(self.on_item_changed)
        self.table_view.setModel(self.model)

        for frame in self.animation.frames:
            self.add_data_item(frame)

    def add_data_item(self, data):
        image_item = QStandardItem(data.image)
        image_item.setEditable(False)
        ticks_item = QStandardItem(str(data.tick_duration))
        self.model.appendRow([image_item, ticks_item])

    def remove_data_item(self, item):
        self.model.removeRows(item.row(), 1)

    def move_up_data_item(self, item):
        row = item.row()
        if row > 0:
            items = self.model.takeRow(row)
            self.model.insertRow(row - 1, items)
            index = self.model.indexFromItem(item)
            self.table_view.setCurrentIndex(index)

    def move_down_data_item(self, item):
        row = item.row()
        if row < self.model.rowCount() - 1:
            items = self.model.takeRow(row)
            self.model.insertRow(row + 1, items)
            index = self.model.indexFromItem(item)
            self.table_view.setCurrentIndex(index)

    def on_item_changed(self, item):
        row = item.row()
        tick_duration = item.text()
        try:
            tick_duration = int(tick_duration)
        except ValueError as ex:
            self.show_error_popup("Value must be an integer")
            item.setText(str(self.animation.get_frame(row).get_duration()))
            return
        try:
            self.animation.get_frame(row).set_duration(tick_duration)
            self.save_animation()
        except ValueError as ex:
            self.show_error_popup(str(ex))

    def add_data(self):
        images = self.manager["images"].load_all()
        selection_popup = image_selection_popup(self.manager, images)
        if selection_popup.show() == QDialog.Accepted:
            image = selection_popup.get_result().image_name
            frame = animation_frame(image, 1)
            self.animation.add_frame(frame)
            self.save_animation()
            self.add_data_item(frame)

    def remove_data(self):
        item = self.get_selected_item()
        if item != None:
            self.animation.remove_frame(item.row())
            self.save_animation()
            self.remove_data_item(item)

    def move_up_data(self):
        item = self.get_selected_item()
        if item != None:
            self.animation.move_up_frame(item.row())
            self.save_animation()
            self.move_up_data_item(item)

    def move_down_data(self):
        item = self.get_selected_item()
        if item != None:
            self.animation.move_down_frame(item.row())
            self.save_animation()
            self.move_down_data_item(item)

    def get_selected_item(self):
        index = self.table_view.currentIndex()
        item = self.model.itemFromIndex(index)
        return item

    def save_animation(self):
        self.manager["animations"].save(self.animation_name, self.animation)

    def show_error_popup(self, message):
        error_dialog = QErrorMessage()
        error_dialog.showMessage(message)
        error_dialog.exec_()

# -----------------------------------------------------------------------------
class animation_edit_pane(QWidget):
    def __init__(self, parent, manager, animation_name, animation):
        super().__init__(parent)

        vbox = QVBoxLayout()
        vbox.addWidget(frames_table(self, manager, animation_name, animation))
        vbox.addWidget(QLabel("Player", self))
        self.setLayout(vbox)
