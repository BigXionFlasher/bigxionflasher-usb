/* BigXionFlasher.c */
/* ====================================================================
 * Copyright (c) 2011-2017 by Thomas König <info@bigxionflasher.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the
 *    BigXionFlasher Project. (http://www.bigxionflasher.org/)"
 *
 * 4. The name "BigXionFlasher" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    info@bigxionflasher.org.
 *
 * 5. Products derived from this software may not be called "BigXionFlasher"
 *    nor may "BigXionFlasher" appear in their names without prior written
 *    permission of the BigXionFlasher Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the
 *    BigXionFlasher Project. (http://www.bigxionflasher.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE BigXionFlasher PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE BigXionFlasher PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#ifdef __WIN32__
 #include <conio.h>
 #define DEVICE_OPEN NULL
 #define TREIBER_NAME "mhstcan.dll"
 #define _NL "\n\r"
 #define _DEGREE_SIGN "o"
#else
 #define DEVICE_OPEN NULL
 #define TREIBER_NAME "libmhstcan.so"
 #define _NL "\n"
 #define _DEGREE_SIGN "°"
#endif
#include "can_drv.h"

#define __DOSTR(v) #v
#define __STR(v) __DOSTR(v)

#define __BXF_VERSION__ "V 0.2.4 rev. 97"

#define UNLIMITED_SPEED_VALUE			70 /* Km/h */
#define UNLIMITED_MIN_SPEED_VALUE		30 /* Km/h */
#define MAX_THROTTLE_SPEED_VALUE		70 /* Km/h */

#include "registers.h"

#define TIMEOUT_VALUE					80
#define TIMEOUT_US						10000 // 10ms

#define doSleep(x) usleep(x*1000)

int gAssistInitLevel = -1, gPrintSystemSettings = 0, gSkipShutdown = 0, gPowerOff = 0, gConsoleSetSlaveMode = 1, gNoSerialNumbers = 0, gSetMountainCap = -1, gSetWheelCircumference = 0;
double gSetSpeedLimit = -1, gSetMinSpeedLimit = -1, gSetThrottleSpeedLimit = -1;

char *getNodeName(unsigned char id)
{
	if (id == CONSOLE)
		return "console";
	else if (id == BATTERY)
		return "battery";
	else if (id == MOTOR)
		return "motor";
	else if (id == BIB)
		return "bib";
	else
		return "UNKNOWN";
}

void setValue(unsigned char receipient, unsigned char reg, unsigned char value)
{
	struct TCanMsg msg;
	int timeout = TIMEOUT_VALUE;

	msg.MsgFlags = 0L;
	msg.Id = receipient;
	msg.MsgLen = 4;
	msg.MsgData[0] = 0x00;
	msg.MsgData[1] = reg;
	msg.MsgData[2] = 0x00;
	msg.MsgData[3] = value;

	CanTransmit(0, &msg, 1);

	while(timeout-- && CanTransmitGetCount(0))
		usleep(TIMEOUT_US);

	if (timeout == -1)
		printf("error: could not send value to %s" _NL, getNodeName(receipient));
}

unsigned int getValue(unsigned char receipient, unsigned char reg)
{
	struct TCanMsg msg;
	int err, retry = 20;
	int timeout = TIMEOUT_VALUE;
	
	msg.MsgFlags = 0L;
	msg.Id = receipient;
	msg.MsgLen = 2;
	msg.MsgData[0] = 0x00;
	msg.MsgData[1] = reg;

	CanTransmit(0, &msg, 1);

	while(timeout-- && CanTransmitGetCount(0))
		usleep(TIMEOUT_US);

	if (timeout == -1)
		printf("error: could not send value to node %s" _NL, getNodeName(receipient));

retry:
	timeout = TIMEOUT_VALUE;
	while(timeout-- && !CanReceiveGetCount(0))
		usleep(TIMEOUT_US);

	if (timeout == -1) {
		printf("error: no response from node %s" _NL, getNodeName(receipient));
		return 0;
	}

	if ((err = CanReceive(0, &msg, 1)) > 0) {
		if (--retry && (msg.Id != BIB || msg.MsgLen != 4 || msg.MsgData[1] != reg))
			goto retry;

		if (!retry) {
			printf("error: no response from node %s to %s" _NL, getNodeName(receipient), getNodeName(BIB));
			return 0;
		}

		return (unsigned int) msg.MsgData[3];
	} else
		printf("Error: %d" _NL, err);

	return 0;
}

