################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/provenance_rewriter/transformation_rewrites/transformation_prov_main.o 

C_SRCS += \
../src/provenance_rewriter/transformation_rewrites/transformation_prov_main.c 

OBJS += \
./src/provenance_rewriter/transformation_rewrites/transformation_prov_main.o 

C_DEPS += \
./src/provenance_rewriter/transformation_rewrites/transformation_prov_main.d 


# Each subdirectory must supply rules for building sources it contributes
src/provenance_rewriter/transformation_rewrites/transformation_prov_main.o: /Users/xun/Documents/database_project/p1/provenance-rewriter-prototype/src/provenance_rewriter/transformation_rewrites/transformation_prov_main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

