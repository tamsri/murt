import numpy as np
import pandas as pd

from murt.utils.object import Object
from murt import Tracer


class SceneGenerator():
    def __init__(self, seed=0, component_path=None):
        self.sceneComponents = []
        self.component_path = component_path
        self.x_min = -140
        self.x_max = 140
        self.z_min = -140
        self.z_max = 140

        self.generator = np.random.default_rng(seed)
        self.globalVertices, self.globalIndice = None, None

    def generate(self, center_outdoor=False):
        if center_outdoor is False:
            self.generate_scene()
        else:
            mini_tracer = Tracer()
            self.generate_scene()
            mini_tracer.load_scene(*self.get_triangles())
            tx_pos = [0.0, 0.1, 0.0]
            while mini_tracer.is_outdoor(tx_pos) is not True:
                self.generate_scene()
                mini_tracer.load_scene(*self.get_triangles())

    def generate_scene(self):
        self.globalVertices = None
        self.globalIndice = None
        self.sceneComponents = []
        # Generate ground
        ground = Object('ground', directory_path=self.component_path)
        self.sceneComponents.append(ground)
        # Generate buildings
        n_building = max(int(self.generator.normal(25, 10)), 2)
        n_building = min(n_building, 40)
        cube_percentage = 0.5
        for i in range(n_building):
            type_build = self.generator.random()
            if type_build < cube_percentage:
                building = self.generate_object(obj_name="cube",
                                                h_min=7, h_max=25, w_min=7,
                                                w_max=max(
                                                    self.generator.normal(20, 5), 15),
                                                d_min=7, d_max=30)
            else:
                building = self.generate_object(obj_name="house",
                                                h_min=5, h_max=20, w_min=7, w_max=15,
                                                d_min=7, d_max=20)
            self.sceneComponents.append(building)
        # Calculate triangles
        self.prepare_triangles()
        return

    def generate_object(self, obj_name=None, h_min=5,
                        h_max=30, w_min=7, w_max=30,
                        d_min=6, d_max=30):
        if obj_name is None:
            obj_name = 'cube'

        building = Object(obj_name, directory_path=self.component_path)

        # Generate Scale
        x_scale = self.generator.uniform(w_min,
                                         w_max)

        y_scale = self.generator.uniform(h_min,
                                         h_max)

        z_scale = self.generator.uniform(d_min,
                                         d_max)

        building.resize(x_scale, y_scale, z_scale)

        # Generate Position
        x = self.generator.uniform(self.x_min, self.x_max)
        z = self.generator.uniform(self.z_min, self.z_max)
        building.reposition(x, 0, z)

        # Generate Rotation

        if self.generator.random() < 0.5:
            rotate = 0
        else:
            rotate = self.generator.normal(90, 3)

        if self.generator.random() < 0.5:
            rotate += 90
        building.rotate_obj(0, rotate, 0)
        return building

    def get_scene(self):
        return self.sceneComponents

    def get_triangles(self):
        return self.globalVertices, self.globalIndice

    def prepare_triangles(self):
        self.globalVertices, self.globalIndice = None, None
        for component in self.sceneComponents:
            vertices, indexes = component.get_triangles()
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
                self.globalIndice = np.append(
                    self.globalIndice, indexes, axis=0)

            # add vertices
            if self.globalVertices is None:
                self.globalVertices = np.array(vertices)
            else:
                self.globalVertices = np.append(
                    self.globalVertices, vertices, axis=0)

    def get_depth_map(self, x_min, x_max, x_n, z_min, z_max, z_n):
        assert x_min < x_max
        assert z_min < z_max
        assert (x_n > 0) and (z_n > 0)

        current_tracer = Tracer()
        current_tracer.load_scene(*self.get_triangles())

        upper_y = 1000
        below_y = -1000
        data_depth = []
        data_frame = None

        for current_x in np.linspace(x_min, x_max, x_n):
            for current_z in np.linspace(z_min, z_max, z_n):
                from_pos = [current_x, upper_y, current_z]
                to_pos = [current_x, below_y, current_z]
                distance = current_tracer.hit_nearest(
                    from_pos, to_pos)
                # in case if there is no ground, skip
                if distance < 0:
                    depth = 0
                else:
                    depth = upper_y-distance
                data_row = {'x': current_x, 'z': current_z, 'depth': depth}
                # print(distance)
                data_depth.append(data_row)

        data_frame = pd.DataFrame(data_depth)
        return data_frame
