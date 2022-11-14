# Frutibandas

<p align=center style="font-size:150%;">
Bringing the game Frutibandas back to life.<br><br>

[https://bicyclette.itch.io/frutibandas](https://bicyclette.itch.io/frutibandas)
<br><br>

<img alt="home" src="screen_capture/home_page.png">
<br><br>
<img alt="game" src="screen_capture/game_page.png">
</p>

## Getting started

### Dependencies

- [Conan](https://conan.io/)
- [Cmake](https://cmake.org/)

## Build

```
conan install . -s build_type=Release --build missing --install-folder=build
cmake -B build -S .
cmake --build build --config Release
```

## Credit
<p align=center>
The original game was made by the Motion Twin.<br><br>
<a href="https://motion-twin.com/fr/">
<img alt="Motion Twin" src="logo/motion_twin.png" width="200">
</a>
</p>
