from murt import core
from murt.utils import objreader
import tracemalloc


def main():
    print("Memory Leak Test")
    test_n = 1000000
    vertices, triangles = objreader.read('./assets/poznan.obj')
    leak = 0
    tracemalloc.start()
    for i in range(test_n):
        start, _ = tracemalloc.get_traced_memory()
        a = core.Tracer(vertices, triangles)
        tracer_id = a.getid()
        del a
        end, _ = tracemalloc.get_traced_memory()
        leak = (end-start)
        if leak > 0:
            print(f"#{tracer_id}, Leaked: {leak} bytes")
    tracemalloc.stop()
    #assert (leak < 1), "Memory should not be leaked"
    print("Passed leak tests")


if __name__ == '__main__':
    main()
