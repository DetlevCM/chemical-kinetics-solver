################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/rates_analysis/Rates_Analysis.cpp \
../source/rates_analysis/ReportAccuracy.cpp 

OBJS += \
./source/rates_analysis/Rates_Analysis.o \
./source/rates_analysis/ReportAccuracy.o 

CPP_DEPS += \
./source/rates_analysis/Rates_Analysis.d \
./source/rates_analysis/ReportAccuracy.d 


# Each subdirectory must supply rules for building sources it contributes
source/rates_analysis/%.o: ../source/rates_analysis/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"C:\Dev\workspace\CKS branches\headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


