################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
USER/%.o: ../USER/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/software_code/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/ccs_workspace_sti/signal_distortion_measurement/USER" -I"D:/ccs_workspace_sti/signal_distortion_measurement" -I"D:/ccs_workspace_sti/signal_distortion_measurement/Debug" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_00_01_00/source" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/DSP/Include" -gdwarf-3 -MMD -MP -MF"USER/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


