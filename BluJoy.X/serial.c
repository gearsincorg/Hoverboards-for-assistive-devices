#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "timers.h"

#define SPEED_CMD_LENGTH  12 
#define ESTOP_CMD_LENGTH  8 
#define DEBUG_CMD_LENGTH  16

#define MAX_SPEED_MMPS    5000
#define MAX_YAW_DPS       5000

// #define MAX_YAW_DPS       1300

extern volatile uint8_t eusart1TxBufferRemaining;

bool    sendDebugText = false;

uint8_t speedBuffer[SPEED_CMD_LENGTH] ;
uint8_t estopBuffer[ESTOP_CMD_LENGTH] = {'/', 0, 0, 0xFF, 0, 0, 0, '\n'};

uint8_t debugBuffer[DEBUG_CMD_LENGTH] ;

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
    axial = clampInt(axial, MAX_SPEED_MMPS);
    yaw   = clampInt(yaw,   MAX_YAW_DPS);

    // Send either the debug text or the binary text.
    if (sendDebugText){
        debugBuffer[0] = 0;
        strcat(debugBuffer, "> ");
        int16cat(debugBuffer, axial);
        strcat(debugBuffer, ",");
        int16cat(debugBuffer, yaw);
        strcat(debugBuffer, "\n");
                
        return sendBTBuffer((void *)debugBuffer, strlen(debugBuffer), blockIfBusy);
    } else {
        int16ToBytes(axial, speedBuffer + 5);
        int16ToBytes(yaw,   speedBuffer + 7);
        calcCRC(speedBuffer);
        return sendBTBuffer((void *)speedBuffer, sizeof(speedBuffer), blockIfBusy);
    }
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

int16_t	clampInt(int16_t num, int16_t limit) {
    if (num > limit)
        num = limit ;
    else if (num < -limit)
        num = - limit;
    return num;
}

void    int16cat(uint8_t * bufP, int16_t num){
    bool    suppress = true;
    int16_t digit = 0;
    int16_t divider = 10000;

    // Find the end of the string;
    while (*bufP != 0)
        bufP++;

    // Look for negative number
    if (num < 0) {
        num = -num;
        *bufP++ = '-';
    }

    // Append the number
    while (divider > 0){
        digit = num / divider;
        num = num - (digit * divider);
        divider /= 10;
        
        if ((digit != 0) || !suppress){
            suppress = false;
            *bufP++ = '0' + digit;
        }
    }
    
    if (suppress)
        *bufP++ = '0';
        
    *bufP++ = 0;
}    