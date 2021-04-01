# import setuptools

# with open("README.md", "r") as fi:
#     long_description = fi.read()

# setuptools.setup(
#     name="murt",
#     version="0.0.1",
#     author="Supawat Tamsri",
#     description="Multipath Radio Tracer",
#     long_description = long_description,
#     long_description_content_type = "text/markdown",
#     packages=setuptools.find_packages(),
#     classifiers=[
#         "Programming Language :: Python :: 3",
#         "License :: OSI Approved :: MIT License",
#         "Operating System :: OS Independent",
#     ],
#     python_requires='>=3.6',
#     # TODO[]: implement later!

# )
from distutils.core import setup, Extension
import numpy as np


def main():
    core_module = Extension('murt.core',
                            sources=['murt/core/core.cpp'],
                            include_dirs=[np.get_include()],
                            extra_compile_args=['-std=c++11'])
    setup(
        name="murt",
        version="0.0.1",
        author="Supawat Tamsri",
        author_email="contact@supawat.dev",
        packages=["murt", "murt.utils"],
        ext_modules=[core_module],
    )


main()
