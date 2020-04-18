```
// Build. Requires g++.
./build.sh.

// To change key.cc/transform.cc/scad.cc do a clean build. These slow
// down the build time a significant amount.
./build.sh --clean

// To generate STL from command line:
openscad -o things/left.stl left.scad
```
