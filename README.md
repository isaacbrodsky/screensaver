# Development

## VCPKG requirements

```
libpng:x86-windows                                 1.6.37-7         libpng is a library implementing an interface fo...
sdl2-image:x86-windows                             2.0.5            SDL_image is an image file loading library. It l...
sdl2:x86-windows                                   2.0.12           Simple DirectMedia Layer is a cross-platform dev...
zlib:x86-windows                                   1.2.11-6         A compression library
```
## Configure CMake

```
cmake -S . -B build2 -DCMAKE_TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake -A Win32
```

