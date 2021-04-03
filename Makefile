clean:
	rm -r build
	python setup.py clean
get:
	python setup.py install
test:
	python ./murt/tests/call_core.py 