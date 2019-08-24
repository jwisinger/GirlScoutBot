################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MX_app_x-cube-ble1.c \
../Src/MX_sensor_service.c \
../Src/custom_bus.c \
../Src/freertos.c \
../Src/hci_tl_interface.c \
../Src/main.c \
../Src/motor.c \
../Src/stm32f1xx_hal_exti.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_hal_timebase_tim.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c 

OBJS += \
./Src/MX_app_x-cube-ble1.o \
./Src/MX_sensor_service.o \
./Src/custom_bus.o \
./Src/freertos.o \
./Src/hci_tl_interface.o \
./Src/main.o \
./Src/motor.o \
./Src/stm32f1xx_hal_exti.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_hal_timebase_tim.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o 

C_DEPS += \
./Src/MX_app_x-cube-ble1.d \
./Src/MX_sensor_service.d \
./Src/custom_bus.d \
./Src/freertos.d \
./Src/hci_tl_interface.d \
./Src/main.d \
./Src/motor.d \
./Src/stm32f1xx_hal_exti.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_hal_timebase_tim.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Inc" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Drivers/CMSIS/Include" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/ST/BlueNRG-MS/includes" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic" -I"C:/Users/John/Documents/Stm32/GirlScoutBot/Middlewares/ST/BlueNRG-MS/utils"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


