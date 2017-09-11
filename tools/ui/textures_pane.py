import tkinter as tk

from ui import textures_grid

class textures_pane(tk.Frame):
    """Edit panel that shows every textures in the project."""
    def __init__(self, parent, manager):
        tk.Frame.__init__(self, parent)
        textures = manager.textures.load_all()
        self.texture_grid = textures_grid.textures_grid(self, textures)
