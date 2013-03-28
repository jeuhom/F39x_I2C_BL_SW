//-----------------------------------------------------------------------------
// Fnnn_TargetBL_DevSpecific.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fnnn_TargetBL_DevSpecific.c
// nnn => Fill-in with the MCU family part number.
// Examples: F50x_TargetBL_DevSpecific.c, F34x_TargetBL_DevSpecific.c
//
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

#include "Fxxx_BL131_SMB0_Interface.h"

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

// ------------------------------------------
// *** Device-Specific *** 
// Change this as necessary based on MCU
// ------------------------------------------

// Bit masks for the RSTSRC SFR
#define PORSF  0x02
#define FERROR 0x40

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------

//=============================================================================
// Support Function Definitions
//=============================================================================
// These support functions write one SFR to a fixed value.
// They help keep the code below the 0x1FF limit by fitting into the gaps
// between interrupt vectors.

#if ((SYSCLK/SMB0_FREQUENCY/3) < 255)
#define SCALE 1
void Set_CKCON_008h (void)
{
   CKCON = 0x08;                   // Timer1 clock source = SYSCLK
}

#elif ((SYSCLK/SMB0_FREQUENCY/4/3) < 255)
#define SCALE 4
void Set_CKCON_001h (void)
{
   CKCON = 0x01;                   // Timer1 clock source = SYSCLK / 4
}
#endif


void Configure_Timer1 (void)
{
   TH1 = -(SYSCLK/SMB0_FREQUENCY/SCALE/3);
   TL1 = TH1;                          // Init Timer1
   TR1 = 1;                            // Timer1 enabled
}

//=============================================================================
// Function Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Init_Device
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//
//-----------------------------------------------------------------------------
void Init_Device(void)
{
	PCA0MD &= ~0x40;  // Disable Watchdog

	// Initialize variables here so that RAM contents are not disturbed on a
	// non-bootloader reset

	VDM0CN = 0x80;                      // Enable VDD monitor and early warning
	RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   // Initialize port I/O
    P0SKIP = 0x03;
    XBR0 = 0x04;                        // Route SMBus signals to port pins
    XBR1 = 0x40;                        // Enable crossbar

    // Initialize SMBus
    SMB0CF = 0x95;                     // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SMBus;

   // Initialize Timer 1 as SMBus clock source
   // Make sure the Timer can produce the appropriate frequency in 8-bit mode
   // Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
   // settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB0_FREQUENCY/3) < 255)
   Set_CKCON_008h ();                  // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB0_FREQUENCY/4/3) < 255)
    Set_CKCON_001h ();
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   Configure_Timer1 ();
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
