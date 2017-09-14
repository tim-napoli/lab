from PyQt5.QtWidgets import (
    QWidget, QListView, QLabel, QHBoxLayout, QVBoxLayout, QPushButton, QDialog,
    QGraphicsView, QGraphicsScene
)
from PyQt5.QtGui import (
    QStandardItem, QStandardItemModel, QPixmap, QPen, QColor, QBrush
)
from PyQt5.QtCore import QRectF, Qt, QEvent, pyqtSignal

from qtui import textures_edit_pane

# textures_list ---------------------------------------------------------------
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
    textures_changed = pyqtSignal()

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

    def move_up_texture_item(self, item):
        row = item.row()
        if row > 0:
            self.model.takeRow(row)
            self.model.insertRow(row - 1, item)
            index = self.model.indexFromItem(item)
            self.list_view.setCurrentIndex(index)

    def move_down_texture_item(self, item):
        row = item.row()
        if row < self.model.rowCount() - 1:
            self.model.takeRow(row)
            self.model.insertRow(row + 1, item)
            index = self.model.indexFromItem(item)
            self.list_view.setCurrentIndex(index)

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
        up_button.clicked.connect(self.move_up_texture)
        down_button = QPushButton("Move down", self)
        down_button.clicked.connect(self.move_down_texture)

        self.button_bar = QWidget(self)
        hbox = QHBoxLayout()
        hbox.addWidget(add_button)
        hbox.addWidget(remove_button)
        hbox.addWidget(up_button)
        hbox.addWidget(down_button)
        self.button_bar.setLayout(hbox)

    def add_texture(self):
        textures = self.manager["textures"].load_all()
        dialog = texture_popup(textures)
        if dialog.show() == QDialog.Accepted:
            texture = dialog.get_result()
            self.add_texture_item(texture)
            self.image.add_texture(texture)
            self.manager["images"].save(self.image_name, self.image)
            self.textures_changed.emit()

    def remove_texture(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        if item != None:
            texture = item.text()
            self.remove_texture_item(item)
            self.image.remove_texture(texture)
            self.manager["images"].save(self.image_name, self.image)
            self.textures_changed.emit()

    def move_up_texture(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        if item != None:
            self.move_up_texture_item(item)
            texture = item.text()
            self.image.move_up_texture(texture)
            self.manager["images"].save(self.image_name, self.image)
            self.textures_changed.emit()

    def move_down_texture(self):
        index = self.list_view.currentIndex()
        item = self.model.itemFromIndex(index)
        if item != None:
            self.move_down_texture_item(item)
            texture = item.text()
            self.image.move_down_texture(texture)
            self.manager["images"].save(self.image_name, self.image)
            self.textures_changed.emit()

# hot_point_editor ------------------------------------------------------------
class hot_point_scene(QGraphicsScene):
    clicked = pyqtSignal(float, float)

    def __init__(self):
        super().__init__()

    def mousePressEvent(self, evt):
        self.clicked.emit(evt.scenePos().x(), evt.scenePos().y())

class hot_point_editor(QGraphicsView):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        self.manager = manager
        self.image = image
        self.image_name = image_name
        self.hot_point = None
        self.pixmap = None

        self.scene = hot_point_scene()
        self.draw_pixmap()
        self.draw_hot_point(image.hot_point)

        self.scene.clicked.connect(self.move_hot_point)
        self.setScene(self.scene)

    def draw_hot_point(self, hot_point):
        pen = QPen()
        pen.setColor(QColor(255, 0, 0))
        brush = QBrush(QColor(255, 0, 0), Qt.SolidPattern)
        self.hot_point = self.scene.addEllipse(
            QRectF(-2, - 2, 4, 4),
            pen, brush
        )
        self.hot_point.setZValue(1)
        self.hot_point.setPos(hot_point.x, hot_point.y)

    def draw_pixmap(self):
        if len(self.image.textures) == 0:
            return
        if self.pixmap != None:
            self.scene.removeItem(self.pixmap)
            self.pixmap = None
        pixmap = QPixmap(self.manager["textures"].load(self.image.textures[0]))
        self.pixmap = self.scene.addPixmap(pixmap)
        self.pixmap.setZValue(0)

    def move_hot_point(self, x, y):
        self.hot_point.setPos(x, y)
        self.scene.update(self.scene.sceneRect())
        self.image.set_hot_point(x, y)
        self.manager["images"].save(self.image_name, self.image)

# -----------------------------------------------------------------------------
class image_edit_pane(QWidget):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        list_view = textures_list(self, manager, image_name, image)
        editor = hot_point_editor(self, manager, image_name, image)

        list_view.textures_changed.connect(editor.draw_pixmap)

        vbox = QVBoxLayout()
        vbox.addWidget(list_view)
        vbox.addWidget(editor)
        self.setLayout(vbox)
