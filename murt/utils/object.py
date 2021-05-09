import numpy as np
from murt.utils import objreader
from pywavefront import Wavefront, visualization
import os
import sys
COMPONENT_PATH = os.path.join(sys.prefix, "murt-assets")


class Object():
    def __init__(self, object_name, file_path=None):
        self.object_name = object_name
        self.scale, self.translate, self.rotate = \
            [1, 1, 1], [0, 0, 0], [0, 0, 0]
        if file_path is None:
            try:
                file_path = os.path.join(COMPONENT_PATH, f'{object_name}.obj')
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

    # def draw(self):
    #     glPushMatrix()
    #     # Rotation
    #     # glRotatef(self.rotate[0], 1.0, 0.0, 0.0) #unused
    #     # glRotatef(self.rotate[2], 0.0, 0.0, 1.0) #unused
    #     #glTranslatef(0, 0, 0)

    #     # Translation
    #     glTranslatef(*self.translate)
    #     glRotatef(self.rotate[1], 0.0, 1.0, 0.0)

    #     # Scale
    #     glScalef(*self.scale)

    #     # Visualise
    #     visualization.draw(self.object)

    #     glPopMatrix()
