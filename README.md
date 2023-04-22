# Dactyl CC Keyboard

This keyboard and associated libraries take heavy inspiration from the
 [dactyl](https://github.com/adereth/dactyl-keyboard) and
the [dactyl_manuform](https://github.com/abstracthat/dactyl-manuform) keyboards.
 Major thanks to Matthew Adereth and Tom Short for their work.

The dactyl-cc keyboard takes a different approach to the structure of the code (and language)
 as well as being more similar in feel to the Kinesis Advantage 2.

If you are interested in a smaller version see the [mini](https://github.com/mjohns/dactyl-cc-mini) version.

If you are interested in building it you can take a look at the [guide](guide/README.md). Any dactyl build guide you choose should work like [this one.](https://medium.com/swlh/complete-idiot-guide-for-building-a-dactyl-manuform-keyboard-53454845b065)

![image](https://imgur.com/CUbPLZC.jpg)

CMake is the preferred way to build and leads to the fastest recompilation times.
```
cd build
./build.sh
```

If you do not have cmake installed you can run the simple build script which just uses g++.
```
cd build
./build_simple.sh
```

You can generate an stl from the command line with the following command:
```
cd build
// openscad -o ../things/left.stl left.scad
make_things.sh
```

The external holder cutout design is taken from https://github.com/cykedev/dactyl-cc and is designed to for loligagger's external holder.

Loligagger's external holder files:

* [Promicro V1](https://web.archive.org/web/20220607031919/https://dactyl.siskam.link/loligagger-external-holder-promicro-v1.stl)
* [Promicro V2](https://web.archive.org/web/20220607031910/https://dactyl.siskam.link/loligagger-external-holder-promicro-v2.stl)
* [Elite-C V1](https://web.archive.org/web/20220607031927/https://dactyl.siskam.link/loligagger-external-holder-elite-c-v1.stl)

v2 changes:
* Smoothed out back walls and walls around thumb cluster.
* Merged https://github.com/cykedev/dactyl-cc for external holder.
* Adjusted 'g' colum rotation from 10 to 4 degrees.

v1 stl files are archived in the things folder. The source code can be viewed on the v1 git branch.

![image](https://imgur.com/IP2UYYA.jpg)
![image](https://imgur.com/LvZQUT4.jpg)
![image](https://imgur.com/0AYtoey.jpg)

[other images](https://imgur.com/a/jBbUXNx)
