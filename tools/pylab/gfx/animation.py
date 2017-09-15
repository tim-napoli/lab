import json

class animation_frame:
    def __init__(self, image, tick_duration):
        self.image = image
        self.tick_duration = tick_duration

    def set_duration(self, duration):
        if not isinstance(duration, int):
            raise ValueError("duration must be an integer.")
        self.tick_duration = duration

    def get_duration(self):
        return self.tick_duration

    def to_json(self):
        return {
            "image": self.image,
            "tick_duration": self.tick_duration
        }

class animation:
    def __init__(self, frames):
        self.frames = frames

    def get_frame(self, index):
        return self.frames[index]

    def add_frame(self, frame):
        self.frames.append(frame)

    def remove_frame(self, index):
        del(self.frames[index])

    def move_up_frame(self, index):
        if index == 0:
            return
        frame = self.frames[index]
        del(self.frames[index])
        self.frames.insert(index - 1, frame)

    def move_down_frame(self, index):
        if index >= len(self.frames) - 1:
            return
        frame = self.frames[index]
        del(self.frames[index])
        self.frames.insert(index + 1, frame)

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

