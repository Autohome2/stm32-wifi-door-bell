
#ifndef _SPI_H_
#define _SPI_H_

void spi_setup();
uint8_t spi_transfer(SPI_TypeDef* spi, uint8_t d);

#endif
