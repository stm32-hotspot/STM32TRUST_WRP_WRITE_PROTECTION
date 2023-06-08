## <b>STM32TRUST_WRP_Write_Protection Example Description</b>

Program to check Flash sectors for write protection (WRP) option bytes (OB) and programs desired sectors with WRP.

Sectors are defined through a #define: OB_WRP_SECTORS

The program checks for pre-programmed flash sectors. If all the sectors are programmed as mentioned in #define OB_WRP_SECTORS, the YELLOW LED is turned ON and program exits.

If not, the program checks for partially pre-programmed sectors, blinks YELLOW LED thrice every 500 ms (left ON thereafter) and programs the remaining sectors per the OB_WRP_SECTORS define. 
    If successful, GREEN LED is turned on. 
    If failed, RED LED is turned on and toggles every 100 ms.

The program ends with GREEN LED flashing every 500 ms.

PB.00 (LED_GREEN), PB.14 (LED_RED) and PE.01 (LED_YELLOW) IOs (configured in output pushpull mode) and turn on/toggle based on program status.

In this example, HCLK is configured at 160 MHz.

#### <b>Notes</b>

 1. Ensure a full chip erase of the target board before running the example. If not, use STM32CubeProgrammer to verify which sectors are programmed and make necessary adjustments to the example. 
 
 2. Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
    based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
    a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
    than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
    To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

 3. The example needs to ensure that the SysTick time base is always set to 1 millisecond
    to have correct HAL operation.

### <b>Keywords</b>

System, GPIO, WRP, Option Bytes,

### <b>Directory contents</b>

  - STM32H7/Projects/wrp_write_protection/Inc/stm32u5xx_nucleo_conf.h BSP configuration file
  - STM32H7/Projects/wrp_write_protection/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - STM32H7/Projects/wrp_write_protection/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - STM32H7/Projects/wrp_write_protection/Inc/main.h                  Header for main.c module
  - STM32H7/Projects/wrp_write_protection/Src/stm32h7xx_it.c          Interrupt handlers
  - STM32H7/Projects/wrp_write_protection/Src/stm32h7xx_hal_msp.c     HAL MSP file
  - STM32H7/Projects/wrp_write_protection/Src/main.c                  Main program
  - STM32H7/Projects/wrp_write_protection/Src/system_stm32h7xx.c      STM32H7xx system source file

### <b>Hardware and Software environment</b>

  - This example runs on STM32H723xx devices.

  - This example has been tested with NUCLEO-H723ZG board and can be
    easily tailored to any other supported device and development board. Details on NUCLEO-H723GZ, please visit https://www.st.com/en/microcontrollers-microprocessors/stm32h723-733.html
    

### <b>How to use it ?</b>

In order to make the program work, you must do the following :

 - Open your STM32CubeIDE toolchain
 - Rebuild all files and load your image into target memory
 - Run the example
