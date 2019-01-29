
The ODEPACK Code is in the public domain based on the license from 
this repository: https://github.com/jacobwilliams/odepack .
Thus the files required in this project are mirrored in this repository for 
convenience and to ensure that the code has fewer external dependencies.
Please see opkd-sum for a detailed description of the code by the original authors, 
including litarature references.
No modifications (!!) to the original source code are required for the use of this 
library. 

Originally, the code was developed by Alan C. Hindmarsh at LLNL and Linda R. Petzold
at the University of California.
Further repositories  for the ODEPACK library are the netlib library:
https://computation.llnl.gov/casc/odepack/
(Which is the source of the files used in this project.)
and the website of the Lawrence Livermore National Laboratories:
https://computation.llnl.gov/casc/odepack/

The main source code for the chemical kinetics solver is written using C++, 
so Fortran code has no direct usability.
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


Note: The opdka1_mod.f file has had a number of functions removed which collided with 
functions in the daskr library. Whether this implementation will be workable is to be 
determined. 
