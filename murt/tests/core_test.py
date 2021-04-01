from murt import core
import tracemalloc

print("Memory Leak Test")
test_n = 10000

leak = 0
tracemalloc.start()
start, _ = tracemalloc.get_traced_memory()
for i in range(test_n):
    a = core.Tracer()
    tracer_id = a.getid()
    del a
end, _ = tracemalloc.get_traced_memory()
leak = (end-start)
tracemalloc.stop()
assert (leak < 1), "Memory should not be leaked"
print("Passed leak tests")
