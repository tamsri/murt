clean:
	rm -r build
	python setup.py clean
get:
	python setup.py install
test:
	python ./murt/tests/core_tests.py
tai:
	make get
	make test
dev:
	make clean
	make tai