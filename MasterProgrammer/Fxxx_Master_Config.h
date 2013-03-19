//-----------------------------------------------------------------------------
// Fxxx_Master_Config.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
//
//
// Target:         Any Silicon Labs C8051 MCU
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0 / 01SEP2010 (PKC)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#ifndef _FXXX_MASTER_CONFIG_H_
#define _FXXX_MASTER_CONFIG_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS - Available to all modules
//-----------------------------------------------------------------------------

// ---------------------------------
// General Constants
// ---------------------------------
#define SYSCLK 49000000UL/2              // SYSCLK frequency in Hz

// ---------------------------------
// Master Config Options
// ---------------------------------

#define TGT_COMM_RX_BUF_LENGTH 128     // (bytes) Max for TGT_Get_Info response
#define TGT_COMM_TX_BUF_LENGTH 1024    // (bytes) Max for TGT_Write_Flash command

#define SRC_COMM_RX_BUF_LENGTH 1026    // (bytes) Max for SRC_Get_NextPage response
#define SRC_COMM_TX_BUF_LENGTH 128     // (bytes) Max for SRC_Disp_TGT_Info command

#define NUM_SERIAL_BYTES   2           // Min: 1; Max: 7. Default: 2

#define EXAMPLE_MASTER_APP 0           // 0: Example app included; 1: Example not included

// ---------------------------------
// LED / Timer0 - Constants:
// ---------------------------------

#define LED_SLOW_RATE   2              // Rate of LED blinks per sec.
                                       // Minimum value is 1

#define LED_FAST_RATE   10             // Rate of LED blinks per sec.
                                       // Minimum value is 1

// Reload value for Timer0 assuming a 1:48 scaling factor for Timer0 clock
#define TIMER0_RL_SLOW_HIGH  (0xFFFF -((SYSCLK/48/LED_SLOW_RATE >> 8) & 0x00FF))
#define TIMER0_RL_SLOW_LOW   (0xFFFF -(SYSCLK/48/LED_SLOW_RATE & 0x00FF))

// Reload value for Timer0 assuming a 1:48 scaling factor for Timer0 clock
#define TIMER0_RL_FAST_HIGH  (0xFFFF -((SYSCLK/48/LED_FAST_RATE >> 8) & 0x00FF))
#define TIMER0_RL_FAST_LOW   (0xFFFF -(SYSCLK/48/LED_FAST_RATE & 0x00FF))

//-----------------------------------------------------------------------------
// Enums, Structs, Unions and Typedefs - Available to all modules
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// "Public" Global Variables - Externs - Available to all modules
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// "Public" Function Prototypes - Externs - Available to all modules
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#endif // #ifndef _FXXX_MASTER_CONFIG_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
