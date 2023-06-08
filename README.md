# STM32TRUST_WRP_Write_Protection

## Overview

Security in microcontrollers encompasses several aspects including protection of firmware intellectual property, protection of
private data in the device, and guarantee of a service execution.

STM32Trust simplifies your approach to security and provides developers with a robust, multilevel strategy to enhance security in their new product designs. For more details, please visit: https://www.st.com/content/st_com/en/ecosystems/stm32trust.html


The write protection (WRP) feature is used to protect the content of the specified memory area against erase or update.
For instance, the write protection can be set on a page or a sector of a flash memory to prevent its alteration
during a firmware or data update. It can also be set by default on the unused memory area to prevent
any malware injection. Its granularity is linked to the page or sector size.

This protection must be used, in particular when write operations are foreseen within the application. This is
the case if data storage or code update operations are expected. The WRP prevents wrong accesses due to
unsafe functions causing unexpected overflows

For a general introduction to STM32 microcontrollers security, please refer to AN5156, available at: https://www.st.com/resource/en/application_note/an5156-introduction-to-stm32-microcontrollers-security-stmicroelectronics.pdf

## Demo

Demo example to demostrate the Write Protection feature of STM32 devices on various platforms.

The example requires a NUCLEO board of the target platform and utilizes onboard LEDs to indicate program status.

The demo example can be configured and expanded based on the user's requirement.

### Supported Platforms

The WRP is available on all STM32 devices. 
Although this example has been developed and tested on the following platforms, it can be easily tailored to any other supported device and development boards: 

* NUCLEO-H723GZ