void setSpeedLimit(double speed)
{
	int limit = (speed != 0);

	if (!speed)
		speed = UNLIMITED_SPEED_VALUE;
	setValue(CONSOLE, CONSOLE_ASSIST_MAXSPEEDFLAG, limit);
	setValue(CONSOLE, CONSOLE_ASSIST_MAXSPEED_HI, ((int)(speed * 10)) >> 8);
	setValue(CONSOLE, CONSOLE_ASSIST_MAXSPEED_LO, ((int)(speed * 10)) & 0xff);
	setValue(MOTOR, MOTOR_PROTECT_UNLOCK, MOTOR_PROTECT_UNLOCK_KEY);
	setValue(MOTOR, MOTOR_ASSIST_MAXSPEED, (int)speed);
}

void setWheelCircumference(unsigned short circumference)
{
	if (!circumference)
		return;

	setValue(CONSOLE, CONSOLE_GEOMETRY_CIRC_HI, (int) (circumference >> 8));
	setValue(CONSOLE, CONSOLE_GEOMETRY_CIRC_LO, (int) (circumference & 0xff));
	setValue(MOTOR, MOTOR_PROTECT_UNLOCK, MOTOR_PROTECT_UNLOCK_KEY);
	setValue(MOTOR, MOTOR_GEOMETRY_CIRC_HI, (int) (circumference >> 8));
	setValue(MOTOR, MOTOR_GEOMETRY_CIRC_LO, (int) (circumference & 0xff));
}

void setMinSpeedLimit(double speed)
{
	char limit = (speed != 0);

	setValue(CONSOLE, CONSOLE_ASSIST_MINSPEEDFLAG, limit);
	setValue(CONSOLE, CONSOLE_ASSIST_MINSPEED, (int)(speed * 10));
}

void setThrottleSpeedLimit(double speed)
{
	int limit = (speed != 0);

	if (!speed)
		speed = MAX_THROTTLE_SPEED_VALUE;

	setValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEEDFLAG, limit);
	setValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEED_HI, ((int)(speed * 10)) >> 8);
	setValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEED_LO, ((int)(speed * 10)) & 0xff);
}

void printBatteryStats()
{
	int channel = 1, packSerial, packParallel;
	
	printf( " balancer enabled ...: %s" _NL _NL, (getValue(BATTERY, BATTERY_CELLMON_BALANCERENABLED != 0) ? "yes" : "no"));

	packSerial = getValue(BATTERY, BATTERY_CONFIG_PACKSERIAL);
	packParallel = getValue(BATTERY, BATTERY_CONFIG_PACKPARALLEL);
	
	packSerial = (packSerial > 20) ? 0 : packSerial;
	packParallel = (packParallel > 20) ? 0 : packParallel;

	for (;channel <= packSerial; channel++) {
		setValue(BATTERY, BATTERY_CELLMON_CHANNELADDR, (int)0x80 + channel);
		printf(" voltage cell #%02d ...: %.3fV" _NL, channel,
			((getValue(BATTERY, BATTERY_CELLMON_CHANNELDATA_HI) << 8) + getValue(BATTERY,BATTERY_CELLMON_CHANNELDATA_LO)) * 0.001);
	}

	for (channel = 0 ; channel < packParallel ; channel ++)
		printf(" temperature pack #%02d: %d" _DEGREE_SIGN "C" _NL, channel + 1,
				getValue(BATTERY, BATTERY_STATUS_PACKTEMPERATURE1 + channel));
	
	printf(_NL);
}

void printChargeStats() {
	int channel = 1, totalChagres = 0, c;

	for (channel = 1 ; channel <= 10; channel++) {
		setValue(BATTERY, 0xf6, channel);
		c = (getValue(BATTERY, 0xf7) << 8) + getValue(BATTERY,0xf8);
		totalChagres += c;
		printf(" charge level @ %03d%% : %04d" _NL, channel*10, c);
	}

	printf(" total # of charges .: %04d" _NL _NL, totalChagres);
}

double getVoltageValue(unsigned char in, unsigned char reg)
{
	return (getValue(BATTERY, reg) + 20.8333) * 0.416667;
}

