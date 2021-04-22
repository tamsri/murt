clean:
	rm -r build
	rm -r dist
	python setup.py clean --all
	pip uninstall murt -y
all:
	python setup.py install
	pip install .
test:
	#python ./murt/tests/core_tests.py
	#python ./murt/tests/tracer_test.py
	#python ./murt/tests/calculator_tests.py
	#python ./murt/tests/generator_test.py
	python ./murt/tests/randomer_test.py
tai:
	make all
	make test
dev:
	make clean
	make tai