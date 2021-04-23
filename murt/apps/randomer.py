from murt.window import MurtWindow
from murt import tracer
from murt.generator import SceneGenerator
from pyglet.window import key
from pyglet import text
import numpy as np


class RandomRunner(MurtWindow):
    def __init__(self, seed=9999):
        super().__init__()
        self.tracer = tracer()
        self.generator = np.random.default_rng(seed)
        self.regenerate_scene()

    def on_key_release(self, pressed_key, modifier):

        if pressed_key == key.G:
            self.regenerate_scene()
        super().on_key_release(pressed_key, modifier)

    def regenerate_scene(self):
        scene_gen = SceneGenerator(self.generator.integers(0, 1000000))
        scene_gen.Generate()
        self.scene = scene_gen.sceneComponents
        vertices, indice = scene_gen.GetTriangles()
        self.tracer.loadTracer(vertices, indice)
        tx = (0.0, 3.0, 0.0)
        self.lines_set = []
        for i in range(200):
            rx = (self.generator.uniform(-150, 150),
                  self.generator.uniform(1.5, 2),
                  self.generator.uniform(-150, 150))
            results = self.tracer.trace(tx, rx)
            lines = self.tracer.ResultsToLines(results, tx, rx)
            self.lines_set += lines

    def render(self):
        super().render()

