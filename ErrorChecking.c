
#include <stdio.h>

#include "ErrorChecking.h"

uint8_t CalcChecksum(const uint8_t * buffer, uint32_t size)
{
  uint8_t sum = 0;
  uint32_t i;
  if ( buffer != NULL ) {
    for ( i = 0; i < size; i ++ ) {
      sum += buffer[i];
    }
  }
  return sum;
}

uint8_t CompareChecksum(const uint8_t * buffer, uint32_t size, uint8_t checksum)
{
  uint8_t calc =  CalcChecksum(buffer, size);
  if ( calc == 0 ) {
    return 0;
  } else if ( calc == checksum ) {
    return 1;
  } else {
    return 0;
  }
}

uint16_t CalcCRC(const uint8_t * buffer, uint32_t size)
{
  uint16_t crc = 0;
  uint32_t i, j;
  
  if ( buffer != NULL ) {
    for ( j = 0; j < size; j ++ ) {
      crc = crc ^ ((uint16_t) buffer[j] << 8);
      i = 8;
      do {
        if (crc & 0x8000) {
          crc = (crc << 1) ^ 0x1021;
        } else {
          crc = crc << 1;
        }
      } while(--i);
    }
  }
  return crc;
}

uint16_t CompareCalcCRC(const uint8_t * buffer, uint32_t size, uint16_t crc)
{
  uint16_t calc =  CalcCRC(buffer, size);
  if ( calc == 0 ) {
    return 0;
  } else if ( calc == crc ) {
    return 1;
  } else {
    return 0;
  }
}

