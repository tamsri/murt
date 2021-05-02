from murt import Tracer

my_tracer = Tracer()

tx_pos = [0, 15, 0]
rx_pos = [-30, 1.5, 45]
tx_freq = 2.4e9

scene_permitivity = 5.3  # Concrete

result = [(2, [(-19.24, 8.56, 28.86)]),
          (3, (-28.94, 4.22, 62.39)),
          (3, (-70.80, 7.04, 15.22))]

losses = my_tracer.get_total_loss(
    tx_pos, rx_pos, result, tx_freq, scene_permitivity)

print(losses)
