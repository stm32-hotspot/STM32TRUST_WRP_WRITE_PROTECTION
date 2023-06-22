## Overview

The STM32H5 was designed to radically improve the security on the STM32 product family, taking into account experience gained with previous Cortex-M33 microcontrollers, customer feedback and requirements to achieve highest PSA certification levels.
Most notable security-related innovations of the STM32H5 are the debug authentication and the lifecycle management using product state. Learning about these is a basic requirement even for the most elementary security. The secure storage, secure boot options and the security ecosystem are the next steps in the learning process.
For general STM32 security see this article or AN5156.

Flash write protection is configured in a bitmap fashion, with a granularity of 4 sectors (32kB per configuration bit). The configuration is entered using OB.
Also the SRAM2 features write protection, bits of write protection configuration each represent 1kB of the total 64kB (or 16kB in case of STM32H503) of the SRAM2 memory.