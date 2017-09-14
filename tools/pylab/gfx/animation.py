import json

class animation_frame:
    def __init__(self, image, tick_duration):
        self.image = image
        self.tick_duration = tick_duration

    def to_json(self):
        return {
            "image": self.image,
            "tick_duration": self.tick_duration
        }

class animation:
    def __init__(self, frames):
        self.frames = frames

    def to_json(self):
        return {
            "frames": [frame.to_json() for frame in self.frames]
        }

    def save(self, path):
        with open(path, 'w+') as f:
            f.write(json.dumps(self.to_json()))

def load_json(json_path):
    with open(json_path, 'r') as f:
        content = json.loads(f.read())
        frames = [
            animation_frame(frame["image"], frame["tick_duration"])
            for frame in content["frames"]
        ]
        return animation(frames)

