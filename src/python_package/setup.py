
from setuptools import setup, find_packages
import os
__version__ = open(os.path.join(os.path.dirname(__file__), "../../version")).read().strip()

setup(
    name="profiler_streamer",
    version=__version__,
    packages=find_packages(),
    install_requires=[
        "numpy==2.0.2",
        # other dependencies...
    ],
    description="profiler_streamer is a python package to stream profiler and rangefinder data, and post-process it.",
    long_description=open("../../README.md").read(),
    long_description_content_type="text/markdown",
    author="Damien Gilliard",
    author_email="damien.gilliard@epfl.ch",
    url="https://github.com/ibois-epfl/profilerStreamer",
    classifiers=[
        "License :: OSI Approved :: GPL-3.0 License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.9",
    ],
    include_package_data=True,
    package_data={  # type: ignore[misc]
        "profiler_streamer": ["bindings/*.dll", "bindings/*.pyd"]
    },
)
