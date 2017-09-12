from PyQt5.QtWidgets import QMainWindow, QAction, qApp, QFileDialog, QErrorMessage

from qtui import editor_pane
from pylab.data import manager

class window(QMainWindow):
    """Main window of the lab data editor."""
    def __init__(self):
        super().__init__()
        self.manager = None
        self.editor_pane = None

        self.setWindowTitle("Lab Data Editor")
        self.create_menu_bar()
        self.show()

    def create_file_menu(self):
        new_action = QAction('&New data directory', self)
        new_action.setShortcut('Ctrl+N')
        new_action.setStatusTip('Create a new data directory')
        new_action.triggered.connect(self.new_data_directory)

        load_action = QAction('&Load data directory', self)
        load_action.setShortcut('Ctrl+L')
        load_action.setStatusTip('Open an existing data directory')
        load_action.triggered.connect(self.load_data_directory)

        quit_action = QAction('&Exit', self)
        quit_action.setShortcut('Ctrl+Q')
        quit_action.setStatusTip('Exit the editor')
        quit_action.triggered.connect(qApp.quit)

        menubar = self.menuBar()
        filemenu = menubar.addMenu('&File')
        filemenu.addAction(new_action)
        filemenu.addAction(load_action)
        filemenu.addAction(quit_action)

    def create_menu_bar(self):
        self.create_file_menu()

    def remove_editor_pane(self):
        if (self.editor_pane != None):
            self.editor_pane.deleteLater()
            self.setCentralWidget(None)
            self.editor_pane = None

    def create_editor_pane(self):
        self.remove_editor_pane()
        self.editor_pane = editor_pane.editor_pane(self, self.manager)
        self.setCentralWidget(self.editor_pane)

    def new_data_directory(self):
        path = QFileDialog.getExistingDirectory(
            self, 'Open new data directory'
        )
        try:
            self.manager = manager.create(path)
            self.create_editor_pane()
        except ValueError as ex:
            error_dialog = QErrorMessage()
            error_dialog.showMessage(
                'Cannot create data directory: {}'.format(ex)
            )
            error_dialog.exec_()

    def load_data_directory(self):
        path = QFileDialog.getExistingDirectory(
            self, 'Open new data directory'
        )
        try:
            self.manager = manager.load(path)
            self.create_editor_pane()
        except ValueError as ex:
            error_dialog = QErrorMessage()
            error_dialog.showMessage(
                'Cannot load data directory: {}'.format(ex)
            )
            error_dialog.exec_()

