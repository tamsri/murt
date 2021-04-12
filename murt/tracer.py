from murt import core
from murt.window import MurtWindow
from murt.utils import objreader

import numpy as np


class MurTracer():
    def __init__(self, scene_path, window=False):
        vertices, triangles = objreader.read(scene_path)
        self.core = core.Tracer(vertices, triangles)
        if window == True:
            self.window = MurtWindow()

    def trace(self, txPos, rxPos):
        txPos = np.array(txPos)
        rxPos = np.array(rxPos)
        assert rxPos.shape[0] == 3
        assert txPos.shape[0] == 3
        txPos = txPos.astype('float32')
        rxPos = rxPos.astype('float32')
        result = self.core.trace(txPos, rxPos)
        print(result)
