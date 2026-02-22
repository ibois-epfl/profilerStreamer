To run the executable in its current form, please:
- Install [cmake](https://cmake.org/)
- Install [git](https://git-scm.com/)
- On Windows (currently the only platform supported), install [Visual Studio 2026](https://visualstudio.microsoft.com/fr/)

Once the necessary tools installed, please run in the root of the repository: 
```bash
mrdir build
cd build
cmake -G "Visual Studio 18 2026" -A x64 -DCMAKE_BUILD_TYPE=Debug -S ../ -B ./ # or "Visual Studio 17 2022"
cmake --build .
```

Once the previous commands successfully run, you should have a `build/Debug` folder in which the executable is located, the OXAPI<...>.dll and Open3D.dll in the `build` folder. At that point you can execute the code from the build folder:
```bash
.\Debug\profilerStreamerTest.exe
```