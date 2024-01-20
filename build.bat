@echo off

set SDL2_INCLUDE=C:\SDL2_stuff\SDL2-2.28.5\include
set SDL2_LIB=C:\SDL2_stuff\SDL2-2.28.5\lib\x64

set SDL2_IMAGE_INCLUDE=C:\SDL2_stuff\SDL2_image-2.8.2\include
set SDL2_IMAGE_LIB=C:\SDL2_stuff\SDL2_image-2.8.2\lib\x64

if not exist bin mkdir bin
if not exist obj mkdir obj

cl.exe /nologo ^
       /Zi ^
       /DEBUG ^
       /I"%SDL2_INCLUDE%" ^
       /I"%SDL2_IMAGE_INCLUDE%" ^
       src\main.cpp ^
       src\texture.cpp ^
       /Febin\game.exe ^
       /Fdbin\game.pdb ^
       /Foobj\ ^
/link  /nologo ^
       /LIBPATH:"%SDL2_LIB%" ^
       /LIBPATH:"%SDL2_IMAGE_LIB%" ^
       SDL2.lib ^
       SDL2main.lib ^
       SDL2_image.lib ^
       Shell32.lib ^
       /INCREMENTAL:NO ^
       /SUBSYSTEM:CONSOLE

copy res\arrows.bmp bin\
copy res\hello_world.bmp bin\
copy res\frog.png bin\

echo Build completed.
