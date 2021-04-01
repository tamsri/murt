from murt import core
from murt.utils import objreader
import numpy as np


def main():
    print("Loading the obj file...")
    vertices, triangles = objreader.read('./assets/poznan.obj')
    pritnt("Creating ray tracer....")
    tracer = core.Tracer(vertices, triangles)
    print(f'#id : {tracer.getid()}')


if __name__ == '__main__':
    main()
