from murt import Tracer
from murt.window import Window
SCENE_PATH = "./assets/poznan.obj"

# load window and scene
my_window = Window()
my_window.load_scene(SCENE_PATH)

# trace
tx_pos = [0, 15, 0]
rx_pos = [-30, 1.5, 45]
result = [(2, [(-19.24, 8.56, 28.86)]),
          (3, (-28.94, 4.22, 62.39)),
          (3, (-70.80, 7.04, 15.22))]


# convert results to visualised paths
my_window.lines_set += Tracer.result_to_lines(result, tx_pos, rx_pos)

# run
my_window.run()
