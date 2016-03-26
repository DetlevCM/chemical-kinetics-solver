################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/mechanism_reduction/Compare_Concentrations.cpp \
../source/mechanism_reduction/Get_Combine_Species_Mapping.cpp \
../source/mechanism_reduction/Process_Thermodynamics_Species_Classes.cpp \
../source/mechanism_reduction/Reduce_Reactions_v2.cpp \
../source/mechanism_reduction/Remove_Species.cpp \
../source/mechanism_reduction/Rename_Species.cpp \
../source/mechanism_reduction/mechanism_picking.cpp 

OBJS += \
./source/mechanism_reduction/Compare_Concentrations.o \
./source/mechanism_reduction/Get_Combine_Species_Mapping.o \
./source/mechanism_reduction/Process_Thermodynamics_Species_Classes.o \
./source/mechanism_reduction/Reduce_Reactions_v2.o \
./source/mechanism_reduction/Remove_Species.o \
./source/mechanism_reduction/Rename_Species.o \
./source/mechanism_reduction/mechanism_picking.o 

CPP_DEPS += \
./source/mechanism_reduction/Compare_Concentrations.d \
./source/mechanism_reduction/Get_Combine_Species_Mapping.d \
./source/mechanism_reduction/Process_Thermodynamics_Species_Classes.d \
./source/mechanism_reduction/Reduce_Reactions_v2.d \
./source/mechanism_reduction/Remove_Species.d \
./source/mechanism_reduction/Rename_Species.d \
./source/mechanism_reduction/mechanism_picking.d 


# Each subdirectory must supply rules for building sources it contributes
source/mechanism_reduction/%.o: ../source/mechanism_reduction/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"C:\Dev\workspace\CKS branches\headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


