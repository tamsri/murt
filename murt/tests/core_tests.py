from murt import core
from murt.utils import objreader
import tracemalloc


def CallCore():
    print("Loading the obj file...")
    vertices, triangles = objreader.read('./assets/poznan.obj')
    print("Creating ray tracer....")
    tracer = core.Tracer(vertices, triangles)
    print(f'#id : {tracer.getid()}')


def CheckMemoryLeak():
    test_n = 1000
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


def main():
    print("------------ Call Core Test -------------")
    CallCore()

    #print("----------- Memory Leak Test ------------")
    # CheckMemoryLeak()


if __name__ == '__main__':
    main()
