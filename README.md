<img src="./data/images/labengine-logo.md" alt="" />
# The Lab Engine Library

The Lab Engine Library is a small C++ library intended to ease games
development using the C++ language.

## A word about the Lab Engine Philosophy

The Lab Engine library is intended to be simple.

## Dependencies

### Programs

        g++ with C++11 support
        cmake
        python3

### C++ dependencies

        libglfw3-dev
        libsoil-dev
        libglm-dev
        cxxtest

### Python dependencies

        pyqt5
        pyqt5.qtopengl
        PyGLM (install with pip3 : sudo pip3 install PyGLM --pre)

## Installation

    After having installed every dependencies, clone the repository:

        git clone --recursive https://github.com/tim-napoli/lab

    I advise you to create a specific python environment for the project:

        python3 -m venv pyenv
        source pyenv/bin/activate
        # Install dependencies using pip

    Next, you just need to build the project using CMake:

        mkdir build
        cd build
        cmake ..
        make -j4

    Easy !

