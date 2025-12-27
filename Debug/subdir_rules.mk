################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/software_code/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/ccs_workspace_sti/signal_distortion_measurement/USER" -I"D:/ccs_workspace_sti/signal_distortion_measurement" -I"D:/ccs_workspace_sti/signal_distortion_measurement/Debug" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_00_01_00/source" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/DSP/Include" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1245600479: ../signal_distortion_measurement.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.20.0/sysconfig_cli.bat" --script "D:/ccs_workspace_sti/signal_distortion_measurement/signal_distortion_measurement.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_00_01_00/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-1245600479 ../signal_distortion_measurement.syscfg
device.opt: build-1245600479
device.cmd.genlibs: build-1245600479
ti_msp_dl_config.c: build-1245600479
ti_msp_dl_config.h: build-1245600479
Event.dot: build-1245600479

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/software_code/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/ccs_workspace_sti/signal_distortion_measurement/USER" -I"D:/ccs_workspace_sti/signal_distortion_measurement" -I"D:/ccs_workspace_sti/signal_distortion_measurement/Debug" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_00_01_00/source" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/DSP/Include" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_2_00_01_00/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"D:/software_code/ti/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/ccs_workspace_sti/signal_distortion_measurement/USER" -I"D:/ccs_workspace_sti/signal_distortion_measurement" -I"D:/ccs_workspace_sti/signal_distortion_measurement/Debug" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_00_01_00/source" -I"C:/ti/mspm0_sdk_2_00_01_00/source/third_party/CMSIS/DSP/Include" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


