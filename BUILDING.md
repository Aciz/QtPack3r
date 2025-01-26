# Building from source

## Windows

Download and install [Qt](https://www.qt.io/download-dev). If you're planning on building with Visual Studio or another IDE using msbuild, make sure you select MSVC kit in the installer.

For working outside of Qt Creator, it is highly recommended to add the Qt binary directory to your `PATH` environmental variable. By default, the path is `C:\Qt\<version>\<toolkit>\bin`.

If Qt is not part of `PATH`, configuring the project outside of Qt Creator fails unless you use [CMAKE_PREFIX_PATH](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html) to point CMake to the Qt installation directory. Running the application requires you to copy over the required DLL's to the applications directory, or using `windeployqt` to automate the process:
```
C:\Qt\<version>\<toolkit>\bin\windeployqt.exe <path\to\QtPack3r.exe>
```

### Visual Studio
1. Generate Visual Studio solution with CMake.
```sh
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
```
2. Open `QtPack3r.sln` in Visual Studio and build the project.

### Qt Creator
1. Open the `CMakeLists.txt` with Qt Creator.
2. Configure the project for a kit you have installed (MSVC or MinGW).
3. Build the project.

## Linux

Install Qt6 package using your distributions package manager. Debian-based distributions also need to install OpenGL development packages.

* Arch and derivatives
```sh
$ sudo pacman -S qt6-base
```

* Debian and derivatives
```sh
$ sudo apt install qt6-base-dev libglx-dev libgl1-mesa-dev
```

### Using make
```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=release
make
```

### Qt Creator
1. Open the `CMakeLists.txt` with Qt Creator.
2. Configure the project for a kit you have installed (GCC by default).
3. Build the project.