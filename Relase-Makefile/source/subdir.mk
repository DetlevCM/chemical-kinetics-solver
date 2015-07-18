################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/C_delta_n.cpp \
../source/Main.cpp \
../source/Make_Irreversible_v2.cpp \
../source/Prepare_Jacobian_Matrix.cpp \
../source/Run_Integrator.cpp 

OBJS += \
./source/C_delta_n.o \
./source/Main.o \
./source/Make_Irreversible_v2.o \
./source/Prepare_Jacobian_Matrix.o \
./source/Run_Integrator.o 

CPP_DEPS += \
./source/C_delta_n.d \
./source/Main.d \
./source/Make_Irreversible_v2.d \
./source/Prepare_Jacobian_Matrix.d \
./source/Run_Integrator.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++14 -I"X:\workspace\CKS branches\headers" -O2 -m64 -march=native -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


