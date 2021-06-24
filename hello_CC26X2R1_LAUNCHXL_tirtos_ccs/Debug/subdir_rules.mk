################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/kernel/tirtos/packages/ti/sysbios/knl" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs" --include_path="C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-319515983:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-319515983-inproc

build-319515983-inproc: ../hello.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs1031/xdctools_3_62_00_08_core/xs" --xdcpath="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source;C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC13X2_CC26X2 -r release -c "C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs\" --include_path=\"D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/kernel/tirtos/packages/ti/sysbios/knl\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs\" --include_path=\"C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-319515983 ../hello.cfg
configPkg/compiler.opt: build-319515983
configPkg/: build-319515983

build-1902055583: ../hello.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.8.0/sysconfig_cli.bat" -s "C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/.metadata/product.json" --script "D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/hello.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-1902055583 ../hello.syscfg
syscfg/ti_drivers_config.c: build-1902055583
syscfg/ti_drivers_config.h: build-1902055583
syscfg/ti_utils_build_linker.cmd.genlibs: build-1902055583
syscfg/syscfg_c.rov.xs: build-1902055583
syscfg/ti_utils_runtime_model.gv: build-1902055583
syscfg/ti_utils_runtime_Makefile: build-1902055583
syscfg/: build-1902055583

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/kernel/tirtos/packages/ti/sysbios/knl" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs" --include_path="C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="D:/ccsWorkspace/hello_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


