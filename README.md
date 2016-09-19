# SteelDuk (or STL-Duk)

SteelDuk (a play on its original name, STL-Duk) is a bunch of C++ helper functions for working with the [Duktape](duktape.org) ECMAScript interpreter, and a scripting environment based on those functions.

The project is usable though not intended as a complete project.


## License

Except for the third-party code used in the project, all of the code is dual-licensed under the MIT license and my custom public domain-esque modification of the MIT license. Use whichever one you want.

In short: do everything you want with it. You don't have to credit me since I don't feel like I've created anything that anyone else couldn't do with a short amount of time. However, I am not responsible for any damages.


## Building SteelDuk

To build SteelDuk, CMake is recommended. However, all third-party dependencies are included in the project and it should be possible to build SteelDuk using other C++ build tools.

SteelDuk should be compatible with any relatively modern C++ compiler, though it is only tested with Visual C++ 2013 since that is what I am currently using.