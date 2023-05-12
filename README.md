# SWAN

Attempting to make a neural network library for C, while still learning it and having no idea how it works. Quality not guaranteed. Also, this is mainly made for personal use, so don't go complaining about things like "unhelpful error messages" that insult you about your mistakes.

TODO
- [x] Loss calculations
- [ ] sofmax activatation function (it's hard okay)
- [ ] backprogation why... it is hard (yes true)

~~compile command (because Sweder has small brain): `gcc src/*.c src/*/*.c -o Swan.exec -Wall -Wextra -Wpedantic`~~

To generate build files, run prebuild.* from the project root directory (Swan/, not to be confused with src/Swan/).

To build, run cmake --build

Final executable should be called from the project root, to get correct file paths (i.e. from Swan/: `./build/src/main`)