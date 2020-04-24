# Dactyl CC Keyboard

This keyboard and associated libraries take heavy inspiration from the
 [dactyl](https://github.com/adereth/dactyl-keyboard) and
the [dactyl_manuform](https://github.com/abstracthat/dactyl-manuform) keyboards.
 Major thanks to Matthew Adereth and Tom Short for their work.

The dactyl-cc keyboard takes a different approach to the structure of the code (and language)
 as well as being more similar in feel to the Kinesis Advantage 2.

If you are interested in a smaller version see the [mini](https://github.com/mjohns/dactyl-cc-mini) version.

![image](https://imgur.com/IP2UYYA.jpg)

cmake is the preferred way to build and leads to the fastest recompilation times.
```
cd build
cmake ../src
make && ./dactyl
```

If you do not have cmake installed you can run the simple build script which just uses g++.
```
cd build
./build_simple.sh
```

You can generate an stl from the command line with the following command:
```
cd build
openscad -o ../things/left.stl left.scad
```

![image](https://imgur.com/LvZQUT4.jpg)
![image](https://imgur.com/0AYtoey.jpg)
