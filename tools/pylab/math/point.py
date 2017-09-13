import json

class point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def to_json(self):
        return {
            "x": self.x,
            "y": self.y
        }

def load_json(json_content):
    return point(json_content["x"], json_content["y"])

