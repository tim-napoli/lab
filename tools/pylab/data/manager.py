import os
import os.path
import shutil
from PIL import Image, ImageTk

from pylab.data import manifest
from pylab.gfx import texture

class textures_manager:
    TEXTURE_DIRECTORY = 'textures'

    def __init__(self, data_path, manifest):
        self.path = '{}/{}'.format(data_path, self.TEXTURE_DIRECTORY)
        self.manifest = manifest

    def create(self, texture_path):
        texture_basename = os.path.basename(texture_path)
        shutil.copyfile(
            texture_path,
            '{}/{}'.format(self.path, texture_basename)
        )
        self.manifest.add_texture(texture_basename)
        self.manifest.save()
        return texture_basename

    def rename(self, previous_name, new_name):
        os.rename(
            '{}/{}'.format(self.path, previous_name),
            '{}/{}'.format(self.path, new_name)
        )
        self.manifest.rename_texture(previous_name, new_name)
        self.manifest.save()

    def delete(self, name):
        self.manifest.delete_texture(name)
        os.remove('{}/{}'.format(self.path, name))
        self.manifest.save()

    def load(self, name):
        return ImageTk.PhotoImage(Image.open('{}/{}'.format(self.path, name)))

    def load_all(self):
        """Load every textures available in the data folder. Returns for
        each texture a couple (name, image).
        """
        return [
            (texture, self.load(texture))
            for texture in self.manifest.textures
        ]

class manager:
    """The data manager is a class that will help to manage the
    project data directory.
    """
    def __init__(self, path, manifest):
        self.path = path
        self.manifest = manifest
        self.textures = textures_manager(path, manifest)

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

    manif = manifest.manifest(manifest_path, [], [], [])
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

