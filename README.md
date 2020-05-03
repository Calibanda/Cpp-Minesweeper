# Cpp-Minesweeper
C++ game project at SeAMK

How to compile project?
- Read 'SFML Setup in Visual Studio 2019.pdf' and install SFML library
- In both debug and realease configurations change C++ Language Standard to 'ISO C++17 Standard (/std:c++17)'
- In the debug configuration of the linker, click 'Input' and add the additional dependencies: sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib;
- In the release configuration of the linker, click 'Input' and add the additional dependencies: sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib;
