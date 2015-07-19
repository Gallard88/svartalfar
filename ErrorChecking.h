#ifndef ERRORCHECKING_H
#define ERRORCHECKING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**************
 *  CalcChecksum()
 *  Calculates the checksum of a buffer of data.
 *  Returns 0 if buffer/size == 0
 *  Else returns calculated checksum.
 */
uint8_t CalcChecksum(const uint8_t * buffer, uint32_t size);

/**************
 *  CompareChecksum()
 *  Calculates the checksum on a buffer of data and compares it.
 *  Returns 0 if buffer/size == 0, or checksums do not match
 *  Returns 1 if checksums match.
 */
uint8_t CompareChecksum(const uint8_t * buffer, uint32_t size, uint8_t checksum);

/**************
 *  CalcCRC()
 *  Calculates the checksum on a buffer of data.
 *  Returns 0 if buffer/size == 0
 *  Else returns calculated CRC.
 */
uint16_t CalcCRC(const uint8_t * buffer, uint32_t size);

/**************
 *  CompareCalcCRC()
 *  Calculates the CRC of a buffer of data and compares it.
 *  Returns 0 if buffer/size == 0, or CRC's do not match
 *  Returns 1 if checksums match.
 */
uint16_t CompareCalcCRC(const uint8_t * buffer, uint32_t size, uint16_t crc);

#ifdef __cplusplus
}
#endif

#endif
