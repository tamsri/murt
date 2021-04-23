from murt import tracer
from murt.utils import objreader
from murt.window import MurtWindow

from time import sleep
from time import time
import numpy as np


def TracerTest():
    print("------------ Tracer Test -------------")
    a = tracer("./assets/poznan.obj")
    window = MurtWindow()
    window.load_scene('./assets/display_poznan.obj')
    # a.trace([1, 2, 1], [2, 3, 4])
    txPos = [5, -3, 0]
    rxPos = [0, -1.5, -7]
    avg_time = 0
    window.lines_set = []
    n = 1000
    start = time()
    for i in range(n):
        #while a.isOutdoor(txPos) is not True:
        txPos = [np.random.random()*200-100, np.random.random() *
                     4+1.5, np.random.random()*200-100]
        #while a.isOutdoor(rxPos) is not True:
        rxPos = [np.random.random()*200-100, np.random.random() +
                     1.5, np.random.random()*200-100]
        results = a.trace(txPos, rxPos)
        window.lines_set += a.ResultsToLines(results, txPos, rxPos)
        losses = a.TotalPathLoss(txPos=txPos, rxPos=rxPos,
                                 txFreq=2.4e9, results=results)
        print(losses)
        #print(f'Is {rxPos} outdoor? -> answer: {a.isOutdoor(rxPos)}')
    end = time()
    duration = end-start
    print(f"Traced {n} points, Total Time: {duration}")
    window.run()
    print("------------ Tracer Finisehd -------------")


if __name__ == "__main__":
    TracerTest()
