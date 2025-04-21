c
c In case that you do not have the IntelODE library or it is no longer
c available, I am providing this subroutine which can be compiled into 
c a NON-FUNCTIONAL library to allow proper linking and compilation of 
c the project. The code can still be run using the Odepack solvers in 
c this case.
c
c Compile this file with the following commands:
c gfortran -c if_you_do_not_have_the_library.f
c
c DO NOT USE THIS IF YOU HAVE THE INTEL ODE LIBRARY, THIS MAY OVERWRITE IT WITH AN 
c EFFECTIVELY EMPTY LIBRARY
c
c And use the following to create the archive:
c ar -rcs libiode_intel64.a if_you_do_not_have_the_library.o
c
c then compile as usual and only use LSODA in the software during execution
c

      subroutine dodesol_mk52lfa(
     $           in1,in2,db1,db2,db3,ext1,ext2,
     $           db4,db5,db6,db7,db8,in3,in4)
     $ bind(C)

      use iso_c_binding, only: C_CHAR, c_null_char
c      implicit none
      
      integer in1, in2, in3, in4
      double precision db1, db2, db3, db4, db5, db6, db7, db8
c      external ext1, ext2
      
      in1 = in2
      
      end subroutine


      subroutine dodesol_rkm9mka(
     $           in1,in2,db1,db2,db3,ext1,ext2,
     $           db4,db5,db6,db7,db8,in3,in4)
     $ bind(C)

      use iso_c_binding, only: C_CHAR, c_null_char
c      implicit none
      
      integer in1, in2, in3, in4
      double precision db1, db2, db3, db4, db5, db6, db7, db8
c      external ext1, ext2
      
      in1 = in2

      end subroutine

      subroutine dodesol_mk52lfn(
     $           in1,in2,db1,db2,db3,ext1,ext2,
     $           db4,db5,db6,db7,db8,in3,in4)
     $ bind(C)

      use iso_c_binding, only: C_CHAR, c_null_char
c      implicit none
      
      integer in1, in2, in3, in4
      double precision db1, db2, db3, db4, db5, db6, db7, db8
c      external ext1, ext2
      
      in1 = in2
      
      end subroutine

      subroutine dodesol_rkm9mkn(
     $           in1,in2,db1,db2,db3,ext1,ext2,
     $           db4,db5,db6,db7,db8,in3,in4)
     $ bind(C)

      use iso_c_binding, only: C_CHAR, c_null_char
c      implicit none
      
      integer in1, in2, in3, in4
      double precision db1, db2, db3, db4, db5, db6, db7, db8
c      external ext1, ext2
      
      in1 = in2
      
      end subroutine
      
