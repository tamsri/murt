from murt.gui import window
import numpy as np
murt_window = window.MurtWindow()
murt_window.load_scene('./assets/poznan.obj')


while True:
    murt_window.lines_set = [{'points': [(np.random.random()*100, 0, 0), (100, 2, -50), (-100, 4, 20)],
                              'color': (0, 0.4, 0.3, 0.3)}]
    murt_window.custom_render()
