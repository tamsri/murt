clean:
	rm -r build
	rm -r dist
	python setup.py clean --all
	pip uninstall murt -y
all:
	python setup.py install
	pip install .
test:
	python ./tests/core_tests.py
	python ./tests/tracer_test.py
	python ./tests/calculator_tests.py
	python ./tests/generator_test.py
	python ./tests/randomer_test.py
	python ./tests/depth_test.py
tai:
	make all
	make test
dev:
	make clean
	make tai