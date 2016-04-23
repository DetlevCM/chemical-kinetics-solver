################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/get_input/Get_Reactions.cpp \
../source/get_input/Get_Species.cpp \
../source/get_input/Handle_Mechanism_Input.cpp \
../source/get_input/Read_Input_Data_v2.cpp \
../source/get_input/Read_Input_Data_v3.cpp \
../source/get_input/Read_Thermodynamic_Data_New_Format.cpp 

OBJS += \
./source/get_input/Get_Reactions.o \
./source/get_input/Get_Species.o \
./source/get_input/Handle_Mechanism_Input.o \
./source/get_input/Read_Input_Data_v2.o \
./source/get_input/Read_Input_Data_v3.o \
./source/get_input/Read_Thermodynamic_Data_New_Format.o 

CPP_DEPS += \
./source/get_input/Get_Reactions.d \
./source/get_input/Get_Species.d \
./source/get_input/Handle_Mechanism_Input.d \
./source/get_input/Read_Input_Data_v2.d \
./source/get_input/Read_Input_Data_v3.d \
./source/get_input/Read_Thermodynamic_Data_New_Format.d 


# Each subdirectory must supply rules for building sources it contributes
source/get_input/%.o: ../source/get_input/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"C:\Dev\workspace\CKS branches\headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


