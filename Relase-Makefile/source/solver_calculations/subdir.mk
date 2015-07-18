################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/solver_calculations/Calculate_Rate_Constants.cpp \
../source/solver_calculations/Calculate_Reaction_Rates.cpp \
../source/solver_calculations/Calculate_Thermodynamics.cpp \
../source/solver_calculations/IntegrateRoutine.cpp \
../source/solver_calculations/Jacobian.cpp \
../source/solver_calculations/ODE_RHS_Liquid_Phase.cpp \
../source/solver_calculations/SpeciesLossRate.cpp 

OBJS += \
./source/solver_calculations/Calculate_Rate_Constants.o \
./source/solver_calculations/Calculate_Reaction_Rates.o \
./source/solver_calculations/Calculate_Thermodynamics.o \
./source/solver_calculations/IntegrateRoutine.o \
./source/solver_calculations/Jacobian.o \
./source/solver_calculations/ODE_RHS_Liquid_Phase.o \
./source/solver_calculations/SpeciesLossRate.o 

CPP_DEPS += \
./source/solver_calculations/Calculate_Rate_Constants.d \
./source/solver_calculations/Calculate_Reaction_Rates.d \
./source/solver_calculations/Calculate_Thermodynamics.d \
./source/solver_calculations/IntegrateRoutine.d \
./source/solver_calculations/Jacobian.d \
./source/solver_calculations/ODE_RHS_Liquid_Phase.d \
./source/solver_calculations/SpeciesLossRate.d 


# Each subdirectory must supply rules for building sources it contributes
source/solver_calculations/%.o: ../source/solver_calculations/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++14 -I"X:\workspace\CKS branches\headers" -O2 -m64 -march=native -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


