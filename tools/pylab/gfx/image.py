import json
from pylab.math import point

class image:
    def __init__(self, textures, hot_point, dimensions):
        self.textures = textures
        self.hot_point = hot_point
        self.dimensions = dimensions

    def add_texture(self, texture):
        self.textures.append(texture)

    def remove_texture(self, texture):
        self.textures.remove(texture)

    def move_up_texture(self, texture):
        """Move a texture to the front of the texture list.
        Return True is the texture is effectively moved.
        """
        index = self.textures.index(texture)
        if index > 0:
            self.textures.pop(index)
            self.textures.insert(index - 1, texture)
            return True
        return False

    def move_down_texture(self, texture):
        """Move a texture to the front of the texture list.
        Return True is the texture is effectively moved.
        """
        index = self.textures.index(texture)
        if index < len(self.textures) - 1:
            self.textures.pop(index)
            self.textures.insert(index + 1, texture)
            return True
        return False

    def set_hot_point(self, x, y):
        self.hot_point.x = x
        self.hot_point.y = y

    def set_dimensions(self, dimensions):
        self.dimensions = dimensions

    def save(self, path):
        with open(path, 'w+') as f:
            content = json.dumps({
                "textures": self.textures,
                "hot_point": self.hot_point.to_json(),
                "dimensions": self.dimensions.to_json()
            })
            f.write(content)

def load_json(json_path):
    with open(json_path, 'r') as f:
        content = f.read()
        data = json.loads(content)
        textures = data["textures"]
        hot_point = point.load_json(data["hot_point"])
        dimensions = point.load_json(data["dimensions"])
        return image(textures, hot_point, dimensions)