void usage(void) {
	printf( "usage:" _NL
			" -l <speedLimit> .......... set the speed limit to <speedLimit> (1 - " __STR(UNLIMITED_SPEED_VALUE) "), 0 = remove the limit" _NL
			" -m <minSpeedLimit> ....... set the minimum speed limit to <minSpeedLimit> (0 - " __STR(UNLIMITED_MIN_SPEED_VALUE) "), 0 = remove the limit" _NL
			" -t <throttleSpeedLimit> .. set the throttle speed limit to <throttleSpeedLimit> (0 - " __STR(MAX_THROTTLE_SPEED_VALUE) "), 0 = remove the limit" _NL
			" -a <assistLevel> ......... set the initial assist level after power on (0 - 4)" _NL
			" -o <level> ............... set the mountain cap level (0%% - 100%%), use 55%%" _NL
			" -c <wheel circumference> . set the wheel circumference (in mm)" _NL
			" -s ....................... print system settings overview" _NL
			" -p ....................... power off system" _NL
			" -n ....................... don't try to put console in slave mode" _NL
			" -x ....................... skip automatic system shutdown when setting new speed limit. (should not be used)" _NL
			" -i ....................... don't display private serial and part numbers" _NL
			" -h ....................... print this help screen" _NL _NL);
}

int parseOptions(int argc, char **argv)
{
	int oc;
	char odef[] = "l:t:m:sa:pnxio:c:h?";

	while((oc = getopt(argc,argv,odef)) != -1) {
		switch(oc) {
			case 'p':
				gPowerOff = 1;
				break;
			case 'x':
				gSkipShutdown = 1;
				break;
			case 'l':
				gSetSpeedLimit = atof(optarg);
				if (gSetSpeedLimit > UNLIMITED_SPEED_VALUE || gSetSpeedLimit < 0) {
					printf("error: speed limit %.2f is out of range. exiting..." _NL, gSetSpeedLimit);
					return -1;
				}
				break;
			case 't':
				gSetThrottleSpeedLimit = atof(optarg);
				if (gSetThrottleSpeedLimit > MAX_THROTTLE_SPEED_VALUE || gSetThrottleSpeedLimit < 0) {
					printf("error: throttle speed limit %.2f is out of range. exiting..." _NL, gSetThrottleSpeedLimit);
					return -1;
				}
				break;
			case 'm':
				gSetMinSpeedLimit = atof(optarg);
				if (gSetMinSpeedLimit > UNLIMITED_MIN_SPEED_VALUE || gSetMinSpeedLimit < 0) {
					printf("error: min speed limit %.2f is out of range. exiting..." _NL, gSetMinSpeedLimit);
					return -1;
				}
				break;
			case 'a':
				gAssistInitLevel = atoi(optarg);
				if (gAssistInitLevel > 4 || gAssistInitLevel < 0) {
					printf("error: initial assist level %d is out of range. exiting..." _NL, gAssistInitLevel);
					return -1;
				}
				break;
			case 'o':
				gSetMountainCap = atoi(optarg);
				if (gSetMountainCap > 100 || gSetMountainCap < 0) {
					printf("error: mountain cap level %d is out of range. exiting..." _NL, gSetMountainCap);
					return -1;
				}
				break;
			case 'c':
				gSetWheelCircumference = atoi(optarg);
				if (gSetWheelCircumference > 3000 || gSetWheelCircumference < 1000) {
					printf("error: wheel circumference %d is out of range. exiting..." _NL, gSetWheelCircumference);
					return -1;
				}
				break;
			case 'n':
				gConsoleSetSlaveMode = 0;
				break;
			case 'i':
				gNoSerialNumbers = 1;
				break;
			case 's':
				gPrintSystemSettings = 1;
				break;
			case 'h':
			case '?':
			default:
				usage();
				return -1;
		}
	}

	return 0;
}

