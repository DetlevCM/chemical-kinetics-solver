################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Fast.cpp \
../source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Slow.cpp \
../source/mechanism_reduction/species_lumping/Combine_Reactions_From_Species_Lumping.cpp \
../source/mechanism_reduction/species_lumping/Species_Class_Mapping.cpp \
../source/mechanism_reduction/species_lumping/Species_Class_New_Name.cpp \
../source/mechanism_reduction/species_lumping/n_zero_k-fitted_Fast.cpp \
../source/mechanism_reduction/species_lumping/n_zero_k-fitted_Slow.cpp 

OBJS += \
./source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Fast.o \
./source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Slow.o \
./source/mechanism_reduction/species_lumping/Combine_Reactions_From_Species_Lumping.o \
./source/mechanism_reduction/species_lumping/Species_Class_Mapping.o \
./source/mechanism_reduction/species_lumping/Species_Class_New_Name.o \
./source/mechanism_reduction/species_lumping/n_zero_k-fitted_Fast.o \
./source/mechanism_reduction/species_lumping/n_zero_k-fitted_Slow.o 

CPP_DEPS += \
./source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Fast.d \
./source/mechanism_reduction/species_lumping/Average_Ea_k-fitted_Slow.d \
./source/mechanism_reduction/species_lumping/Combine_Reactions_From_Species_Lumping.d \
./source/mechanism_reduction/species_lumping/Species_Class_Mapping.d \
./source/mechanism_reduction/species_lumping/Species_Class_New_Name.d \
./source/mechanism_reduction/species_lumping/n_zero_k-fitted_Fast.d \
./source/mechanism_reduction/species_lumping/n_zero_k-fitted_Slow.d 


# Each subdirectory must supply rules for building sources it contributes
source/mechanism_reduction/species_lumping/%.o: ../source/mechanism_reduction/species_lumping/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++11 -I"C:\Dev\workspace\CKS branches\headers" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


