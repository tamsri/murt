from murt.tracer import MurTracer
from murt.utils import objreader

from time import sleep
from time import time
import numpy as np


def TracerTest():
    print("------------ Tracer Test -------------")
    a = MurTracer("./assets/poznan.obj", window=True)
    #a.trace([1, 2, 1], [2, 3, 4])
    txPos = [5, 3, 0]
    rxPos = [0, 1.5, -7]
    avg_time = 0
    for i in range(1):
        start = time()
        a.trace_draw(txPos, rxPos)
        end = time()
        avg_time += end-start
        rxPos = [np.random.random()*200-100, 3, np.random.random()*200-100]
    print("Average Tracing Time: " + str(avg_time))
    a.window.run()


if __name__ == "__main__":
    TracerTest()
