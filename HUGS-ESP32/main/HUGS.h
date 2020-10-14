/*
 * HUGS.h
 *
 *  Created on: Mar 13, 2020
 *      Author: Phil
 */

#ifndef HUGS_H_
#define HUGS_H_

// ===============================================================
// declarations for global use.
// ===============================================================

#define SPEED_PF 0
#define SPEED_STEP 1
#define SPEED_DUAL 2

// Unmodified HOVER-1 Ultra Dimensions
#define WHEEL_BASE			 408
#define MM_PER_DEGREE 	   3.508
#define DEGREE_PER_MM 	  0.2851
#define	TOP_SPEED 			5000
#define	TOP_TURN			1425


void 		resetPosition(void) ;
void 		setSpeedMode(uint8_t mode, uint8_t maxStepSpeed);
void 		setSpeeds(uint16_t leftSpeed, int16_t rightSpeed);
void 		setTwist(int16_t mmPerSec,  int16_t degreesPerSec);

void 		start_HUGS_tasks(void);
void 		init_HUGS(void);
void		shutDown(char * reason);

// ===============================================================
// declarations for internal use.
// ===============================================================

#define NOP 0
#define RSP 1
#define RES 2
#define ENA 3
#define DIS 4
#define POW 5
#define SPE 6
#define ABS 7
#define REL 8
#define DOG 9
#define MOD 10
#define XXX 0xFF

#define NOR 0
#define SMOT 1
#define SPOW 2
#define SSPE 3
#define SPOS 4
#define SVOL 5
#define SAMP 6
#define SDOG 7
#define SMOD 8
#define SFPI 9
#define STOP 0xFF

#define CONTROL_LOOP_PERIOD  20
#define REMOTE_TIMEOUT		 200
#define LOCAL_ESTOP			 0x01

#define PORT_NAME_SIZE 12
#define RX_BUF_SIZE 64

#define MAX_PENDING 4

#define MASTER_UART UART_NUM_1
#define SLAVE_UART UART_NUM_2

#define TXMASTER_PIN (GPIO_NUM_2)
#define RXMASTER_PIN (GPIO_NUM_15)

#define TXSLAVE_PIN (GPIO_NUM_17)
#define RXSLAVE_PIN (GPIO_NUM_16)

typedef struct {
	char		name[PORT_NAME_SIZE];
	uint8_t		UART_num;
	uint8_t		response;
	QueueHandle_t queue;
	uint8_t		RXbuf[RX_BUF_SIZE];
	uint8_t		RXIndex;
	bool		appending;
	uint8_t		pending;
	uint8_t		state;
	int16_t		power;
	int32_t		mm;
	int16_t		mmps;
	uint16_t	mV;
	uint16_t	mA;
	uint16_t	wdog;
	uint8_t		controlMode;
	uint8_t		speedMode;
	uint8_t		maxStepSpeed;
	int16_t		outF;
	int16_t		outP;
	int16_t		outI;
}	DRIVE_STATUS ;

typedef struct {
	uint8_t		seqNum;
	uint8_t		estop;
	int8_t		leftFront;
	int8_t		rightFront;
	int8_t		leftBack;
	int8_t		rightBack;
} REMOTE_MESSAGE ;

void 		rx_task(void *arg);
void 		tx_task(void *arg);
void		setupPort(DRIVE_STATUS * status, char * name, int portNum);

void 		displayBuffer(DRIVE_STATUS * status);
void		saveResponse(DRIVE_STATUS * status);
void		displayStatus(DRIVE_STATUS * status);
void		checkRXBuffer(DRIVE_STATUS * status);
void		motionPerformance(void);

int 		sendData(DRIVE_STATUS * status, uint8_t * buffer, int len);
uint16_t 	CalcCRC(uint8_t *ptr);
int8_t		bytesToInt8 (uint8_t * ndata);
int16_t		bytesToInt16 (uint8_t * ndata);
uint16_t 	bytesToUInt16 (uint8_t * ndata);
int32_t 	bytesToInt32 (uint8_t * ndata) ;

#endif /* HUGS_H_ */
