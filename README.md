# polygons

Small simple game engine utilizing OpenGL

## Building and Running

Project uses CMake to create build files. Install [CMake](https://cmake.org/) and follow the below instructions to build project.

```
$ cd /project/root/
$ mkdir build/
$ cd build/
```

### Linux

```
$ cmake ..
$ make
```

creates an executable as "polygons"

### Mac OS X
(Could use Linux/GNU make for building if desired)
```
$ cmake -G "Xcode" ..
```
creates an XCode project file.

### Windows
```
$ cmake -G "Visual Studio <version> <arch> .."
```

Where <version> is Visual Studio version, and <arch> is Win64, Win32
Creates a Visual Studio solution file.

### how to play
desktop - use mouse to rotate camera, use WASD to move guy forward.
Hit Q to be able to flyaround first person, click mouse to posses guy.
Use mouse while looking at other guy to switch to him (he should be green).
