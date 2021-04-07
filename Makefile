clean:
	rm -r build
	python setup.py clean
all:
	python setup.py install
test:
	python ./murt/tests/core_tests.py
tai:
	make all
	make test
dev:
	make clean
	make tai