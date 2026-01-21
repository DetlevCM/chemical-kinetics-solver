# A Chemical Kinetics Solver

This is a basic chemical kinetics solver for chemkin style mechanisms, employed in the thesis
titled "Autoxidation Behaviour of Hydrocarbons in the Context of Conventional and Alternative
Aviation Fuels",submitted at the University of Leeds in 2015.
[http://etheses.whiterose.ac.uk/10407/](http://etheses.whiterose.ac.uk/10407/)

Initially developed using the Intel ODE solver, this has now been deprecated as a solver, and LSODA should be used instead.

The recommended solver is odepack,
developed by Alan C. Hindmarsh and Linda R. Petzold.
As of writing this readme, the source code is freely available on the
LLNL website: [https://computing.llnl.gov/projects/odepack/software](https://computing.llnl.gov/projects/odepack/software)

Under some conditions, the solver can be unstable.
This can be resolved by running the first calculations for very small time steps.
For example 10 steps at 1e.-26s up to 1e-25s before switching to the desired time step.
This should not change the result in any significant manner.

The chemical kinetics solver was originally developed using Eclipse CDT using the included builder.
This has since been migrated to the user of makefiles.
It used to compile on both Windows and Linux. With the inclusion of odepack, only Linux is
officially supported.
However, you are free to build the code under Windows too,
which may be possible if you built the odepack archive with static links.

To build the code, both a C++ compiler and a Fortran compiler are required.
C++11 needs to be supported as a minimum required.
