# IPv4Prefix
Program uses red-black tree structure underneath to store ipv4 prefixes.
It allows you to add, delete and check if given IP addres match any previously added prefix.
If several prefixes match, then "check" function will pick the one with the biggest mask, and its value.
Function declarations are placed in ipv4prefix.h

# Build
Project is based on CMake, so in order to properly build it, follow steps below:

1. Navigate to root directory
```
cmake -S ./ -B ./build
```
2. Move to build dir
```
make
```
3. To run UT
```
cd test
./UT_Prefix
```
4. To run main
```
build/
./prefix
```
NOTE: Building requires downloading GTest - it's done automatically during Cmake building

#
