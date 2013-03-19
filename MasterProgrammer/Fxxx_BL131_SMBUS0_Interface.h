//-----------------------------------------------------------------------------
// Fxxx_BL131_SMBUS0_Interface.h
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fxxx_BLddd_CommType_Interface.h
// xxx => Leave as-is; this means the file is compatible with all C8051F devices.
// ddd => Fill-in decimal value of Bootloader Type; The macro 'TGT_BL_TYPE' used
//        in the BL and App InfoBlocks should be defined as this value elsewhere.
//        Note: BL type values 000 and 128 through 255 are reserved for use by 
//        standardized interfaces defined by Silicon Labs; If you make any 
//        modifications to a standardized BL interface, please change the BL type 
//        number to one within the customized range of 001 through 127.
// CommType => Fill-in communication channel type.
// Examples: Fxxx_BL001_BitBang_Interface.h, Fxxx_BL129_CAN_Interface.h, Fxxx_BL128_UART_Interface.h
//
// In a Master project, define "_INCLUDE_MASTER_ONLY_ITEMS_" before including this file.
// In a Target BL/App project, define _INCLUDE_TARGET_ONLY_ITEMS_  before including this file.
// In a Target BL project, define "_INCLUDE_TARGET_BL_ONLY_ITEMS_" before including this file.
// In a Target App project, define "_INCLUDE_TARGET_APP_ONLY_ITEMS_" before including this file.
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

#ifndef _FXXX_BL131_SMBUS0_INTERFACE_H_
#define _FXXX_BL131_SMBUS0_INTERFACE_H_

//-----------------------------------------------------------------------------
// Global CONSTANTS - Available to all modules
//-----------------------------------------------------------------------------




//-----------------------------------------------------
// Constants common to Master / Target BL / Target App
//-----------------------------------------------------

// *** Add constants here to be accessed by the Master, Target BL & Target App ***

// Note: 'TGT_BL_TYPE' should be defined as 129 (0x81) elsewhere.

#define  AUTO_BAUD   0                 // 0 = disable autobaud
                                       // 1 = enable autobaud
#define  LIN_BAUD_RATE 19200           // Baud Rate to run LIN peripheral
                                       // 625 < LIN_BAUD_RATE < 20000

// Size of LIN Messages
// Note: Changes to this should be mirrored in 'TGT_BL_PKT_SIZE' in 'Fxxx_TargetBL_Config.h'
#define LIN_MESSAGE_SIZE   8            // Range is 1-8 bytes

// Note: The Message Objects and IDs have names with directions w.r.t. the Master 
// (so those will be reverse for the target)

// Instruction Set
#define ID_MASTER_TO_SLAVE_BL_CMD 0x04
#define ID_MASTER_TO_SLAVE_WR_CMD 0x05
#define ID_SLAVE_TO_MASTER_BL_RSP 0x06

//---------------------------------------------
// MASTER-only Constants
//---------------------------------------------

#ifdef _INCLUDE_MASTER_ONLY_ITEMS_

// *** Add constants here to be accessed only by the master ***

#define ID_MASTER_TX_BL_CMD      ID_MASTER_TO_SLAVE_BL_CMD // Transmitted by Master MCU for all BL commands
#define ID_MASTER_TX_WR_CMD      ID_MASTER_TO_SLAVE_WR_CMD // Transmitted by Master MCU for Write command
#define ID_MASTER_RX_BL_RSP      ID_SLAVE_TO_MASTER_BL_RSP // Received by Master MCU (responses from target MCU)

#endif

//-----------------------------------------------------
// Constants common to Target BL and Target App
//-----------------------------------------------------

#ifdef _INCLUDE_TARGET_ONLY_ITEMS_

// *** Add constants here to be accessed by the target BL & App ***

#define ID_TARGET_RX_BL_CMD      ID_MASTER_TO_SLAVE_BL_CMD // Transmitted by Master MCU for all BL commands
#define ID_TARGET_RX_WR_CMD      ID_MASTER_TO_SLAVE_WR_CMD // Transmitted by Master MCU for Write command
#define ID_TARGET_TX_BL_RSP      ID_SLAVE_TO_MASTER_BL_RSP // Received by Master MCU (responses from target MCU)

#endif

//---------------------------------------------
// TARGET BL-only Constants
//---------------------------------------------

#ifdef _INCLUDE_TARGET_BL_ONLY_ITEMS_

// *** Add constants here to be accessed only by the Target BL ***

#endif

//---------------------------------------------
// TARGET App-only Constants
//---------------------------------------------

#ifdef _INCLUDE_TARGET_APP_ONLY_ITEMS_

// *** Add constants here to be accessed only by the Target App ***

//-------------------------------
// Support Function Prototypes
//-------------------------------
// void Comm_Init_BL_Support_AddOn (void); (not provided for LIN)
U8 Enter_BL_Mode (void);
U8 LIN0_Check_BL_Command (void); // TODO: Replace with actual function proto - DONE

#endif


//-----------------------------------------------------------------------------
// Header file include-once preprocessor directive
//-----------------------------------------------------------------------------

#endif // #ifndef _FXXX_BL130_LIN0_INTERFACE_H_

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------