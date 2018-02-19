
Subject to determining whether I can actually redistribute the odepack source code, 
developed by Alan C. Hindmarsh at LLNL and Linda R. Petzold at the University of 
California, only compile instructions will be supplied for now.
To the best of my knowledge, this is the official source:
https://computation.llnl.gov/casc/odepack/
(Though the code is also mirrored elsewhere.)

The main source code is written using C++, so Fortran code has no direct usability.
The most painless implementation is to compile the Fortran code into a library, to 
then be able to call the required functionality from the main code.

I cannot recommend the best compilation method, however these are number of options I 
have tried - with seemingly identical results regarding runtime...

Create the binary files using either of:

  gfortran -c opkda1.f opkda2.f opkdmain.f

  gfortran -O2 -march=native -c opkda1.f opkda2.f opkdmain.f
  
  gfortran -O3 -march=native -c opkda1.f opkda2.f opkdmain.f

  gfortran -O3 -march=native -ffast-math -c opkda1.f opkda2.f opkdmain.f

  gfortran -O3 -march=native -mavx2 -ffast-math -c opkda1.f opkda2.f opkdmain.f

And use the following to create the archive:

  ar -rcs libodepack.a opkda1.o opkda2.o opkdmain.o


libodepack.a, together with the gfortran library can now be linked during the compilation.

Windows was found to be too much hassle, though it may work if you compile with the -static 
command before building the archive.

