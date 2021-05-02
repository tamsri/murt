from murt import Tracer

# Scene File Path in obj
OBJ_FILE_PATH = "./assets/poznan.obj"

# Initialize Tracer
my_tracer = Tracer(OBJ_FILE_PATH)
# Set transmiting position
tx_pos = [0, 15, 0]
# Set receiving position
rx_pos = [-30, 1.5, 45]

# Return the traced paths
results = my_tracer.trace(tx_pos, rx_pos)

print(results)