void printSystemSettings()
{
	int hwVersion, swVersion, wheelCirc;
	char *sl;
	double speedLimit = 0;

	printf(_NL _NL);

	hwVersion = getValue(CONSOLE, CONSOLE_REF_HW);

	if (hwVersion == 0)
		printf("Console not responding" _NL _NL);
	else {
		swVersion = getValue(CONSOLE, CONSOLE_REF_SW);
		printf( "Console information:" _NL
				" hardware version ........: %02d" _NL
				" software version ........: %02d" _NL
				" assistance level ........: %d" _NL,
					hwVersion, swVersion,
					getValue(CONSOLE, CONSOLE_ASSIST_INITLEVEL)
				);

		if (!gNoSerialNumbers)
		printf( " part number .............: %05d" _NL
				" item number .............: %05d" _NL _NL,
					((getValue(CONSOLE, CONSOLE_SN_PN_HI) << 8) + getValue(CONSOLE, CONSOLE_SN_PN_LO)),
					((getValue(CONSOLE, CONSOLE_SN_ITEM_HI) << 8) + getValue(CONSOLE, CONSOLE_SN_ITEM_LO))
				);

		/* ASSIST speed limit */
		sl = getValue(CONSOLE, CONSOLE_ASSIST_MAXSPEEDFLAG) == 0 ? "no" : "yes";
		speedLimit = ((getValue(CONSOLE, CONSOLE_ASSIST_MAXSPEED_HI) << 8) + getValue(CONSOLE, CONSOLE_ASSIST_MAXSPEED_LO)) / (double)10;
		printf( " max limit enabled .......: %s" _NL
				" speed limit .............: %0.2f Km/h" _NL _NL, sl, speedLimit);

		/* MIN speed limit */
		sl = getValue(CONSOLE, CONSOLE_ASSIST_MINSPEEDFLAG) == 0 ? "no" : "yes";
		speedLimit = (getValue(CONSOLE, CONSOLE_ASSIST_MINSPEED)) / (double)10;
		printf( " min limit enabled .......: %s" _NL
				" min speed limit .........: %0.2f Km/h" _NL _NL, sl, speedLimit);

		/* THROTTLE speed limit */
		sl = getValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEEDFLAG) == 0 ? "no" : "yes";
		speedLimit = ((getValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEED_HI) << 8) + getValue(CONSOLE, CONSOLE_THROTTLE_MAXSPEED_LO)) / (double)10;
		printf( " throttle limit enabled ..: %s" _NL
				" throttle speed limit ....: %0.2f Km/h" _NL _NL, sl, speedLimit);

		/* WHEEL CIRCUMFERENCE */
		wheelCirc = (getValue(CONSOLE, CONSOLE_GEOMETRY_CIRC_HI) << 8) + getValue(CONSOLE, CONSOLE_GEOMETRY_CIRC_LO);
		printf( " wheel circumference .....: %d mm" _NL _NL, wheelCirc);

		if (swVersion >= 59)
			printf(
				" mountain cap ............: %0.2f%%" _NL,
					(getValue(CONSOLE, CONSOLE_ASSIST_MOUNTAINCAP) * 1.5625));

		printf( " odo .....................: %0.2f Km" _NL _NL,
					((getValue(CONSOLE, CONSOLE_STATS_ODO_1) << 24) +
					(getValue(CONSOLE, CONSOLE_STATS_ODO_2) << 16) +
					(getValue(CONSOLE, CONSOLE_STATS_ODO_3) << 8) +
					(getValue(CONSOLE, CONSOLE_STATS_ODO_4))) / (double)10
					);
	}

	hwVersion = getValue(BATTERY, BATTERY_REF_HW);
	if (hwVersion == 0)
		printf("Battery not responding" _NL _NL);
	else {
		printf( "Battery information:" _NL
				" hardware version ........: %02d" _NL
				" software version ........: %02d" _NL,
					hwVersion, getValue(BATTERY, BATTERY_REF_SW)
				);

		if (!gNoSerialNumbers)
		printf( " part number .............: %05d" _NL
				" item number .............: %05d" _NL,
					((getValue(BATTERY, BATTERY_SN_PN_HI) << 8) + getValue(BATTERY, BATTERY_SN_PN_LO)),
					((getValue(BATTERY, BATTERY_SN_ITEM_HI) << 8) + getValue(BATTERY, BATTERY_SN_ITEM_LO))
				);

		printf( " voltage .................: %0.2fV" _NL
				" battery level ...........: %0.2f%%" _NL
				" maximum voltage .........: %0.2f%%" _NL
				" minimum voltage .........: %0.2f%%" _NL
				" mean voltage ............: %0.2f%%" _NL
				" resets ..................: %0d" _NL
				" ggjrCalib ...............: %0d" _NL
				" vctrlShorts .............: %0d" _NL
				" lmd .....................: %0.2fAh" _NL
				" cell capacity ...........: %0.2fAh" _NL _NL,
					((getValue(BATTERY, BATTERY_STATUS_VBATT_HI) << 8) + getValue(BATTERY, BATTERY_STATUS_VBATT_LO)) * 0.001,
					(getValue(BATTERY, BATTERY_STATUS_LEVEL) * 6.6667),
					getVoltageValue(BATTERY, BATTERY_STATS_VBATTMAX),
					getVoltageValue(BATTERY, BATTERY_STATS_VBATTMIN),
					getVoltageValue(BATTERY, BATTERY_STATS_VBATTMEAN),
					(getValue(BATTERY, BATTERY_STATS_RESET_HI) << 8) + getValue(BATTERY, BATTERY_STATS_RESET_LO),
					getValue(BATTERY, BATTERY_STSTS_GGJSRCALIB),
					getValue(BATTERY, BATTERY_STSTS_VCTRLSHORTS),
					((getValue(BATTERY, BATTERY_STATS_LMD_HI) << 8) + getValue(BATTERY, BATTERY_STATS_LMD_LO)) * 0.002142,
					((getValue(BATTERY, BATTERY_CONFIG_CELLCAPACITY_HI) << 8) + getValue(BATTERY, BATTERY_CONFIG_CELLCAPACITY_LO)) * 0.001
				);

		printf( " charge time worst .......: %0d" _NL
				" charge time mean ........: %0d" _NL
				" charge cycles ...........: %0d" _NL
				" full charge cycles ......: %0d" _NL
				" power cycles ............: %0d" _NL
				" battery temp max ........: %0d" _NL
				" battery temp min ........: %0d" _NL _NL,
					(getValue(BATTERY, BATTERY_STATS_CHARGETIMEWORST_HI) << 8) + getValue(BATTERY, BATTERY_STATS_CHARGETIMEWORST_LO),
					(getValue(BATTERY, BATTERY_STATS_CHARGETIMEMEAN_HI) << 8) + getValue(BATTERY, BATTERY_STATS_CHARGETIMEMEAN_LO),
					(getValue(BATTERY, BATTERY_STATS_BATTCYCLES_HI) << 8) + getValue(BATTERY, BATTERY_STATS_BATTCYCLES_LO),
					(getValue(BATTERY, BATTERY_STATS_BATTFULLCYCLES_HI) << 8) + getValue(BATTERY, BATTERY_STATS_BATTFULLCYCLES_LO),
					(getValue(BATTERY, BATTERY_STATS_POWERCYCLES_HI) << 8) + getValue(BATTERY, BATTERY_STATS_POWERCYCLES_HI),
					getValue(BATTERY, BATTERY_STATS_TBATTMAX),
					getValue(BATTERY, BATTERY_STATS_TBATTMIN)
				);

		printChargeStats();

		if (hwVersion >= 60)
			printBatteryStats();
		else
			printf(" no battery details supported by battery hardware #%d" _NL _NL, hwVersion);
	}

	hwVersion = getValue(MOTOR, MOTOR_REF_HW);
	if (hwVersion == 0)
		printf("Motor not responding" _NL _NL);
	else {
		printf( "Motor information:" _NL
				" hardware version ........: %02d" _NL
				" software version ........: %02d" _NL
				" temperature .............: %02d" _DEGREE_SIGN "C"_NL
				" speed limit .............: %02d Km/h" _NL,
					hwVersion, getValue(MOTOR, MOTOR_REF_SW),
					getValue(MOTOR, MOTOR_REALTIME_TEMP),
					getValue(MOTOR, MOTOR_ASSIST_MAXSPEED)
				);

		wheelCirc = (getValue(MOTOR, MOTOR_GEOMETRY_CIRC_HI) << 8) + getValue(MOTOR, MOTOR_GEOMETRY_CIRC_LO);
		printf( " wheel circumference .....: %d mm" _NL _NL, wheelCirc);

		if (!gNoSerialNumbers)
		printf( " part number .............: %05d" _NL
				" item number .............: %05d" _NL _NL,
					((getValue(MOTOR, MOTOR_SN_PN_HI) << 8) + getValue(MOTOR, MOTOR_SN_PN_LO)),
					((getValue(MOTOR, MOTOR_SN_ITEM_HI) << 8) + getValue(MOTOR, MOTOR_SN_ITEM_LO))
				);
	}
}

