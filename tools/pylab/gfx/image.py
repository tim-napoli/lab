import json
from pylab.math import point

class image:
    def __init__(self, textures, hot_point):
        self.textures = textures
        self.hot_point = hot_point

    def save(self, path):
        with open(path, 'w+') as f:
            content = json.dumps({
                "textures": self.textures,
                "hot_point": self.hot_point.to_json()
            })
            f.write(content)

def load_json(json_path):
    with open(json_path, 'r') as f:
        content = f.read()
        data = json.loads(content)
        textures = data["textures"]
        hot_point = point.load_json(data["hot_point"])
        return image(textures, hot_point)

