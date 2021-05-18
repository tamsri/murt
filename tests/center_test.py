from murt.utils.generator import SceneGenerator
from murt.engine import Tracer


def test():
    generator = SceneGenerator(7777, component_path="./assets/components")
    test_tracer = Tracer()
    tx_pos = [0, 1, 0]
    hit_something = False
    for i in range(10000):
        generator.generate(center_outdoor=True)
        test_tracer.load_scene(*generator.get_triangles())
        if test_tracer.is_outdoor(tx_pos) is False:
            hit_something = True
    if hit_something:
        print("Center Outdoor teste is not passed")
    else:
        print("Center Outdoor test is passed")


if __name__ == '__main__':
    test()
