![image](https://i.redd.it/zhrj9a4d1gt41.jpg)

The main entry point is dactyl.cc. All of the key positioning is in key_data.*.

```
// Build. Requires g++.
./build.sh.

// To change key.cc/transform.cc/scad.cc do a clean build. These slow
// down the build time a significant amount.
./build.sh --clean

// To generate STL from command line:
openscad -o things/left.stl out/left.scad
```

