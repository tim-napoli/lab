from PyQt5.QtWidgets import (
    QWidget, QListView, QLabel, QHBoxLayout, QVBoxLayout, QPushButton, QDialog,
    QGraphicsView, QGraphicsScene, QLineEdit, QGridLayout
)
from PyQt5.QtGui import (
    QStandardItem, QStandardItemModel, QPixmap, QPen, QColor, QBrush
)
from PyQt5.QtCore import QRectF, Qt, QEvent, pyqtSignal

from glm import vec2
from qtui import textures_edit_pane
from qtui.textures_edit_pane import texture_miniature
from qtui.miniature_selection_popup import miniature_selection_popup
from qtui.data_list import data_list

def get_image_original_dimensions(manager, image):
    if len(image.textures) == 0:
        return vec2(0, 0)
    else:
        pixmap = QPixmap(
            manager["textures"].load(image.textures[0])
        )
        return vec2(pixmap.width(), pixmap.height())

# textures_list ---------------------------------------------------------------
class texture_popup(miniature_selection_popup):
    def __init__(self, textures):
        super().__init__("Texture selection", [], 5)
        for texture in textures:
            self.add_miniature(texture_miniature(self, *texture))

class textures_list(data_list):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent, image.textures)
        self.manager = manager
        self.image_name = image_name
        self.image = image

    def add_data(self):
        textures = self.manager["textures"].load_all()
        dialog = texture_popup(textures)
        if dialog.show() == QDialog.Accepted:
            texture = dialog.get_result().texture
            self.image.add_texture(texture)
            if len(self.image.textures) == 1:
                self.image.set_dimensions(
                    get_image_original_dimensions(self.manager, self.image)
                )
            self.manager["images"].save(self.image_name, self.image)
            return texture

    def remove_data(self):
        texture = self.get_selected_data()
        self.image.remove_texture(texture)
        self.manager["images"].save(self.image_name, self.image)
        return True

    def move_up_data(self):
        texture = self.get_selected_data()
        self.image.move_up_texture(texture)
        self.manager["images"].save(self.image_name, self.image)
        return True

    def move_down_data(self):
        texture = self.get_selected_data()
        self.image.move_down_texture(texture)
        self.manager["images"].save(self.image_name, self.image)
        return True

# dimensions_editor -----------------------------------------------------------

class dimensions_editor(QWidget):
    def __init__(self, parent, manager, image_name, image):
        super().__init__(parent)

        self.manager = manager
        self.image = image
        self.image_name = image_name

        vbox = QVBoxLayout()

        grid = QGridLayout()
        grid_widget = QWidget(self)
        self.width_input = QLineEdit(grid_widget)
        self.height_input = QLineEdit(grid_widget)
        grid.addWidget(QLabel("Width", grid_widget), 0, 0);
        grid.addWidget(self.width_input, 0, 1)
        grid.addWidget(QLabel("Height", grid_widget), 1, 0);
        grid.addWidget(self.height_input, 1, 1)
        grid_widget.setLayout(grid)

        hbox = QHBoxLayout()
        hbox_widget = QWidget(self)
        reset_button = QPushButton("Reset", hbox_widget)
        reset_button.clicked.connect(self.reset)
        save_button = QPushButton("Save", hbox_widget)
        save_button.clicked.connect(self.save)
        hbox.addWidget(save_button)
        hbox.addWidget(reset_button)
        hbox_widget.setLayout(hbox)

        vbox.addWidget(grid_widget)
        vbox.addWidget(hbox_widget)

        self.setLayout(vbox)

        self.update_dimensions_input()

    def show_error_popup(self, message):
        error_dialog = QErrorMessage()
        error_dialog.showMessage(message)
        error_dialog.exec_()

    def update_dimensions_input(self):
        self.width_input.setText(str(self.image.dimensions.x))
        self.height_input.setText(str(self.image.dimensions.y))

    def update(self):
        self.update_dimensions_input()

    def reset(self):
        dimensions = get_image_original_dimensions(self.manager, self.image)
        self.image.set_dimensions(dimensions)
        self.update_dimensions_input()

    def save(self):
        try:
            width = int(self.width_input.text())
            height = int(self.height_input.text())
        except Exception as ex:
            self.show_error_message("Values must be integers")
        self.image.set_dimensions(vec2(width, height))
        self.manager["images"].save(self.image_name, self.image)

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
        dimensions = dimensions_editor(self, manager, image_name, image)
        editor = hot_point_editor(self, manager, image_name, image)

        list_view.data_changed.connect(editor.draw_pixmap)
        list_view.data_changed.connect(dimensions.update)

        vbox = QVBoxLayout()
        vbox.addWidget(list_view)
        vbox.addWidget(dimensions)
        vbox.addWidget(editor)
        self.setLayout(vbox)
