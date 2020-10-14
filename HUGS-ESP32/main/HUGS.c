#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "driver/adc.h"

#include "HUGS.h"
#include "hugsIO.h"

static const char *TAG = "hugs";
static const bool verbose  = true;
static const bool xverbose = false;

DRIVE_STATUS master;
DRIVE_STATUS slave;
int16_t		 leftSpeed = 0;
int16_t		 rightSpeed = 0;

uint8_t	REScommand[]    = {'/', 0,  0, RES, SMOD,         		     0,   0, '\n'};
uint8_t	MODcommand[]    = {'/', 2,  0, MOD, SMOD,   SPEED_DUAL, 250, 0,   0, '\n'};
uint8_t	PWRcommand[]    = {'/', 2,  0, POW, SMOT,   0, 0, 		     0,   0, '\n'};
uint8_t	SPEcommand[]    = {'/', 2,  0, SPE, SMOT,   0, 0, 		     0,   0, '\n'};
uint8_t	XXXcommand[]    = {'/', 0,  0, XXX, SMOT,         		     0,   0, '\n'};

bool	shuttingDown 	= false;
uint8_t	sequence		=   0;

int	max(int a, int b) {
	return ((a > b) ? a : b);
}

void resetPosition() {
    sendData(&master, REScommand, sizeof(REScommand));
    sendData(&slave,  REScommand, sizeof(REScommand));
	vTaskDelay(CONTROL_LOOP_PERIOD / portTICK_PERIOD_MS);
}

void setSpeedMode(uint8_t mode, uint8_t maxStepSpeed) {
	MODcommand[5] = mode;
	MODcommand[6] = maxStepSpeed;

	sendData(&master, MODcommand, sizeof(MODcommand));
	sendData(&slave,  MODcommand, sizeof(MODcommand));
	vTaskDelay(CONTROL_LOOP_PERIOD / portTICK_PERIOD_MS);
}

void setSpeeds(uint16_t newLeftSpeed, int16_t newRightSpeed) {
	leftSpeed  = newLeftSpeed;
	rightSpeed = newRightSpeed;
}

void setTwist(int16_t mmPerSec,  int16_t degreesPerSec) {
	int leftDrive  = mmPerSec ;
	int rightDrive = mmPerSec ;
	int turnSpeed  = (int)((float)degreesPerSec * MM_PER_DEGREE);
	int smax = 0;

	// Positive rotation is CW
	leftDrive  += turnSpeed;
	rightDrive -= turnSpeed;

	// normalize so no speed is > +/-TOP_SPEED
	smax = max(abs(leftDrive), abs(rightDrive));

	if (smax > TOP_SPEED) {
		leftDrive  = (leftDrive  * TOP_SPEED) / smax;
		rightDrive = (rightDrive * TOP_SPEED) / smax;
	}
	setSpeeds(leftDrive, rightDrive);
}

void start_HUGS_tasks(void)
{
	ESP_LOGW("HUGS", "INIT");
    init_HUGS();

    xTaskCreate(rx_task, "master_rx", 2048, (void *)&master, configMAX_PRIORITIES-1, NULL);
    xTaskCreate(rx_task, "slave_rx", 2048, (void *)&slave, configMAX_PRIORITIES-1, NULL);

    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, 4, NULL);
}

