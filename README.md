# Frutibandas

<p align=center style="font-size:150%;">
Bringing the game Frutibandas back to life.<br>

<p align=center>
<a href="https://bicyclette.itch.io/frutibandas">bicyclette.itch.io/frutibandas</a>
</p>


<img alt="home" src="screen_capture/home_page.png">
<br><br>
<img alt="game" src="screen_capture/game_page.png">
</p>

## Getting started

### Dependencies

- [VCPKG](https://github.com/microsoft/vcpkg)
- [Cmake](https://cmake.org/)

## Build

```
./vcpkg install enet freetype glew libsndfile sdl2 openal-soft nlohmann-json
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:\dev\vcpkg\scripts\buildsystems\vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Credit
<p align=center>
The original game was made by the Motion Twin.<br><br>
<a href="https://motion-twin.com/fr/">
<img alt="Motion Twin" src="logo/motion_twin.png" width="200">
</a>
</p>
