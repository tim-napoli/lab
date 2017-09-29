import json

from .vec2 import vec2

def load_vec2_json(json_content):
    return vec2(json_content["x"], json_content["y"])

def build_vec2_json(v):
    return {
        "x": v.x,
        "y": v.y
    }


