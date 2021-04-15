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
    rxPos = [20, 3, 0]
    avg_time = 0
    for i in range(1000):
        start = time()
        a.trace(txPos, rxPos)
        end = time()
        avg_time += end-start
        rxPos = [np.random.random()*100-50, 3, np.random.random()*100-50]
    print("Average Tracing Time:" + str(avg_time/1000))
    # a.window.run()


if __name__ == "__main__":
    TracerTest()
