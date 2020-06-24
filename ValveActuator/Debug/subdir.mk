################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP.c \
../DIO.c \
../ESP8266.c \
../Scheduler.c \
../TIMER0.c \
../UART.c \
../main.c 

OBJS += \
./APP.o \
./DIO.o \
./ESP8266.o \
./Scheduler.o \
./TIMER0.o \
./UART.o \
./main.o 

C_DEPS += \
./APP.d \
./DIO.d \
./ESP8266.d \
./Scheduler.d \
./TIMER0.d \
./UART.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


