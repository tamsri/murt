from murt.tracer import MurTracer
from murt.utils import objreader


def TracerTest():
    print("------------ Tracer Test -------------")
    a = MurTracer("./assets/triple_edge.obj", window=True)
    #a.trace([1, 2, 1], [2, 3, 4])
    txPos = [5, 3, 0]
    rxPos = [20, 3, 0]
    a.trace_draw(txPos, rxPos)


if __name__ == "__main__":
    TracerTest()
