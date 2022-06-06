# Frutibandas

<p align=center style="font-size:150%;">
Bringing the game Frutibandas back to life.<br><br>
<img alt="home" src="screen_capture/home_page.png">
</p>

## Getting started

### Dependencies

- [Conan](https://conan.io/)
- [Cmake](https://cmake.org/)

## Build

```
conan install . -s build_type=Debug --build missing --install-folder=build
cmake -B build -S .
```

## Credit
<p align=center>
The original game was made by the Motion Twin.<br>
<a href="https://motion-twin.com/fr/">
<img alt="Motion Twin" src="logo/motion_twin.png" width="200">
</a>
</p>