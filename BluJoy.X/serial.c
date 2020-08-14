#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "timers.h"

#define SPEED_CMD_LENGTH  12 
#define ESTOP_CMD_LENGTH  8 

extern volatile uint8_t eusart1TxBufferRemaining;

uint8_t speedBuffer[SPEED_CMD_LENGTH] ;
uint8_t estopBuffer[ESTOP_CMD_LENGTH] = {'/', 0, 0, 0xFF, 0, 0, 0, '\n'};

void    initSerial(void) {
 
    memset(speedBuffer, 0, sizeof(speedBuffer));
    speedBuffer[0]  = '/';
    speedBuffer[1]  = 4;     // 4 bytes of data (two short ints)
    speedBuffer[2]  = 0;
    speedBuffer[3]  = 0x86;  // Dual Speed command
    speedBuffer[4]  = 0;     // No Reply
    speedBuffer[11] = '\n';  // EOM
}

bool    sendBTSpeedCmd(int16_t  axial, int16_t yaw,  bool blockIfBusy) {
    int16ToBytes(axial, speedBuffer + 5);
    int16ToBytes(yaw,   speedBuffer + 7);
    calcCRC(speedBuffer);
    return sendBTBuffer((void *)speedBuffer, sizeof(speedBuffer), blockIfBusy);
}

bool     sendBTEstopCmd(void){
    calcCRC(estopBuffer);
    return sendBTBuffer((void *)estopBuffer, sizeof(estopBuffer), true);
}

void    sendBTString(char * buffer) {
    flushBTRXbuffer();
    sendBTBuffer((void *)buffer, strlen(buffer), true);
}
 
bool    sendBTBuffer(uint8_t * buffer, uint8_t length, bool blockIfBusy) {
    if (blockIfBusy || (eusart1TxBufferRemaining > length)){
        while (length > 0) {
            // wait till tx buffer ready
            while(!EUSART1_is_tx_ready());

            // Write next character
            EUSART1_Write(*buffer++);
            length--;
        }
        return true;
    }
    else {
        return false;
    }
}

void    flushBTRXbuffer(void) {
    while (EUSART1_is_rx_ready()){
        EUSART1_Read();
        if (!EUSART1_is_rx_ready())
            sleep(5);  // Just in case they are still coming
    }
}

/**
 * 
 * @param  buffer    Pointer to buffer to contain received chars
 * @param  maxChars  Maximum characters required
 * @param  timeoutMS Length of time to wait for chars
 * @return Number of characters read.
 */
uint8_t receiveBTBuffer(uint8_t * buffer, uint8_t maxChars, uint16_t timeoutMS) {
    uint32_t startTime = getTicks();
    uint8_t charsRead = 0;
    
    // return when chars read, or timeout.
    while ((charsRead < maxChars) && (getTicksSince(startTime) <= timeoutMS)) {
        if (EUSART1_is_rx_ready()) {
            (*buffer++ = EUSART1_Read());
            charsRead++;
        }
    }
    return charsRead;
}

//----------------------------------------------------------------------------
// Calculate CRC
//----------------------------------------------------------------------------
uint16_t calcCRC(uint8_t *ptr)
{
  uint16_t  crc;
  uint8_t 	i;
  int8_t	count = ptr[1] + 5;
  crc = 0;

  while (--count >= 0)
  {
    crc = crc ^ (uint16_t) *ptr++ << 8;
    i = 8;
    do
    {
      if (crc & 0x8000)
        crc = crc << 1 ^ 0x1021;
      else
        crc = crc << 1;
    } while(--i);
  }
  
  *ptr++ = crc & 0xFF;
  *ptr++ = (crc >> 8) & 0xFF;
  
  return (crc);
}

void	int16ToBytes(int16_t num, uint8_t * ndata) {
	for (int i = 0; i < 2; i++) {
		*ndata++ = ((uint8_t)(num & 0xFF));
		num = num >> 8;
	}
}
