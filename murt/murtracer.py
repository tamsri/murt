from murt import core


class MurTracer():
    def __init__(self, scene):
        vertices, triangles = scene
        self.core = core.Tracer(vertices, triangles)

    def trace(txPos, rxPos):
