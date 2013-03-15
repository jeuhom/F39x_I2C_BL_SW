//-----------------------------------------------------------------------------
// Fnnn_Flash.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fnnn_Flash.c
// nnn => Fill-in with the MCU family part number.
// Examples: F50x_Flash.c, F34x_Flash.c
// 
// Note: Typically, the flash primitive functions from application note AN201
// can be used here with minimal modifications. FLASH_ByteRead is needed only
// for code-banked devices.
//
// Note: Typically, the flash primitive functions from application note AN201
// can be used here with minimal modifications. FLASH_ByteRead is needed only
// for code-banked devices.
//
// Note: Scratchpad memory access is NOT supported.
//
// Target:         C8051Fxxx (Any Silicon Labs Flash MCU)
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0 / 01SEP2010 (PKC)
//    -Initial Revision
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <compiler_defs.h>
#include "Fxxx_SFR_Defs_Wrapper.h"     // This header file will include the
                                       // real MCU register definition file

#include "Fxxx_Target_Config.h"
#include "Fxxx_Target_Interface.h"

#include "Fxxx_TargetBL_Config.h"
#include "Fxxx_TargetBL_Interface.h"
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------


//=============================================================================
// Function Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// FLASH_Modify
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) U16 addr - address of the byte to write to
//   2) U8 byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr> if PSCTL == 1.
// This routine erases the FLASH page containing the linear FLASH address is
// PSCTL == 3.
//-----------------------------------------------------------------------------
void FLASH_Modify (U16 addr, U8 byte)
{

   // *** Device-Specific *** 
   // TODO: Add this based on MCU.

   // ** IMPORTANT ** When modifying flash primitive functions to fit in this 
   // template, it is important to remove the literal flash key values and use 
   // the global variables Flash_Key0 and Flash_Key1 instead.  

//   U8 xdata * data pwrite;             // FLASH write pointer

//   pwrite = (U8 xdata *) addr;

   VDM0CN = 0xA0;                      // Enable VDD monitor and high threshold

   Set_RSTSRC_002h ();                 // Enable VDD monitor as a reset source

   FLKEY  = Flash_Key0;                // Key Sequence 1

   FLKEY  = Flash_Key1;                // Key Sequence 2

   *((U8 xdata *) addr) = byte;
//   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x03;                     // PSWE = 0 which disable writes
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------