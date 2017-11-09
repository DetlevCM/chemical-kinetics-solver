This is a basic chemical kinetics solver for chemkin style mechanisms, employed in the thesis 
titled "Autoxidation Behaviour of Hydrocarbons in the Context of Conventional and Alternative 
Aviation Fuels",submitted at the University of Leeds in 2015.
http://etheses.whiterose.ac.uk/10407/

This solver draws on the Intel ODE library for solving the appropriate ordinary differential 
equations. however in theory any other solver may be used to solve the system of ODEs. I have 
done my best to use self contained functions, hence modifying the code should not be too 
difficult - though it may be tedious to step through some of my comments.

The library is NOT included with the distribution and has to be downloaded separately from the
Intel website as the EULA disallows distribution of the library itself:
https://software.intel.com/en-us/articles/intel-ordinary-differential-equations-solver-library
Should the link "go dead", it is advised to search for "Intel ODE solver" with the help of a 
search engine.

A later expansion is seeking to implement the odepack library, developed by Alan C. Hindmarsh 
and Linda R. Petzold. As of writing this readme, the source code is freely availabl on the 
 LLNL website. (As well as on other websites.)


The chemical kinetics solver was originally developed using Eclipse CDT using the included builder.
It used to compile on both Windows and Linux. With the inclusion of odepack, only Linux is 
officially supported. (It is easier.) However, you are free to build the code under Windows too, 
which may be possible if you built the odepack archive with static links.

To build the code, both a C++ compiler and a Fortran compiler are required.
C++11 needs to be supported as a minimum required.


Codacy Badge
