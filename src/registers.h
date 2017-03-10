/* registers.h
 * ====================================================================
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

#ifndef REGISTERS_H_
#define REGISTERS_H_

#define BIB								0x58
#define CONSOLE_STANDARD_MODE			0x08

#define CONSOLE							0x48 // (CAN ID in slave mode)
#define CONSOLE_REF_SW					0xa3
#define CONSOLE_REF_HW					0xd0
#define CONSOLE_SN_PN_HI				0x75
#define CONSOLE_SN_PN_LO				0x76
#define CONSOLE_SN_ITEM_HI				0x77
#define CONSOLE_SN_ITEM_LO				0x78
#define CONSOLE_GEOMETRY_CIRC_HI		0x81
#define CONSOLE_GEOMETRY_CIRC_LO		0x82
#define CONSOLE_ASSIST_MAXSPEEDFLAG		0x83
#define CONSOLE_ASSIST_MAXSPEED_HI		0x84
#define CONSOLE_ASSIST_MAXSPEED_LO		0x85
#define CONSOLE_ASSIST_MINSPEEDFLAG		0x89
#define CONSOLE_ASSIST_MINSPEED			0x8a
#define CONSOLE_ASSIST_INITLEVEL		0xb4
#define CONSOLE_ASSIST_MOUNTAINCAP		0xc6
#define CONSOLE_STATUS_SLAVE			0xd1
#define CONSOLE_THROTTLE_MAXSPEEDFLAG	0x86
#define CONSOLE_THROTTLE_MAXSPEED_HI	0x87
#define CONSOLE_THROTTLE_MAXSPEED_LO	0x88
#define CONSOLE_STATS_ODO_1				0x64
#define CONSOLE_STATS_ODO_2				0x65
#define CONSOLE_STATS_ODO_3				0x66
#define CONSOLE_STATS_ODO_4				0x67

#define BATTERY							0x50
#define BATTERY_REF_SW					0x3c
#define BATTERY_REF_HW					0x3b
#define BATTERY_SN_PN_HI				0x75
#define BATTERY_SN_PN_LO				0x76
#define BATTERY_SN_ITEM_HI				0x77
#define BATTERY_SN_ITEM_LO				0x78
#define BATTERY_CONFIG_SHUTDOWN			0x25
#define BATTERY_CONFIG_PACKSERIAL		0xae
#define BATTERY_CONFIG_PACKPARALLEL		0xaf
#define BATTERY_CONFIG_CELLCAPACITY_HI	0xfd
#define BATTERY_CONFIG_CELLCAPACITY_LO	0xfe

#define BATTERY_STATUS_VBATT_HI			0xa6
#define BATTERY_STATUS_VBATT_LO			0xa7
#define BATTERY_STATUS_LEVEL			0x61
#define BATTERY_STATUS_PACKTEMPERATURE1	0x66

#define BATTERY_CELLMON_CHANNELADDR		0x6c
#define BATTERY_CELLMON_CHANNELDATA_HI	0x6d
#define BATTERY_CELLMON_CHANNELDATA_LO	0x6e
#define BATTERY_CELLMON_BALANCERENABLED	0x65

#define BATTERY_STATS_LMD_HI			0xd5
#define BATTERY_STATS_LMD_LO			0xd6
#define BATTERY_STATS_RESET_HI			0x48
#define BATTERY_STATS_RESET_LO			0x49
#define BATTERY_STATS_CHARGETIMEWORST_HI	0x8c
#define BATTERY_STATS_CHARGETIMEWORST_LO	0x8d
#define BATTERY_STATS_CHARGETIMEMEAN_HI	0x8a
#define BATTERY_STATS_CHARGETIMEMEAN_LO	0x8b
#define BATTERY_STATS_BATTCYCLES_HI		0x8e
#define BATTERY_STATS_BATTCYCLES_LO		0x8f
#define BATTERY_STATS_BATTFULLCYCLES_HI	0x92
#define BATTERY_STATS_BATTFULLCYCLES_LO	0x93
#define BATTERY_STATS_POWERCYCLES_HI	0x96
#define BATTERY_STATS_POWERCYCLES_LO	0x97
#define BATTERY_STATS_VBATTMAX			0x98
#define BATTERY_STATS_VBATTMIN			0x99
#define BATTERY_STATS_VBATTMEAN			0x33
#define BATTERY_STATS_TBATTMAX			0x9c
#define BATTERY_STATS_TBATTMIN			0x9d
#define BATTERY_STSTS_GGJSRCALIB		0xec
#define BATTERY_STSTS_VCTRLSHORTS		0x9e

#define MOTOR							0x60
#define MOTOR_REF_SW					0x20
#define MOTOR_REF_HW					0x19
#define MOTOR_GEOMETRY_CIRC_HI			0x44
#define MOTOR_GEOMETRY_CIRC_LO			0x45
#define MOTOR_SN_PN_HI					0x62
#define MOTOR_SN_PN_LO					0x63
#define MOTOR_SN_ITEM_HI				0x60
#define MOTOR_SN_ITEM_LO				0x61
#define MOTOR_PROTECT_UNLOCK			0xa5
#define MOTOR_ASSIST_MAXSPEED			0x8b
#define MOTOR_REALTIME_TEMP				0x16

#define MOTOR_PROTECT_UNLOCK_KEY		0xaa


#endif /* REGISTERS_H_ */
