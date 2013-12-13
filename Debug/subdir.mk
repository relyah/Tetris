################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Piece.cpp \
../Well.cpp \
../main.cpp \
../resTexture.cpp \
../shader_utils.cpp 

OBJS += \
./Piece.o \
./Well.o \
./main.o \
./resTexture.o \
./shader_utils.o 

CPP_DEPS += \
./Piece.d \
./Well.d \
./main.d \
./resTexture.d \
./shader_utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


