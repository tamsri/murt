from murt.tracer import MurTracer
from murt.utils import objreader
from murt.window import MurtWindow

from time import sleep
from time import time
import numpy as np


def TracerTest():
    print("------------ Tracer Test -------------")
    a = MurTracer("./assets/poznan.obj")
    window = MurtWindow()
    window.load_scene('./assets/display_poznan.obj')
    #a.trace([1, 2, 1], [2, 3, 4])
    txPos = [5, 3, 0]
    rxPos = [0, 1.5, -7]
    avg_time = 0
    window.lines_set = []
    n = 100
    start = time()
    for i in range(n):
        results = a.trace(txPos, rxPos)
        window.lines_set += a.ResultsToLines(results, txPos, rxPos)
        rxPos = [np.random.random()*200-100, 3, np.random.random()*200-100]
    end = time()
    duration = end-start
    print(f"Traced {n} points, Total Time: {duration}")
    window.run()
    print("------------ Tracer Finisehd -------------")


if __name__ == "__main__":
    TracerTest()
