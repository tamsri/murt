import numpy as np
from murt.utils import objreader
from pyglet.gl import *
from pywavefront import Wavefront, visualization


class Object():
    def __init__(self, file_path):
        self.object = Wavefront(file_path, collect_faces=True)
        self.scale, self.translate, self.rotate = [
            1, 1, 1], [0, 0, 0], [0, 0, 0]
        self.vertices, self.triangles = objreader.read(file_path)

    def get_triangles(self):
        return self.vertices, self.triangles

    def resize(self, x, y, z):
        self.scale = (float(x), float(y), float(z))

    def reposition(self, x, y, z):
        self.translate = (float(x), float(y), float(z))

    def rotate_obj(self, x, y, z):
        self.rotate = (float(x), float(y), float(z))

    def draw(self):
        glPushMatrix()
        # Rotation
        # glRotatef(self.rotate[0], 1.0, 0.0, 0.0) #unused
        # glRotatef(self.rotate[2], 0.0, 0.0, 1.0) #unused
        #glTranslatef(0, 0, 0)

        # Translation
        glTranslatef(*self.translate)
        glRotatef(self.rotate[1], 0.0, 1.0, 0.0)

        # Scale
        glScalef(*self.scale)

        # Visualise
        visualization.draw(self.object)

        glPopMatrix()
