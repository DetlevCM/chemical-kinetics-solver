################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/petrooxy/PetroOxyAdditionalOutput.cpp \
../source/petrooxy/adjust_gas_concentration_PetroOxy.cpp 

OBJS += \
./source/petrooxy/PetroOxyAdditionalOutput.o \
./source/petrooxy/adjust_gas_concentration_PetroOxy.o 

CPP_DEPS += \
./source/petrooxy/PetroOxyAdditionalOutput.d \
./source/petrooxy/adjust_gas_concentration_PetroOxy.d 


# Each subdirectory must supply rules for building sources it contributes
source/petrooxy/%.o: ../source/petrooxy/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"C:\Dev\workspace\CKS branches\headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


