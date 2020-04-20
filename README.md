![image](https://i.redd.it/zhrj9a4d1gt41.jpg)

This keyboard and associated libraries takes heavy inspiration from the
 [dactyl](https://github.com/adereth/dactyl-keyboard) and
the [dactyl_manuform](https://github.com/abstracthat/dactyl-manuform).
 Major thanks to Matthew Adereth and Tom Short for their work creating those
 respective keyboards.

```
// Build. Requires g++.
./build.sh.

// If changing files under util you must run a clean build.
./build.sh --clean

// To generate STL from command line:
openscad -o things/left.stl out/left.scad
```
