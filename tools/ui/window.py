import tkinter as tk
import tkinter.ttk as ttk
from tkinter.filedialog import askopenfilename, askdirectory

from ui import navigation_pane
from ui import texture_pane
from pylab.data import manifest
from pylab.data import manager

class window(tk.Frame):
    """The main window of the lab-data-editor program."""
    def __init__(self, parent, *args, **kwargs):
        tk.Frame.__init__(self, parent, *args, **kwargs)
        self.root = parent
        self.edit_frame = None
        self.init_gui()

    def on_quit(self):
        quit()

    def on_new(self):
        filename = askdirectory(parent=self.root)
        self.manager = manager.create(filename)
        self.init_edition_gui()
        self.navigation_pane.set_manager(self.manager)

    def on_load(self):
        filename = askdirectory(parent=self.root)
        self.manager = manager.load(filename)
        self.init_edition_gui()
        self.navigation_pane.set_manager(self.manager)

    def create_menu_bar(self):
        self.root.option_add("tearOff", "FALSE")
        self.menu_bar = tk.Menu(self.root)

        self.menu_file = tk.Menu(self.menu_bar)
        self.menu_file.add_command(label="New data folder", command=self.on_new)
        self.menu_file.add_command(label="Load data folder", command=self.on_load)
        self.menu_file.add_command(label="Save manifest")
        self.menu_file.add_command(label="Exit", command=self.on_quit)
        self.menu_bar.add_cascade(menu=self.menu_file, label='File')

        self.root.config(menu=self.menu_bar)

    def emplace_edit_frame(self, frame):
        if (self.edit_frame != None):
            self.edit_frame.destroy()
        self.edit_frame = frame
        frame.grid(row=0, column=1, sticky=(tk.W, tk.N, tk.E, tk.S))

    def start_texture_edition(self, name):
        texture = self.manager.textures.load(name)
        texture_edit_pane = texture_pane.texture_pane(self, texture)
        self.emplace_edit_frame(texture_edit_pane)

    def edit_data(self, event):
        edited_element = self.navigation_pane.get_data_name_at(event.x, event.y)
        parent = self.navigation_pane.get_data_parent_at(event.x, event.y)
        if (parent == self.navigation_pane.textures_node):
            self.start_texture_edition(edited_element)

    def init_edition_gui(self):
        self.navigation_pane = navigation_pane.navigation_pane(self)
        self.navigation_pane.grid(row=0, column=0, sticky=(tk.W, tk.N, tk.E, tk.S))

        main_panel = tk.Label(self, text="main panel")
        self.emplace_edit_frame(main_panel)

        self.columnconfigure(0, weight=1)
        self.columnconfigure(1, weight=4)
        self.rowconfigure(0, weight=1)
        self.grid(row=0, column=0, sticky=(tk.W, tk.N, tk.E, tk.S))
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        self.navigation_pane.bind('<Button-1>', self.edit_data)

    def init_gui(self):
        """Builds GUI."""
        self.root.title("Lab data manifest editor")
        self.create_menu_bar()


