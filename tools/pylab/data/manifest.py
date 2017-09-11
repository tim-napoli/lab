import json

class manifest:
    def __init__(self, path, textures, images, animations):
        self.path = path
        self.textures = textures
        self.images = images
        self.animations = animations

    def add_texture(self, name):
        self.textures.append(name)

    def rename_texture(self, previous_name, new_name):
        self.textures.remove(previous_name)
        self.textures.append(new_name)

    def delete_texture(self, name):
        self.textures.remove(name)

    def to_json(self):
        data = {
            'textures': self.textures,
            'images': self.images,
            'animations': self.animations
        }
        return json.dumps(data)

    def save(self):
        with open(self.path, 'w+') as f:
            f.write(self.to_json())

def load_json(json_file_path):
    """Load a JSON file containing a lab data manifest file."""
    with open(json_file_path, 'r') as f:
        json_text = f.read()
        json_data = json.loads(json_text)
        return manifest(
            json_file_path,
            json_data["textures"],
            json_data["images"],
            json_data["animations"]
        )
