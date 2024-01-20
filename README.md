# SDL2 Notes

A little SDL2 project to be used as a reference.

Most of this code was from following [Lazy Foo's tutorial](https://lazyfoo.net/tutorials/SDL/).

![gif](./gif.gif)

# Build

Download and extract [SDL2-devel-2.28.5-VC.zip](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5) and [SDL2_image-devel-2.8.2-VC.zip](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.2) to `C:\SDL2_stuff\`. Update the versions and paths in `build.bat` if yours are different.

Open `x64 Native Tools Command Prompt for VS 2022` and navigate to the project.

Run `.\build.bat` to build a binary to `.\bin\game.exe`.
