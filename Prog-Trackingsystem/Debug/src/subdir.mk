################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Camera.cpp \
../src/ObjectDetection.cpp \
../src/Socket.cpp \
../src/calibrateCamera_Settings.cpp \
../src/main.cpp 

OBJS += \
./src/Camera.o \
./src/ObjectDetection.o \
./src/Socket.o \
./src/calibrateCamera_Settings.o \
./src/main.o 

CPP_DEPS += \
./src/Camera.d \
./src/ObjectDetection.d \
./src/Socket.d \
./src/calibrateCamera_Settings.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -lstdc++ -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -Wno-unused-local-typedefs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


