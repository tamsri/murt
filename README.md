# MURT

Multipath Radio Tracer (MURT) is a ray-tracing engine for multipath propagation of radio wave. MURT is a Python package which consits of ray-tracing engine, visualisation, path loss calculator, and the customized OpenAI Gym for machine learning implementations.

---

## Get Started

MURT consists of multiple module. Each module can be called and used separately.

### Ray Tracing Core

The core engine only accepts the triangulated obj files to the tracing process.

```Python
from murt import tracer

# Scene File Path in obj
SCENE_FILE_PATH =  "scene_file_path.obj"

# Initialize Tracer
my_tracer = tracer(SCENE_FILE_PATH)
# Set transmitting position
tx_pos = [ 0, 15, 0]
# Set receiving position
rx_pos = [-30, 1.5, 45]

# Return the traced paths
result = my_tracer.trace(tx_pos, rx_pos)
# [(2, [(-19.24, 8.56, 28.86)]), (3, (-28.94, 4.22, 62.39)), (3, (-70.80, 7.04, 15.22))]
```

The result is the list of traced paths. The first position of the tuple indicates types of the paths (1 - direct path, 2 - diffracted path, 3 - reflected path). Tbe second position of the tuple is the detail of the traced paths.

### Calculate Path Loss

As the possible paths are traced from the engine. the results from the tracer can be used to

```Python
tx_freq = 2.4e9

scene_permittivity = 5.3  # Concrete

result = [(2, [(-19.24, 8.56, 28.86)]),
          (3, (-28.94, 4.22, 62.39)),
          (3, (-70.80, 7.04, 15.22))]

losses = my_tracer.TotalPathLoss(
    tx_pos, rx_pos, result, tx_freq, scene_permittivity)
# {'total_dB': 84.3, 'signals': [[95.50, 1.86e-07], [86.63, 2.91e-07], [89.27, 4.12e-07]]}
```
The tracer calculates the total path loss according to the theoretical propagation models. In this example, the scene material is considered to be concrete.  
### Visualiser

```

```

#### Random

```


```
