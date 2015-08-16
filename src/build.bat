@echo off

mkdir ..\..\build\dolphintas
pushd ..\..\build\dolphintas
cl -FC -Zi -EHsc -MDd ..\..\dolphintas\src\main.cpp -I ../../_tools/SFML-2.3/include^
 -link User32.lib Gdi32.lib^
 -LIBPATH:../../_tools/SFML-2.3/lib winmm.lib sfml-system-d.lib opengl32.lib gdi32.lib sfml-window-d.lib freetype.lib jpeg.lib sfml-graphics-d.lib

popd