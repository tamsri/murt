from murt import scene_generator, mwindow, tracer
from matplotlib import pyplot as plt

RANDOM_SEED = 9999
my_scene = scene_generator(RANDOM_SEED)
for i in range(3):
    my_scene.generate()
    my_tracer = tracer()
    my_tracer.load_scene(*my_scene.get_triangles())
    frame = my_scene.get_depth_map(-100, 100, 500, -100, 100, 500)

    depth_data = frame.pivot('x', 'z', 'depth')
    depth_map = depth_data.T
    print(f"depth shape: {depth_map.shape}")
    plt.imshow(depth_map, cmap='inferno', interpolation='nearest')
    plt.colorbar()
    plt.axis('off')
    plt.show()

my_window = mwindow()
my_window.scene = my_scene.get_scene()
my_window.run()
