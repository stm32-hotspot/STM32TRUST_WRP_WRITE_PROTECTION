## Overview

The STM32H5 was designed to radically improve the security on the STM32 product family, taking into account experience gained with previous Cortex-M33 microcontrollers, customer feedback and requirements to achieve highest PSA certification levels.
Most notable security-related innovations of the STM32H5 are the debug authentication and the lifecycle management using product state. Learning about these is a basic requirement even for the most elementary security. The secure storage, secure boot options and the security ecosystem are the next steps in the learning process.
For general STM32 security see this article or AN5156.

Flash write protection is configured in a bitmap fashion, with a granularity of 4 sectors (32kB per configuration bit). The configuration is entered using OB.
Also the SRAM2 features write protection, bits of write protection configuration each represent 1kB of the total 64kB (or 16kB in case of STM32H503) of the SRAM2 memory.

## <b>STM32TRUST_WRP_Write_Protection Example Description</b>

Program to check Flash sectors for write protection (WRP) option bytes (OB) and programs desired sectors with WRP.

Sectors are defined through a #define: OB_WRP_SECTORS

The program checks for pre-programmed flash sectors. If all the sectors are programmed as mentioned in #define OB_WRP_SECTORS, the USER_LED (GREEN, LD2) is toggled once at 500ms interval.

If not, the program checks for partially pre-programmed sectors, blinks USER_LED thrice every 500 ms and programs the remaining sectors per the OB_WRP_SECTORS define. Or, the program proceeds to program any new sectors per the #define.

The program ends with GREEN LED turned on.

In case of a WRP config failure, USER_LED toggles every 100 ms.

In this example, HCLK is configured at 250 MHz.

#### <b>Notes</b>

 1. Ensure a full chip erase of the target board before running the example. If not, use STM32CubeProgrammer to verify which sectors are programmed and make necessary adjustments to the example. 
 
 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds) based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower) than the peripheral interrupt. Otherwise the caller ISR process will be blocked. To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The example needs to ensure that the SysTick time base is always  set to 1 millisecondto have correct HAL operation.

### <b>Keywords</b>

System, GPIO, WRP, Option Bytes,

### <b>Directory contents</b>

  - STM32H5/Projects/wrp_write_protection/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - STM32H5/Projects/wrp_write_protection/Inc/STM32H5xx_hal_conf.h    HAL configuration file
  - STM32H5/Projects/wrp_write_protection/Inc/STM32H5xx_it.h          Interrupt handlers header file
  - STM32H5/Projects/wrp_write_protection/Inc/main.h                  Header for main.c module
  - STM32H5/Projects/wrp_write_protection/Src/STM32H5xx_it.c          Interrupt handlers
  - STM32H5/Projects/wrp_write_protection/Src/STM32H5xx_hal_msp.c     HAL MSP file
  - STM32H5/Projects/wrp_write_protection/Src/main.c                  Main program
  - STM32H5/Projects/wrp_write_protection/Src/system_STM32H5xx.c      STM32H5xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32H503xx devices.

  - This example has been tested with NUCLEO-H503RB board and can be
    easily tailored to any other supported device and development board. Details on NUCLEO-H503RB, please visit https://www.st.com/en/microcontrollers-microprocessors/stm32h503rb.html
    

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your STM32CubeIDE toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
