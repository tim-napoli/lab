import json

class image:
    def __init__(self, img_path, hot_point):
        self.img_path = img_path
        self.hot_point = hot_point

    def to_json(self):
        return "Not implemented yet."

def load_json(text):
    data = json.loads(text)
    print(data)
    path = data["path"]
    hot_point = data["hot_point"]
    return image(path, hot_point)

