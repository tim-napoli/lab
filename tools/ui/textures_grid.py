import tkinter as tk

class texture_miniature(tk.Frame):
    """A texture miniature is an object that shows a texture image upon it's
    name in the manifest.
    """
    def __init__(self, parent, name, image):
        tk.Frame.__init__(self, parent)
        self.init_gui(name, image)

    def init_gui(self, name, image):
        self.title = tk.Label(self, text=name)
        self.image = tk.Label(self, image=image)
        self.image.image = image
        self.title.grid(row=1, column=0, sticky='NESW')
        self.image.grid(row=0, column=0, sticky='NESW')
        self.columnconfigure(0, weight=1)
        self.rowconfigure(1, weight=2)
        self.rowconfigure(0, weight=6)

class textures_grid(tk.Frame):
    """The texture grid is a special widget that, given a list of textures,
    shows them in a grid of the specified dimensions.
    """
    def __init__(self, parent, textures):
        tk.Frame.__init__(self, parent)
        self.miniatures = self.create_miniatures(textures)
        self.organize_miniatures()
        self.pack()

    def create_miniatures(self, textures):
        return [
            texture_miniature(self, *texture_data)
            for texture_data in textures
        ]

    def organize_miniatures(self):
        current_row = 0
        current_col = 0
        for miniature in self.miniatures:
            miniature.grid(row=current_row, column=current_col, sticky='NESW')
            self.columnconfigure(current_col, weight=1)
            current_col = current_col + 1
            if current_col % 5 == 0:
                current_col = 0
                self.rowconfigure(current_row, weight=1)
                current_row = current_row + 1
