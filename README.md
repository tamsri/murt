# MURT

Multipath Radio Tracer (MURT) is a ray-tracing engine for multipath propagation of radio wave. MURT is a Python package which consits of ray-tracing engine, visualisation, path loss calculator, and the customized OpenAI Gym for machine learning implementations.

---

## Get Started

MURT consists of multiple tools. Each tool can be called and used separately.

### Ray Tracing Core

The core engine only accepts the triangulated obj files to the tracing process.

```Python
from murt import tracer

# Scene File Path in obj
OBJ_FILE_PATH =  "file_path.obj"

# Initialize Tracer
my_tracer = tracer(OBJ_FILE_PATH)
# Set transmiting position
tx_pos = [ 0, 15, 0]
# Set receiving position
rx_pos = [-30, 1.5, 45]

# Return the traced paths
result = my_tracer.trace(tx_pos, rx_pos)
```

The result is the list of traced paths. The first position of the tuple indicates types of the paths (1 - direct path, 2 - diffracted path, 3 - reflected path).

### Calculate Path Loss

As the possible paths are traced from the engine. the results from the tracer can be used to

```Python


```

### Visualiser

```

```

#### Random

```


```
