from murt import Tracer
from murt.window import Window

from time import sleep
from time import time
import numpy as np


def TracerTest():
    print("------------ Tracer Test -------------")
    a = Tracer("./assets/poznan.obj")
    window = Window()
    window.load_scene('./assets/display_poznan.obj')
    # a.trace([1, 2, 1], [2, 3, 4])

    avg_time = 0
    window.lines_set = []
    n = 100
    start = time()
    for i in range(n):
        tx_pos = [5, 10, 0]
        rx_pos = [0, -1.5, -7]
        while a.is_outdoor(tx_pos) is not True:
            tx_pos = [np.random.random()*200-100, np.random.random() *
                      4+1.5, np.random.random()*200-100]
        while a.is_outdoor(rx_pos) is not True:
            rx_pos = [np.random.random() * 200 - 100, np.random.random() +
                      1.5, np.random.random() * 200 - 100]
        results = a.trace(tx_pos, rx_pos)
        window.lines_set += a.result_to_lines(results, tx_pos, rx_pos)
        losses = a.get_total_loss(tx_pos=tx_pos, rx_pos=rx_pos,
                                  tx_freq=2.4e9, results=results)
        print(f'{results} \n losses: {losses} \n---------------------\n')
        # print(f'Is {rx_pos} outdoor? -> answer: {a.isOutdoor(rx_pos)}')
    end = time()
    duration = end - start
    print(f"Traced {n} points, Total Time: {duration}")
    window.run()
    print("------------ Tracer Finisehd -------------")


if __name__ == "__main__":
    TracerTest()
