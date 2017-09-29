import os
import os.path
import shutil

from pylab.data import manifest
from pylab.gfx import texture
from pylab.gfx import image
from pylab.gfx import animation
from pylab.math.vec2 import vec2

# sub_manager -----------------------------------------------------------------
class sub_manager:
    def __init__(self, path, manifest_node, template_object, load_func):
        self.path = path
        self.manifest_node = manifest_node
        self.template_object = template_object
        self.load_func = load_func

    def create(self, name):
        self.manifest_node.add(name)
        self.template_object.save('{}/{}'.format(self.path, name))
        return name

    def rename(self, previous_name, new_name):
        os.rename(
            '{}/{}'.format(self.path, previous_name),
            '{}/{}'.format(self.path, new_name)
        )
        self.manifest_node.rename(previous_name, new_name)

    def delete(self, name):
        os.remove('{}/{}'.format(self.path, name))
        self.manifest_node.delete(name)

    def load(self, name):
        return self.load_func('{}/{}'.format(self.path, name))

    def load_all(self):
        """Returns every data inventoried by the referenced manifest_node
        in the format (name, data).
        """
        return [
            (data, self.load(data))
            for data in self.manifest_node.data_list
        ]

    def save(self, name, data):
        data.save('{}/{}'.format(self.path, name))

# textures_manager ------------------------------------------------------------
class textures_manager(sub_manager):
    def __init__(self, path, manifest_node):
        super().__init__(path, manifest_node, None, None)

    def create(self, texture_path):
        texture_basename = os.path.basename(texture_path)
        shutil.copyfile(
            texture_path,
            '{}/{}'.format(self.path, texture_basename)
        )
        self.manifest_node.add(texture_basename)
        return texture_basename

    def load(self, name):
        """Returns the absolute path of the given texture."""
        return '{}/{}'.format(self.path, name)

    def load_all(self):
        return [
            (texture, self.load(texture))
            for texture in self.manifest_node.data_list
        ]

# manager ---------------------------------------------------------------------
class manager:
    """The data manager is a class that will help to manage the
    project data directory.
    """
    def __init__(self, path, manifest):
        self.path = path
        self.manifest = manifest

        self.managers = {
            "textures": textures_manager(
                path + "/textures", manifest.get_node("textures")
            ),
            "images": sub_manager(
                path + "/images", manifest.get_node("images"),
                image.image([], vec2(0, 0), vec2(0, 0)), image.load_json
            ),
            "animations": sub_manager(
                path + "/animations", manifest.get_node("animations"),
                animation.animation([]), animation.load_json
            )
        }

    def __getitem__(self, key):
        return self.managers[key]

# -----------------------------------------------------------------------------
def create(path):
    """Creates every data folder and initialize an empty manifest
    file.
    """
    if (not os.path.exists(path)):
        os.mkdir(path)

    manifest_path = path + '/manifest.json'
    if (os.path.exists(manifest_path)):
        raise ValueError('{} is already a lab data folder'.format(path))


    textures_path = path + '/textures/'
    images_path = path + '/images/'
    animations_path = path + '/animations/'

    os.mkdir(textures_path)
    os.mkdir(images_path)
    os.mkdir(animations_path)

    nodes = {
        "textures": manifest.manifest_node("textures", []),
        "images": manifest.manifest_node("images", []),
        "animations": manifest.manifest_node("animations", [])
    }
    manif = manifest.manifest(manifest_path, nodes)
    manif.save()
    return manager(path, manif)

def load(path):
    "Load a data folder."""
    if (not os.path.exists(path)):
        raise ValueError('{} doesn\'t exist'.format(path))

    manif_path = path + '/manifest.json'
    if (not os.path.exists(manif_path)):
        raise ValueError('{} is not a data folder'.format(path))

    manif = manifest.load_json(manif_path)
    return manager(path, manif)

