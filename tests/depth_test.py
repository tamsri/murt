from murt import Tracer
from matplotlib import pyplot as plt
from murt.utils.generator import SceneGenerator
from murt.window import Window

RANDOM_SEED = 9999
my_scene = SceneGenerator(RANDOM_SEED)

my_scene.generate()
my_tracer = Tracer()
my_tracer.load_scene(*my_scene.get_triangles())
frame = my_scene.get_depth_map(-100, 100, 500, -100, 100, 500)

depth_data = frame.pivot('x', 'z', 'depth')
depth_map = depth_data.T
print(f"depth shape: {depth_map.shape}")
plt.imshow(depth_map, cmap='inferno', interpolation='nearest')
plt.colorbar()
plt.axis('off')
plt.show()

my_window = Window()
my_window.scene = my_scene.get_scene()
my_window.run()
