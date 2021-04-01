import numpy as np


def read(filename):
    triangles = []
    vertices = []
    with open(filename) as file:
        for line in file:
            components = line.strip(' \n').split(' ')
            if components[0] == "f":
                indices = list(
                    map(lambda c: int(c.split('/')[0]) - 1, components[1:]))
                for i in range(0, len(indices) - 2):
                    triangles.append(indices[i: i + 3])
            elif components[0] == "v":  #
                vertex = list(map(lambda c: float(c), components[1:]))
                vertices.append(vertex)

    return np.array(vertices), np.array(triangles)
