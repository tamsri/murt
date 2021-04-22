from setuptools import setup, Extension, find_packages
import numpy as np
import os


data_files = [
    ('murt-assets', [
        'assets/poznan.obj',
     'assets/components/cube.obj', 'assets/components/cube.mtl',
     'assets/components/house.obj', 'assets/components/house.mtl',
     'assets/components/ground.obj', 'assets/components/ground.mtl'])
]


def install():
    core_module = Extension('murt.core',
                            sources=['murt/core/core.cpp'],
                            include_dirs=[np.get_include()],
                            extra_compile_args=['-std=c++17'])

    calculator_module = Extension('murt.calculator',
                                  sources=['murt/core/calculator.cpp'],
                                  extra_compile_args=['-std=c++17'])
    setup(
        name="murt",
        version="0.0.1",
        author="Supawat Tamsri",
        author_email="contact@supawat.dev",
        description="Python Library for Multipath Ray Tracing",
        long_description=open('README.md').read(),
        long_description_content_type="text/markdown",
        license="MIT",
        packages=['murt', 'murt.utils', 'murt.apps'],
        requires=['numpy', 'pyglet', 'pywavefront'],
        ext_modules=[core_module, calculator_module],
        data_files=data_files,
        include_package_data=True
    )


if __name__ == "__main__":
    install()