void init_HUGS(void) {

	setupPort(&master, "MA", MASTER_UART );
	setupPort(&slave,  "SL", SLAVE_UART );

    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(MASTER_UART, 512 , 0 , 20, &(master.queue), 0);
    uart_param_config(MASTER_UART, &uart_config);
    uart_set_pin(MASTER_UART, TXMASTER_PIN, RXMASTER_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(SLAVE_UART, 512, 0, 20, &(slave.queue), 0);
    uart_param_config(SLAVE_UART, &uart_config);
    uart_set_pin(SLAVE_UART, TXSLAVE_PIN, RXSLAVE_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void	setupPort(DRIVE_STATUS * status, char * name, int portNum) {
	memset((void *)status, 0, sizeof(DRIVE_STATUS) );
	strncpy(status->name, name, sizeof(status->name));
	status->UART_num = portNum;
}

void rx_task(void *arg)
{
	DRIVE_STATUS *status = (DRIVE_STATUS *)(arg);

	uint8_t		  abyte = 0;
    uart_event_t  event;

    for(;;) {
    	// Wait for a UART event to occur
        if(xQueueReceive(status->queue, (void * )&event, (portTickType)portMAX_DELAY)) {
        	// Incoming data ??
            if (event.type == UART_DATA) {
        		// Process each character till full message is received.
            	while (uart_read_bytes(status->UART_num, &abyte, 1, 0) > 0) {
            		if (!status->appending && abyte == '/') {
            			status->appending = true;
            		}

            		if (status->appending) {
            			status->RXbuf[status->RXIndex++] = abyte;

						// should we check message
						if ((status->RXIndex > 2) && (status->RXIndex == status->RXbuf[1] + 8)) {
							checkRXBuffer(status);
							status->RXIndex = 0;
							status->appending = false;
						}
            		}
            	}

            }
        }
    }
}

void	checkRXBuffer(DRIVE_STATUS * status) {
    uint8_t		length  = status->RXbuf[1] + 5;
    uint16_t	crc		= CalcCRC(status->RXbuf);

	if ( (status->RXbuf[length + 2] == '\n') &&
  	     ((crc & 0xFF) == status->RXbuf[length]) &&
		 ((crc >> 8)   == status->RXbuf[length+1])  ) {

		saveResponse(status);
		if (xverbose) {
			displayBuffer(status);
		}
	}
}


int 	sendData(DRIVE_STATUS * status, uint8_t * buffer, int len)
{
	// Add CRC to message
	uint8_t dataLength;
    uint16_t crc;

	dataLength = buffer[1];
    buffer[2] = sequence;
    crc = CalcCRC((uint8_t *)buffer);

    buffer[dataLength + 5] = crc & 0xFF;
    buffer[dataLength + 6] = (crc >> 8) & 0xFF;
    buffer[dataLength + 7] = '\n';

    if (status->pending < MAX_PENDING) {
    	status->pending++;
    }

    if (xverbose) {
		printf("\n> ");
		for (int i = 0; i< buffer[1] + 8; i++ ) {
			printf("%02X>", buffer[i]);
		}
		printf("\n");
    }

	return uart_write_bytes(status->UART_num, (char *)buffer, len);
}

void 	tx_task(void *arg)
{

    sendData(&master, REScommand, sizeof(REScommand));
    sendData(&slave,  REScommand, sizeof(REScommand));
	vTaskDelay(CONTROL_LOOP_PERIOD / portTICK_PERIOD_MS);

    while(!shuttingDown) {


//		  if (RSP_ID == 1) {
//        	printf("\033c");
//        	displayStatus(&master);
//    		displayStatus(&slave);
//  		motionPerformance();
//        }

		// Check for missing Local replies
		if ((master.pending >= MAX_PENDING) ||
			(slave.pending >= MAX_PENDING)  ) {
			shutDown("loss of reply");
		}

		motionPerformance();
		//  printf ("M (%ld) S (%ld)\n", RxMaster - TxMaster, RxSlave - TxSlave);

        // send current velocity commands
   		SPEcommand[4] = SMOT ;
		int16ToBytes( -leftSpeed, &SPEcommand[5]);
		sendData(&master, SPEcommand, sizeof(SPEcommand));

		int16ToBytes( rightSpeed, &SPEcommand[5]);
		sendData(&slave, SPEcommand, sizeof(SPEcommand));

		sequence += 0x10;
		vTaskDelay(CONTROL_LOOP_PERIOD / portTICK_PERIOD_MS);
    }

    while(true) {
		vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void	shutDown(char * reason) {
	shuttingDown = true;

	sendData(&slave, XXXcommand, sizeof(XXXcommand));
	sendData(&master, XXXcommand, sizeof(XXXcommand));
    ESP_LOGW(TAG, "Shutdown (%s)", reason);
	vTaskDelay(100 / portTICK_PERIOD_MS);

	for(;;) {
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		sendData(&slave, XXXcommand, sizeof(XXXcommand));
		sendData(&master, XXXcommand, sizeof(XXXcommand));
	    ESP_LOGW(TAG, "Shutdown (%s)", reason);
	}
}


void	motionPerformance(void) {
	if (verbose) {
		printf ("[%4d] %4d -> %8d (%4d) <%d>   [%4d] %4d -> %8d (%4d) <%d>\n",
			    leftSpeed,  master.mmps, master.mm, master.power, master.controlMode,
				rightSpeed,  slave.mmps,  slave.mm,  slave.power, slave.controlMode) ;

//		printf ("[%4d] %4d (%4d) %8d <%d> F:P:I %5d %5d %5d\n",
//			    leftSpeed,  master.mmps, master.power, master.mm, master.controlMode,
//				master.outF, master.outP,  master.outI) ;

	} else {
		printf ("(%4d mm/s %8d mm) (%4d mm/s %8d mm)\n",
			    master.mmps, master.mm, slave.mmps,  slave.mm) ;
	}
}

void	displayBuffer(DRIVE_STATUS * status) {
	switch (status->RXbuf[4]) {
	case SSPE:
		printf ("%s Velocity:  %8d mm/s\n", status->name, bytesToInt16(status->RXbuf));
		break;

	case SPOS:
		printf ("%s Position:  %8d mm\n", status->name,  (int)(bytesToInt32(status->RXbuf)));
		break;

	case SVOL:
		printf ("%s Voltage:   %8d mV\n", status->name,  bytesToUInt16(status->RXbuf));
		break;

	case SAMP:
		printf ("%s Current:   %8d mA\n", status->name,  bytesToUInt16(status->RXbuf));
		break;

	case SPOW:
		printf ("%s Power:     %8d %%\n", status->name,  bytesToInt16(status->RXbuf));
		break;

	case SMOD:
		printf ("%s Mode: %2d  MaxSpeed: %3d mmPs\n", status->name,  status->RXbuf[6],  status->RXbuf[7]);
		break;

	case SDOG:
		printf ("%s WatchDog:  %8d mS\n", status->name,  bytesToUInt16(status->RXbuf));
		break;

	case SMOT:
		printf ("%s Motion:  %8d mm/s, %8d mm, %8d %%\n", status->name,
				bytesToInt16(status->RXbuf), (int)(bytesToInt32(status->RXbuf+2)), bytesToInt16(status->RXbuf+6));
		break;

	default:
		break;
	}
}

void	displayStatus(DRIVE_STATUS * status) {
	printf("%s (%d) V %+5d P %+7d W %+4d B %5d /%4d [%d]\n",
			status->name, status->pending,
			status->mmps, status->mm, status->power,
			status->mV, status->mA, leftDrive) ;
}

void	saveResponse(DRIVE_STATUS * status) {


    if (xverbose) {
		printf("\n< ");
		for (int i = 0; i< status->RXbuf[1] + 8; i++ ) {
			printf("%02X<", status->RXbuf[i]);
		}
		printf("\n");
    }

	status->pending = 0;
	status->response = status->RXbuf[4];
	status->state = status->RXbuf[5];

	if ((status->state & LOCAL_ESTOP) == LOCAL_ESTOP) {
		if (status == &master)
			shutDown("Local Master ESTOP");
		else
			shutDown("Local Slave ESTOP");

	} else {

		status->controlMode = (status->state >> 1) & 0x03;

		switch (status->RXbuf[4]) {
		case SSPE:
			status->mmps = bytesToInt16(status->RXbuf);
			break;

		case SPOS:
			status->mm = (int)(bytesToInt32(status->RXbuf));
			break;

		case SVOL:
			status->mV = bytesToUInt16(status->RXbuf);
			break;

		case SAMP:
			status->mA = bytesToUInt16(status->RXbuf);
			break;

		case SPOW:
			status->power = bytesToInt16(status->RXbuf);
			break;

		case SDOG:
			status->wdog = bytesToUInt16(status->RXbuf);
			break;

		case SMOD:
			status->speedMode = status->RXbuf[6];
			status->maxStepSpeed = status->RXbuf[7];
			break;

		case SMOT:
			status->mmps  = bytesToUInt16(status->RXbuf);
			status->mm    = bytesToInt32(status->RXbuf + 2);
			status->power = bytesToUInt16(status->RXbuf + 6);
			break;

		case SFPI:
			status->outF  = bytesToInt16(status->RXbuf);
			status->outP  = bytesToInt16(status->RXbuf + 2);
			status->outI  = bytesToInt16(status->RXbuf + 4);
			break;

		default:
			break;
		}
	}
}


//----------------------------------------------------------------------------
// Calculate CRC
//----------------------------------------------------------------------------
uint16_t CalcCRC(uint8_t *ptr)
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
      {
        crc = crc << 1 ^ 0x1021;
      }
      else
      {
        crc = crc << 1;
      }
    } while(--i);
  }
  return (crc);
}

int8_t	bytesToInt8 (uint8_t * ndata) {
	return (int8_t)ndata[6]	;
}

int16_t	bytesToInt16 (uint8_t * ndata) {
	return (int16_t)(((uint16_t)ndata[7] << 8) + ndata[6]);
}

uint16_t bytesToUInt16 (uint8_t * ndata) {
	return (((uint16_t)ndata[7] << 8) + ndata[6]);
}

int32_t bytesToInt32 (uint8_t * ndata) {
	return ((int32_t)(((uint32_t)ndata[9] << 24) | ((uint32_t)ndata[8] << 16) | ((uint32_t)ndata[7] << 8) |  ndata[6]));
}

void	int16ToBytes(int16_t num, uint8_t * ndata) {
	*ndata++ = ((uint8_t)((uint16_t)num & 0xFF));
	*ndata   = ((uint8_t)((uint16_t)num >> 8));
}


