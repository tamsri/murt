import numpy as np
import sys
import os
import logging
import pywavefront
from pywavefront import Wavefront
from murt.utils import objreader
from murt.object import Object
COMPONENT_PATH = os.path.join(sys.prefix, "murt-assets")

pywavefront.configure_logging(
    logging.ERROR,
    formatter=logging.Formatter('%(name)s-%(levelname)s: %(message)s')
)


class SceneGenerator():
    def __init__(self, seed=0):
        self.cube_path = os.path.join(COMPONENT_PATH, "cube.obj")
        self.house_path = os.path.join(COMPONENT_PATH, "house.obj")
        self.ground_path = os.path.join(COMPONENT_PATH, "ground.obj")
        self.sceneComponents = []

        self.x_min = -140
        self.x_max = 140
        self.z_min = -140
        self.z_max = 140

        self.generator = np.random.default_rng(seed)

        self.globalVertices, self.globalIndice = None, None

    def Generate(self):
        # Generate ground
        ground = Object(self.ground_path)
        self.sceneComponents.append(ground)
        # Generate buildings
        n_building = max(int(self.generator.normal(20, 5)), 2)
        n_building = min(n_building, 35)
        cube_percentage = 0.5
        for i in range(n_building):
            type_build = self.generator.random()
            if type_build < cube_percentage:
                building = self.GenerateObj(obj_path=self.cube_path)
            else:
                building = self.GenerateObj(obj_path=self.house_path,
                                            h_min=3, h_max=15, w_min=7, w_max=15,
                                            d_min=7, d_max=20)
            self.sceneComponents.append(building)
        # Calculate triangles

    def GenerateObj(self, obj_path=None, h_min=5,
                    h_max=30, w_min=7, w_max=30,
                    d_min=6, d_max=30):
        if obj_path is None:
            obj_path = self.cube_path

        building = Object(obj_path)

        # Generate Scale
        x_scale = self.generator.uniform(w_min,
                                         w_max)

        y_scale = self.generator.uniform(h_min,
                                         h_max)

        z_scale = self.generator.uniform(d_min,
                                         d_max)

        building.Resize(x_scale, y_scale, z_scale)

        # Generate Position
        x = self.generator.uniform(self.x_min, self.x_max)
        z = self.generator.uniform(self.z_min, self.z_max)
        building.Reposition(x, 0, z)

        # Generate Rotation
        rotate = self.generator.normal(90, 5)
        if self.generator.random() < 0.5:
            rotate += 90
        building.Rotate(0, rotate, 0)
        return building

    def GetScene(self):
        return self.sceneComponents

    def GetTriangles(self):
        self.globalVertices, self.globalIndice = None, None

        for component in self.sceneComponents:
            vertices, indexes = component.GetTriangles()

            # scale
            vertices *= component.scale

            # rotate (yaw)
            angle = np.deg2rad(component.rotate[1])
            cos_angle = np.cos(-angle)
            sin_angle = np.sin(-angle)
            R_mat = np.array([[cos_angle, 0, sin_angle],
                              [0, 1, 0],
                              [-sin_angle, 0, cos_angle]])
            vertices = np.matmul(vertices, R_mat)
            # translate
            vertices += component.translate

            # add indice
            if self.globalIndice is None:
                self.globalIndice = np.array(indexes)
            else:
                indexes += self.globalVertices.shape[0]
                # print(
                #    f'index {indexes.shape}\nglobal: {self.globalIndice.shape}')
                self.globalIndice = np.append(
                    self.globalIndice, indexes, axis=0)

            # add vertices
            if self.globalVertices is None:
                self.globalVertices = np.array(vertices)
            else:
                self.globalVertices = np.append(
                    self.globalVertices, vertices, axis=0)

        return self.globalVertices, self.globalIndice
