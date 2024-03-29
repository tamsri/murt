from murt import Tracer
from murt.utils.generator import SceneGenerator
import numpy as np
import time
import plotly.graph_objects as go

SEED = 100
my_scene = SceneGenerator(SEED)
my_scene.generate()

my_tracer = Tracer()
my_tracer.load_scene(*my_scene.get_triangles())

txPos = [0, -1, 0]
while my_tracer.is_outdoor(txPos) is not True:
    txPos = [np.random.random()*200-100,
             np.random.random()*10+2,
             np.random.random()*200-100]

start = time.time()
data_frame_1 = my_tracer.get_traced_volume(x_min=-150, x_max=150, x_n=10,
                                           y_min=1, y_max=50, y_n=10,
                                           z_min=-150, z_max=150, z_n=10,
                                           tx_pos=txPos, tx_freq=2.4e9, mat_perm=5.3,
                                           max_thread=64)
end = time.time()
print(f"The multithread tracing engine takes {end-start:.2f} seconds")
print(data_frame_1.head)
