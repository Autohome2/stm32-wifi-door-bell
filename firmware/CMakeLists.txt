PROJECT(stm32-wifi-door-bell)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ENABLE_LANGUAGE(ASM)

IF(NOT STM32_USB_FS_DEVICE_LIB_DIR)
  SET(STM32_USB_FS_DEVICE_LIB_DIR "/opt/STM32_USB-FS-Device_Lib_V4.0.0")
  MESSAGE(STATUS "No STM32_USB_FS_DEVICE_LIB_DIR specified, using default: " ${STM32_USB_FS_DEVICE_LIB_DIR})
ENDIF()

SET(CMSIS_FIND_LIBS "cmsis_md")
SET(CMSIS_STARTUP_SOURCE "${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Device/ST/STM32F10x/Source/Templates/gcc_ride7/startup_stm32f10x_md.s")
SET(STM32_CHIP_DEF "STM32F10X_MD")
SET(STACK_ADDRESS "0x20005000")
SET(FLASH_SIZE "128K")
SET(RAM_SIZE "20K")
SET(MIN_STACK_SIZE "0x200")
SET(MIN_HEAP_SIZE "0")
SET(EXT_RAM_SIZE "0K")
SET(FLASH_ORIGIN "0x08000000")
SET(RAM_ORIGIN "0x20000000")
SET(EXT_RAM_ORIGIN "0x60000000")

CONFIGURE_FILE(stm32_flash.md.in ${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.md)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DMDNS_ADVERTISE_HOST -D${STM32_CHIP_DEF} -I../ -I../contiki/")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DMDNS_ADVERTISE_HOST -D${STM32_CHIP_DEF} -I../ -I../contiki/")
SET(CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_BINARY_DIR}/stm32_flash.md ${CMAKE_EXE_LINKER_FLAGS}")

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_STDPERIPH_DRIVER")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_STDPERIPH_DRIVER")

INCLUDE_DIRECTORIES(
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Device/ST/STM32F10x/Include/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/CMSIS/Include/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/inc/
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32_USB-FS-Device_Driver/inc/
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET(PROJECT_SOURCES
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
  ${STM32_USB_FS_DEVICE_LIB_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c
  system_stm32f10x.c

  main.c
  interrupts.c
  rtc.c
  config.c
  network.c
  button.c
  stm32lib/spi.c
  stm32lib/delay.c
  stm32lib/time.c
  stm32lib/debug.c
  stm32lib/util.c
  stm32lib/ntp.c
  stm32lib/newlib.c
  stm32lib/crc32.c
  stm32lib/sdcard.c
  stm32lib/sdcard-fat.c
  stm32lib/cc3000.c
  stm32lib/cc3000-host-driver/wlan.c
  stm32lib/cc3000-host-driver/cc3000_common.c
  stm32lib/cc3000-host-driver/socket.c
  stm32lib/cc3000-host-driver/hci.c
  stm32lib/cc3000-host-driver/evnt_handler.c
  stm32lib/cc3000-host-driver/nvmem.c
  stm32lib/cc3000-host-driver/netapp.c
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES} ${CMSIS_STARTUP_SOURCE})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}.elf ${CMSIS_LIBRARIES} ${StdPeriphLib_LIBRARIES})

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.list DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJDUMP} -x -S ${CMAKE_PROJECT_NAME}.elf > ${CMAKE_PROJECT_NAME}.list)

