from murt.core import Tracer
from murt.utils import objreader

import tracemalloc
import numpy as np


def CallCore():
    print("Loading the obj file...")
    vertices, triangles = objreader.read('./assets/poznan.obj')
    print("Creating ray tracer....")
    tracer = Tracer(vertices, triangles)
    print(f'#id : {tracer.getId()}')


def main():
    print("------------ Call Core Started -------------")
    CallCore()
    print("------------ Call Core Ended -------------")


if __name__ == '__main__':
    main()
