This ia a basic chemical kinetics solver for chemkin style mechanisms, employed in the thesis 
titled "Autoxidation Behaviour of Hydrocarbons in the Context of Conventional and Alternative 
Aviation Fuels",submitted at the University of Leeds in 2015.

This solver draws on the Intel ODE library for solving the appropriate ordinary differential 
equations. however in theory any other solver may be used to solve the system of ODEs. I have 
done my best to use self contained functions, hence modifying the code should not be too 
difficult - though it may be tedious to step through some of my comments.

The library is NOT included with the distribution and has to be downloaded separately from the
Intel Website as the EULA disallows distribution of the library iteself:
https://software.intel.com/en-us/articles/intel-ordinary-differential-equations-solver-library
Should the link "go dead", it is advised to search for "Intel ODE solver" with the help of a 
search engine.

The code was developed using Eclipse CDT using the included builder - if you would like to 
build the code yourself, you will require a copy of GCC (Linux) or MinGW/TDM-GCC (Win) 
with Eclipse CDT.

The included binary in the release folder should work on most 64Bit PCs, however a speedup
can be achieved by compiling the code for the employed architecture with -march=native.
Where the supplied binary does not work, it is recommended to compile it from source.