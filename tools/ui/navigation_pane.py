import tkinter as tk
import tkinter.ttk as ttk
from tkinter.filedialog import askopenfilename, askdirectory

from pylab.data import manager
from pylab.data import manifest
from ui import entry_popup

class navigation_pane(ttk.Treeview):
    """Shows every data contained in the loaded manifest.
    Group data per data kind.
    """
    def __init__(self, parent, *args, **kwargs):
        ttk.Treeview.__init__(self, parent, *args, **kwargs)
        self.root = parent
        self.init_gui()
        self.init_tree()
        self.manager = None
        self.entry_popup = None

    def init_tree(self):
        self.textures_node = self.insert('', 'end', text="Textures")
        self.images_node = self.insert('', 'end', text="Images")
        self.animations_node = self.insert('', 'end', text="Animations")

    def set_manager(self, manager):
        self.manager = manager
        self.set_manifest()

    def set_manifest(self):
        for texture in self.manager.manifest.textures:
            self.insert(self.textures_node, 'end', text=texture)

    def refresh_manifest(self):
        if (len(self.get_children(self.textures_node)) > 0):
            self.delete(*self.get_children(self.textures_node))
        if (len(self.get_children(self.images_node)) > 0):
            self.delete(*self.get_children(self.images_node))
        if (len(self.get_children(self.animations_node)) > 0):
            self.delete(*self.get_children(self.animations_node))
        self.set_manifest()

    def import_texture(self):
        texture_path = askopenfilename(title='Select a texture file')
        texture_name = self.manager.textures.create(texture_path)
        self.insert(self.textures_node, 'end', text=texture_name)

    def delete_texture(self):
        self.manager.textures.delete(self.clicked_texture)
        self.clicked_texture = None
        self.refresh_manifest()

    def texture_node_popup_menu(self, event):
        popup = tk.Menu(self.root, tearoff=0)
        popup.add_command(label="Import a texture", command=self.import_texture)
        popup.tk_popup(event.x_root, event.y_root, 0)

    def texture_popup_menu(self, event):
        popup = tk.Menu(self.root, tearoff=0)
        popup.add_command(label="Delete a texture", command=self.delete_texture)
        popup.tk_popup(event.x_root, event.y_root, 0)

    def popup_menu(self, event):
        self.release_rename_popup()
        clicked_element = self.identify('item', event.x, event.y)
        if (clicked_element == self.textures_node):
            self.texture_node_popup_menu(event)
        else:
            parent = self.get_data_parent_at(event.x, event.y)
            if (parent == self.textures_node):
                self.clicked_texture = self.get_data_name_at(event.x, event.y)
                self.texture_popup_menu(event)

    def release_rename_popup(self):
        if (self.entry_popup != None):
            self.entry_popup.destroy()
            self.entry_popup = None
            self.renamed_element = None

    def rename_element(self):
        new_name = self.entry_popup.get()
        if (self.renamed_element_parent == self.textures_node):
            self.manager.textures.rename(self.renamed_element, new_name)
        self.release_rename_popup()
        self.refresh_manifest()

    def get_data_name_at(self, x, y):
        data = self.identify('item', x, y)
        return self.item(data, option='text')

    def get_data_parent_at(self, x, y):
        rowid = self.identify_row(y)
        return self.parent(rowid)

    def get_item_at(self, x, y):
        return self.identify('item', x, y)

    def rename(self, event):
        if (self.entry_popup != None):
            self.release_rename_popup()

        self.renamed_element = self.get_data_name_at(event.x, event.y)

        rowid = self.identify_row(event.y)
        colid = self.identify_column(event.x)
        self.renamed_element_parent = self.parent(rowid)

        if (self.renamed_element_parent == ''):
            return

        x, y, width, height = self.bbox(rowid, colid)
        pady = height // 2

        name = self.item(rowid, 'text')
        self.entry_popup = entry_popup.entry_popup(self, name)
        self.entry_popup.place(x=x, y=y + pady, anchor=tk.W, relwidth=1)

        self.entry_popup.bind('<Return>', lambda *ignore: self.rename_element())


    def init_gui(self):
        self.bind('<Button-3>', self.popup_menu)
        self.bind('<Double-Button-1>', self.rename)

