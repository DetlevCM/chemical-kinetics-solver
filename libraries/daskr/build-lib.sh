#!/bin/bash

#
# this is a build script to create the archive which supplies 
# the daspk functionality to the C++ code
#
# buid script written by Detlev Conrad Mielczarek 
#
#

gfortran -O3 -march=native -mavx2 -ffast-math -c \
./preconds/dbanpre.f ./preconds/dilupre.f ./preconds/drbdpre.f ./preconds/drbgpre.f \
./preconds/dsparsk.f ./solver/daux.f ./solver/ddaskr.f ./solver/dlinpk.f


ar -rcs libddaskr.a \
dbanpre.o dilupre.o drbdpre.o drbgpre.o \
dsparsk.o daux.o ddaskr.o dlinpk.o


