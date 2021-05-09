import numpy as np
from murt.utils import objreader
import os
import sys
COMPONENT_PATH = os.path.join(sys.prefix, "murt-assets")


class Object():
    def __init__(self, object_name, file_path=None, directory_path=None):
        self.object_name = object_name
        self.scale, self.translate, self.rotate = \
            [1, 1, 1], [0, 0, 0], [0, 0, 0]
        if file_path is None:
            try:
                if directory_path is None:
                    file_path = os.path.join(
                        COMPONENT_PATH, f'{object_name}.obj')
                else:
                    file_path = os.path.join(
                        directory_path, f'{object_name}.obj')
                self.vertices, self.triangles = objreader.read(file_path)
            except:
                print("Unknown file, please enter the file path for object.")

    def get_triangles(self):
        return self.vertices, self.triangles

    def resize(self, x, y, z):
        self.scale = (float(x), float(y), float(z))

    def reposition(self, x, y, z):
        self.translate = (float(x), float(y), float(z))

    def rotate_obj(self, x, y, z):
        self.rotate = (float(x), float(y), float(z))
