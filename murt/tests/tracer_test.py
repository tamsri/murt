from murt.tracer import MurTracer
from murt.utils import objreader


def TracerTest():
    print("------------ Tracer Test -------------")
    a = MurTracer("./assets/poznan.obj")
    a.trace([1, 2, 1], [2, 3, 4])


if __name__ == "__main__":
    TracerTest()