int main(int argc, char **argv)
{
	int err, doShutdown = 0, consoleInSlaveMode = 0;
	struct TDeviceStatus status;

	printf("BigXionFlasher USB " __BXF_VERSION__ _NL " (c) 2011-2013 by Thomas Koenig <info@bigxionflasher.org> - www.bigxionflasher.org"_NL _NL);

	if ((err=parseOptions(argc, argv) < 0))
		exit(1);

	if ((err = LoadDriver(TREIBER_NAME)) < 0) {
		printf("LoadDriver error: %d" _NL, err);
		goto error;
	}

	if ((err = CanInitDriver(NULL)) < 0) {
		printf("CanInitDriver error: %d" _NL, err);
		goto error;
	}

	if ((err = CanDeviceOpen(0, DEVICE_OPEN)) < 0) {
		printf("CanDeviceOpen error: %d" _NL, err);
		goto error;
	}

	CanSetSpeed(0, CAN_125K_BIT);
	CanSetMode(0, OP_CAN_START, CAN_CMD_ALL_CLEAR);
	CanGetDeviceStatus(0, &status);

	if (status.DrvStatus >= DRV_STATUS_CAN_OPEN) {
		if (status.CanStatus == CAN_STATUS_BUS_OFF) {
			printf("CAN Status BusOff" _NL);
			CanSetMode(0, OP_CAN_RESET, CAN_CMD_NONE);
		}
	} else {
		printf("error: could not open device" _NL);
		goto error;
	}

	consoleInSlaveMode = getValue(CONSOLE, CONSOLE_STATUS_SLAVE);
	if (consoleInSlaveMode) {
		printf("console already in salve mode. good!" _NL _NL);
	} else {
		if (gConsoleSetSlaveMode) {
			int retry = 20;

			printf("putting console in salve mode ... ");
			do {
				setValue(CONSOLE, CONSOLE_STATUS_SLAVE, 1);
				consoleInSlaveMode = getValue(CONSOLE, CONSOLE_STATUS_SLAVE);
				usleep(200000);
			} while(retry-- && !consoleInSlaveMode);

			doSleep(500); // give the console some time to settle
			printf("%s" _NL _NL, consoleInSlaveMode ? "done" : "failed");
		} else
			printf("console not in slave mode" _NL _NL);
	}

	if (gAssistInitLevel != -1) {
		printf("setting initial assistance level to %d" _NL, gAssistInitLevel);
		setValue(CONSOLE, CONSOLE_ASSIST_INITLEVEL, gAssistInitLevel);
	}

	if (gSetSpeedLimit > 0) {
		printf("set speed limit to %0.2f km/h" _NL, gSetSpeedLimit);
		setSpeedLimit(gSetSpeedLimit);
		doShutdown = 1;
	} else if (gSetSpeedLimit == 0) {
		printf("disable speed limit, drive carefully" _NL);
		setSpeedLimit(0);
		doShutdown = 1;
	}

	if (gSetMinSpeedLimit > 0) {
		printf("set minimal speed limit to %0.2f km/h" _NL, gSetMinSpeedLimit);
		setMinSpeedLimit(gSetMinSpeedLimit);
		doShutdown = 1;
	} else if (gSetMinSpeedLimit == 0) {
		printf("disable minimal speed limit, drive carefully" _NL);
		setMinSpeedLimit(0);
		doShutdown = 1;
	}

	if (gSetThrottleSpeedLimit > 0) {
		printf("set throttle speed limit to %0.2f km/h" _NL, gSetThrottleSpeedLimit);
		setThrottleSpeedLimit(gSetThrottleSpeedLimit);
		doShutdown = 1;
	} else if (gSetThrottleSpeedLimit == 0) {
		printf("disable throttle speed limit, drive carefully" _NL);
		setThrottleSpeedLimit(0);
		doShutdown = 1;
	}

	if (gSetMountainCap > 0) {
		printf("set mountain cap level to %0.2f%%" _NL, ((int)gSetMountainCap / 1.5625) * 1.5625);
		setValue(CONSOLE, CONSOLE_ASSIST_MOUNTAINCAP, gSetMountainCap / 1.5625);
	}

	if (gSetWheelCircumference > 0) {
		printf("set wheel circumference to %d" _NL, gSetWheelCircumference);
		setWheelCircumference(gSetWheelCircumference);
	}

	if (gPrintSystemSettings)
		printSystemSettings();

	if ((doShutdown && !gSkipShutdown) || gPowerOff) {
		doSleep(1000);
		printf("shutting down system." _NL);
		setValue(BATTERY, BATTERY_CONFIG_SHUTDOWN, 1);
	}

	CanDownDriver();
	UnloadDriver();

	return 0;

error:
	CanDownDriver();
	UnloadDriver();

	return 1;

}
