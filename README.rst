=======
Blowgun
=======

Blowgun is a 3D graphics library that designed to run on Windows, Linux,
and Android.
In order to achieve that, it's written using C++ and on top of OpenGLES 2.


Building
--------

Basically, Blowgun only needs two things:

* CMake_
* C++11-capable compiler.


Windows
~~~~~~~

On Windows, Blowgun is tested working using MSVC 10.0
(Shipped with Visual C++ 2010).

Steps to build:

1. Open command prompt and then change directory to ``builder\cmake``.
2. Make directory junction for ``resources``.
   You can do that using command ``mklink /d /j resources ..\..\resources``.
3. Run ``cmake . && make``.


Linux
~~~~~

On Linux, Blowgun is tested working using:
* GCC 4.7
* Clang 3.1

Steps to build:

1. Open your favorite terminal and then change directory to ``builder/cmake``.
2. Make symbolic link for ``resources``.
   Just to refresh your memory, you can do it
   by executing ``ln -s ../../resources resources``.
3. Run ``cmake . && make``.


Android
~~~~~~~

TODO


.. _CMake: http://cmake.org
