/*
 * hugs.h
 *
 *  Created on: Mar 20, 2020
 *      Author: Phil
 */

#ifndef MAIN_HUGSIO_H_
#define MAIN_HUGSIO_H_

extern int16_t	leftDrive;
extern int16_t	rightDrive;

void	start_HUGSIO_tasks();
void    calculateMotion(void);
void	int16ToBytes(int16_t num, uint8_t * ndata);
int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP);
void    calculateMotion(void);
int16_t limitSpeed(int16_t targetFP, int16_t limitedFP, int16_t accelFP);



#endif /* MAIN_HUGSIO_H_ */
