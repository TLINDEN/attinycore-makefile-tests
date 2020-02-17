/* Optimized AVR305 half-duplex serial uart implementation
 * timing for 81N, 115.2kbps @8Mhz = 69.4 cycles/bit
 * and @16Mhz = 138.9 cycles/bit
 * @author: Ralph Doncaster
 * @version: $Id$
 */


#ifdef F_CPU
  #define TXDELAY (((F_CPU/BAUD_RATE)-9)/3)
#else
  #error CPU frequency F_CPU undefined
#endif

#if TXDELAY > 255
  #error low baud rates unsupported - use higher BAUD_RATE
#endif

void TxTimedByte(char, char);


#define TxByte(C) TxTimedByte(C , TXDELAY)

