from murt.window import MurtWindow
from murt import tracer, scene_generator
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
        scene_gen = scene_generator(self.generator.integers(0, 1000000))
        scene_gen.generate()
        self.scene = scene_gen.sceneComponents
        vertices, indice = scene_gen.get_triangles()
        self.tracer.load_scene(vertices, indice)
        self.lines_set = []
        for i in range(1):
            tx = (0.0, -3.0, 0.0)
            rx = (0.0, -3.0, 0.0)
            while self.tracer.is_outdoor(tx) is not True:
                tx = (self.generator.uniform(-150, 150),
                      self.generator.uniform(1.5, 2),
                      self.generator.uniform(-150, 150))

            while self.tracer.is_outdoor(rx) is not True:
                rx = (self.generator.uniform(-150, 150),
                      self.generator.uniform(2, 7),
                      self.generator.uniform(-150, 150))

            results = self.tracer.trace(tx, rx)
            lines = self.tracer.result_to_lines(results, tx, rx)
            self.lines_set += lines

    def render(self):
        super().render()
