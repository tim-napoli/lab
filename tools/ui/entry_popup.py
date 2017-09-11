import tkinter as tk

class entry_popup(tk.Entry):
    def __init__(self, parent, text, **kw):
        super().__init__(parent, **kw)
        self.insert(0, text)
        self.focus_force()

        self.bind("<Escape>", lambda *ignore: self.destroy())
