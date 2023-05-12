# SWAN

Attempting to make a neural network library for C, while still learning it and having no idea how it works. Quality not guaranteed. Also, this is mainly made for personal use, so don't go complaining about things like "unhelpful error messages" that insult you about your mistakes.

TODO
- [x] Loss calculations
- [ ] sofmax activatation function (it's hard okay)
- [ ] backprogation why... it is hard (yes true)

~~compile command (because Sweder has small brain): `gcc src/*.c src/*/*.c -o Swan.exec -Wall -Wextra -Wpedantic`~~

To generate build files, execute the `prebuild.*` script in `./scripts` for your corresponding platform (.bat for windows, .sh for unix-based systems)

To build, execute the `build.*` script in the same folder for your corresponding platform.

This will generate a `build` directory in the project root, where all cmake files will be located. The scripts in `./scripts` use Make (or mingw32-make)
as their default build tool. To alter this, edit the `G` flag in the `cmake` command in `prebuild.*`. Allowed options are listed here: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html

All links under "CMake Generators" (scroll down) are valid options in plain text form surrounded by quotation marks (i.e. `cmake -G "Visual Studio 17 2022" .`)

The build commands are located in separate scripts as opposed to running from bare cli, as this makes it easier to dispose of cmake files afterwards (files are generated in a separate `build` directory which can just be deleted at any time if needed).