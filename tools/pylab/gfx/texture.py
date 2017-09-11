import json

class texture:
    def __init__(self, image_path):
        self.image_path = image_path

    def save(self, path):
        with open(path, 'w+') as f:
            json_text = json.dumps({
                "image": self.image_path
            })
            f.write(json_text)

def load_json(json_path):
    with open(json_path, 'r') as f:
        content = f.read()
        json_data = json.loads(content)
        return texture(json_data["image"])
