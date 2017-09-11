import tkinter as tk

class texture_pane(tk.Label):
    """The texture pane widget is the pane used to edit a texture data."""
    def __init__(self, parent, texture):
        tk.Label.__init__(self, parent, image=texture)
        self.image = texture

