# How to Build the Code

## Linux (tested on openSUSE Tumbleweed)

To build the code, it is necessary to have a GCC toolchain available with at least C++11 support.

The configuration script can be generated using `autoconf`.
The configuration can then be carried out using `./configure`, currently no flags are provided/supported.

The code can be built with:
  - `make all` builds code & libraries.
  - `make code` builds only the code
  - `make libs` builds only the library (intel placeholder and odepack)

The build files can be cleaned up using:
   - `make clean_libs` to remove the libraries
   - `make clean_code` to remove only the code related files (keep the libraries)
   - `make clean_all` to remove all built files

## Windows

While the original version was built on windows using a MinGW implementation, building on Windows has been deprecated.
However it should be possible to build the code on Windows, with the caveat that it is untested and officially unsupported.
