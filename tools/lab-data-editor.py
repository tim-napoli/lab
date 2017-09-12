import sys

from PyQt5.QtWidgets import QApplication

import pylab
import qtui

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = qtui.window.window()
    sys.exit(app.exec_())
