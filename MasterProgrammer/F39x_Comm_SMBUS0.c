//-----------------------------------------------------------------------------
// F50x_Comm_LIN0.c
//-----------------------------------------------------------------------------
// Copyright (C) 2010 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// [*** TEMPLATE INFORMATION ***]
// Filename: Fnnn_Comm_CommType.c
// nnn => Fill-in with the MCU family part number.
// CommType => Fill-in Comm interface name.
// Examples: F50x_Comm_CAN0.c, F34x_Comm_UART0.c
//
//
//
// Note: The following changes were made to this file so it can be used 
// in the Master project:
// 1. The include header files were changed from 'TargetBL' to 'Master'.
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

#include "Fxxx_Master_Config.h"
#include "Fxxx_Master_Interface.h"

#include "Fxxx_BL131_SMBUS0_Interface.h"  

#if 0
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define RSP_OK 0x00

#define READ_LIN0_REG(addr, target) { LIN0ADR = addr; target = LIN0DAT; }
#define WRITE_LIN0_REG(addr1, data1) { LIN0ADR = addr1; LIN0DAT = data1; }

#if (SYSCLK < 8000000)                 // For the slowest baud rate
#error Minimum system clock needed to operate LIN peripheral is 8Mhz.
#endif

//-----------------------------------------------------------------------------
// LIN #defines
//-----------------------------------------------------------------------------

//------ Control register ------
#define  STREQ       0x01              // LIN Master Start transmission request
#define  WUPREQ      0x02              // Wake-Up request
#define  RSTERR      0x04              // Error reset request
#define  RSTINT      0x08              // Interrupt reset request
#define  DTACK       0x10              // Slave Data ACK
#define  TXRX        0x20              // Master selection of TX or RX mode
#define  SLEEP       0x40              // Slave flag set by master to inform
                                       // the peripheral that either a sleep
                                       // mode frame or bus idle timeout
                                       // was reached
#define  STOP        0x80              // Slave flag, to be set by the
                                       // application so no processing is to be
                                       // done until next SYNCH BREAK
//------ Status Register ------
#define  DONE        0x01              // Msg. processing completed
#define  WAKEUP      0x02              // Wake-Up signal
#define  ERROR       0x04              // Error detected
#define  LININTREQ   0x08              // Interrupt request
#define  DTREQ       0x10              // ID detected
#define  ABORT       0x20              // Abort requested
#define  IDLTOUT     0x40              // Time-out detected
#define  ACTIVE      0x80              // Interface active (communicating)

#if (AUTO_BAUD == 0)                   // If running in manual baud rate mode

 #define LIN_MULTIPLIER (U8) ((20000.0 / LIN_BAUD_RATE) - 1)

 #if (SYSCLK >= 16000000)
   #define LIN_PRESCALAR 1
   #define LIN_DIVIDER (U16) (SYSCLK / ((U32) 4 * (LIN_MULTIPLIER + 1) * (U32) LIN_BAUD_RATE))
 #else
   #define LIN_PRESCALAR 0
   #define LIN_DIVIDER (U16) (SYSCLK / ((U32) 2 * (LIN_MULTIPLIER + 1) * (U32) LIN_BAUD_RATE))
 #endif

#else                                  // If running in auto baud rate mode

   #if (SYSCLK >= 16000000)
      #define LIN_PRESCALAR 1
      #define LIN_DIVIDER (U16)((U32) SYSCLK / ((U32) 4 * (U32) 20000))
   #else
      #define LIN_PRESCALAR 0
      #define LIN_DIVIDER (U16)((U32) SYSCLK / ((U32) 2 * (U32) 20000))
   #endif

#endif

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
U8 LIN_Error = 0;

//-----------------------------------------------------------------------------
// Function Prototypes (Local)
//-----------------------------------------------------------------------------
void Comm_Init (void);
U8 Comm_Wait_For_Rx_Bytes (U16 numbytes_to_read);
void Comm_Send_Tx_Bytes (U16 numbytes_to_send);

U8 Comm_Wait_For_Rx_RSP_OK (void);
void Comm_Send_Tx_RSP_OK (void);

//=============================================================================
// Function Definitions
//=============================================================================

//-----------------------------------------------------------------------------
// Comm_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Intializes the LIN peripheral to slave mode.
//
// LIN is an asynchronous, serial communications interface. All LIN registers
// besides the control mode register (LIN0CF) are indirectly accessed;
// the address is specified in the LINADDR register which points to a specific
// data register which can be accessed through the LINDATA register.
// Waveforms and additional specifications can be found in the LIN consortium
// (http:\\www.lin-subbus.com).
//
//-----------------------------------------------------------------------------
void Comm_Init (void)
{
   // *** Comm-Specific / Device-Specific ***
   // TODO: Add code here to initialize the Comm Interface

   //volatile U16 divider;

   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   // Enable interface, configure the device for LIN Master, manual baud rate
   LIN0CF = 0x40;
   LIN0CF |= 0x80;

   SFRPAGE = ACTIVE_PAGE;
   // Load the least significant byte of the divider.
   WRITE_LIN0_REG(LIN0DIV, (U8) LIN_DIVIDER);

   // Load the multiplier, prescalar, and most significant bit of the divider.
   WRITE_LIN0_REG(LIN0MUL, (U8) ((LIN_PRESCALAR << 6) | (LIN_MULTIPLIER << 1) | (LIN_DIVIDER >> 8)));

   WRITE_LIN0_REG(LIN0SIZE, 0x80); // Enhanced checksum

   WRITE_LIN0_REG(LIN0CTRL, 0x0C); // Reset LININT and LIN0ERR

   //EIE1 |= 0x80;                       // Enable LIN interrupts
   // *** Do not enable LIN0 interrupts -- Polled-mode ***

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Comm_Wait_For_Rx_RSP_OK
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//-----------------------------------------------------------------------------
U8 Comm_Wait_For_Rx_RSP_OK (void)
{
   // *** Comm-Specific / Device-Specific ***
   // TODO: Add code here to read bytes from the comm interface and store them
   // in the "Rx_Buf" buffer (buffer variable defined elsewhere).
   // Return the 1st byet of Rx_Buf as the return value.
   // This should be a blocking function; wait until requested number of bytes
   // have been read from the comm interface.
   // Send an ack (RSP_OK) after receiving each packet (except for the last packet)

   U8 status;
   bit rx_complete_flag = 0;
   U8 error;
   static U8 num_error = 0;
   U8 rsp;

   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = ACTIVE_PAGE;

   LIN0ADR = LIN0CTRL;
   LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag

   while (rx_complete_flag == 0)
   {
      // Send a slave transmit request with the right ID
   
      // 1. Set data direction
      LIN0ADR = LIN0CTRL;
      LIN0DAT &= ~TXRX;           // Select to receive data

      // 2. Load 6-bit ID
      WRITE_LIN0_REG(LIN0ID, ID_MASTER_RX_BL_RSP);
   
      // 3. Load data length
      LIN0ADR = LIN0SIZE;
      LIN0DAT = (LIN0DAT & 0xF0) | LIN_MESSAGE_SIZE;  //transmit 8 data bytes

      // 4. Set STREQ (LIN0CTRL.0) to start message transfer
      LIN0ADR = LIN0CTRL;
      LIN0DAT |= STREQ;           // Start Request

      // 5. Wait for interrupt request
      LIN0ADR = LIN0ST; // Poll the LIN0 status register

      do
      {
         status = LIN0DAT;             // Read status
      }
      while ((status & LININTREQ) == 0);    // Wait until interrupt bit is set

      if(status & DONE)               // If ID field received
      {
         // Read 1 byte, ignore others
         READ_LIN0_REG(LIN0DT1, rsp);
         rx_complete_flag = 1;
      }
      else if (status & ERROR)         // If an error was detected
      {

         READ_LIN0_REG(LIN0ERR, error);   // Check the error register
         // Keep track of the number of errors received
         num_error++;
         // Handle your errors here, application dependent
      }

      LIN0ADR = LIN0CTRL;
      LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag
   }
  
   SFRPAGE = SFRPAGE_save;
   
   return (rsp); // Return the response code
}

//-----------------------------------------------------------------------------
// Comm_Wait_For_Rx_Bytes
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
//
//-----------------------------------------------------------------------------
U8 Comm_Wait_For_Rx_Bytes (U16 numbytes_to_read)
{
   // *** Comm-Specific / Device-Specific ***
   // TODO: Add code here to read bytes from the comm interface and store them
   // in the "Rx_Buf" buffer (buffer variable defined elsewhere).
   // Return the 1st byet of Rx_Buf as the return value.
   // This should be a blocking function; wait until requested number of bytes
   // have been read from the comm interface.
   // Send an ack (RSP_OK) after receiving each packet (except for the last packet)

   U8 status;
   U8 received_numbytes = 0;
   bit rx_complete_flag = 0;
   U8 error;
   static U8 num_error = 0;
   U8 i;

   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = ACTIVE_PAGE;

   while (received_numbytes < numbytes_to_read)
   {
      LIN0ADR = LIN0CTRL;
      LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag

      while (rx_complete_flag == 0)
      {
         // Send a slave transmit request with the right ID
      
         // 1. Set data direction
         LIN0ADR = LIN0CTRL;
         LIN0DAT &= ~TXRX;           // Select to receive data

         // 2. Load 6-bit ID
         WRITE_LIN0_REG(LIN0ID, ID_MASTER_RX_BL_RSP);
      
         // 3. Load data length
         LIN0ADR = LIN0SIZE;
         LIN0DAT = (LIN0DAT & 0xF0) | LIN_MESSAGE_SIZE;  //transmit 8 data bytes

         // 4. Set STREQ (LIN0CTRL.0) to start message transfer
         LIN0ADR = LIN0CTRL;
         LIN0DAT |= STREQ;           // Start Request

         // 5. Wait for interrupt request
         LIN0ADR = LIN0ST; // Poll the LIN0 status register

         do
         {
            status = LIN0DAT;             // Read status
         }
         while ((status & LININTREQ) == 0);    // Wait until interrupt bit is set

         if(status & DONE)               // If ID field received
         {
            // Read all 8 data bytes to *(Rx_Buf+], even though all bytes might not be valid
            for (i = 0; i < LIN_MESSAGE_SIZE; i++)
            {
               READ_LIN0_REG((LIN0DT1 + i), *(Rx_Buf + i + received_numbytes));
            }

            received_numbytes += LIN_MESSAGE_SIZE;
            rx_complete_flag = 1;
         }
         else if (status & ERROR)         // If an error was detected
         {

            READ_LIN0_REG(LIN0ERR, error);   // Check the error register
            // Keep track of the number of errors received
            num_error++;
            // Handle your errors here, application dependent
         }

         LIN0ADR = LIN0CTRL;
         LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag
      }

      rx_complete_flag = 0;

      if (received_numbytes < numbytes_to_read)
      {
         // For all packets except the last packet, 
         // send ack to master to get the next packet

         //*(Tx_Buf + 0) = RSP_OK;
         //Comm_Send_Tx_Bytes(1);

         Comm_Send_Tx_RSP_OK();
      }
   }
   
   SFRPAGE = SFRPAGE_save;
   
   return (*(Rx_Buf + 0)); // Return the BL command code, which is always at byte0
}

//-----------------------------------------------------------------------------
// Comm_Send_Tx_RSP_OK
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Send the data in the buffer using the message object. 
//
//-----------------------------------------------------------------------------
void Comm_Send_Tx_RSP_OK (void)
{
   // *** Comm-Specific / Device-Specific ***
   // TODO: Add code here to write bytes to the comm interface from
   // the "Tx_Buf" buffer (buffer variable defined elsewhere).
   // This should be a blocking function; wait until ALL of the requested number 
   // of bytes have been sent via the comm interface.

   U8 status;
   bit tx_complete_flag = 0;
   U8 error;
   static U8 num_error = 0;
      
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = ACTIVE_PAGE;

   LIN0ADR = LIN0CTRL;
   LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag

   // 1. Set data direction
   LIN0ADR = LIN0CTRL;
   LIN0DAT |= TXRX;           // Select to transmit data

   // 2. Load 6-bit ID
   if (TGT_Write_Flash_Data_Bytes == 0)
   {
      WRITE_LIN0_REG(LIN0ID, ID_MASTER_TX_BL_CMD);
   }
   else
   {
      WRITE_LIN0_REG(LIN0ID, ID_MASTER_TX_WR_CMD);
   }
   
   // 3. Load data length
   LIN0ADR = LIN0SIZE;
   LIN0DAT = (LIN0DAT & 0xF0) | LIN_MESSAGE_SIZE;  //transmit 8 data bytes

   // 4. Load data bytes
   WRITE_LIN0_REG(LIN0DT1, RSP_OK); // other bytes are "don't cares"

   // 5. Set STREQ (LIN0CTRL.0) to start message transfer
   LIN0ADR = LIN0CTRL;
   LIN0DAT |= STREQ;           // Start Request

   // 6. Wait for interrupt request
   while (tx_complete_flag == 0)
   {  
      LIN0ADR = LIN0ST; // Poll the LIN0 status register

      do
      {
         status = LIN0DAT;             // Read status
      }
      while ((status & LININTREQ) == 0);    // Wait until interrupt bit is set

      // 7. If DONE is set, that means the transfer completed successfully
      if(status & DONE)               // If ID field received
      {
         // Transmission complete
         tx_complete_flag = 1;
      }
      else if (status & ERROR)         // If an error was detected
      {

         LIN0ADR = LIN0ERR;
         error = LIN0DAT;              // Check the error register
         // Keep track of the number of errors received
         num_error++;
         // Handle your errors here, application dependent

      }

      LIN0ADR = LIN0CTRL;
      LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag
   }

   SFRPAGE = SFRPAGE_save;             // Restore SFRPAGE
}


//-----------------------------------------------------------------------------
// Comm_Send_Tx_Bytes
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Send the data in the buffer using the message object. 
//
//-----------------------------------------------------------------------------
void Comm_Send_Tx_Bytes (U16 numbytes_to_send)
{
   // *** Comm-Specific / Device-Specific ***
   // TODO: Add code here to write bytes to the comm interface from
   // the "Tx_Buf" buffer (buffer variable defined elsewhere).
   // This should be a blocking function; wait until ALL of the requested number 
   // of bytes have been sent via the comm interface.

   // Wait for an ack (RSP_OK) after sending each packet (except for the last packet)

   U16 sent_numbytes = 0;
   U8 status;
   bit tx_complete_flag = 0;
   bit resend_last_req = 0;
   U8 error;
   static U8 num_error = 0;
   U8 i;
      
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = ACTIVE_PAGE;

   while (sent_numbytes < numbytes_to_send)
   {
      LIN0ADR = LIN0CTRL;
      LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag

      if (resend_last_req == 0)
      {
         // 1. Set data direction
         LIN0ADR = LIN0CTRL;
         LIN0DAT |= TXRX;           // Select to transmit data

         // 2. Load 6-bit ID
         if (TGT_Write_Flash_Data_Bytes == 0)
         {
            WRITE_LIN0_REG(LIN0ID, ID_MASTER_TX_BL_CMD);
         }
         else
         {
            WRITE_LIN0_REG(LIN0ID, ID_MASTER_TX_WR_CMD);
         }
      
         // 3. Load data length
         LIN0ADR = LIN0SIZE;
         LIN0DAT = (LIN0DAT & 0xF0) | LIN_MESSAGE_SIZE;  //transmit 8 data bytes

         // 4. Load data bytes
         for (i = 0; i < LIN_MESSAGE_SIZE; i++)
         {
            WRITE_LIN0_REG((LIN0DT1 + i), *(Tx_Buf + i + sent_numbytes));
         }
      }
      
      // 5. Set STREQ (LIN0CTRL.0) to start message transfer
      LIN0ADR = LIN0CTRL;
      LIN0DAT |= STREQ;           // Start Request

      // 6. Wait for interrupt request
      while (tx_complete_flag == 0)
      {  
         LIN0ADR = LIN0ST; // Poll the LIN0 status register

         do
         {
            status = LIN0DAT;             // Read status
         }
         while ((status & LININTREQ) == 0);    // Wait until interrupt bit is set

         // 7. If DONE is set, that means the transfer completed successfully
         if(status & DONE)               // If ID field received
         {
            // Transmission complete

            sent_numbytes += LIN_MESSAGE_SIZE;     
            tx_complete_flag = 1;
         }
         else if (status & ERROR)         // If an error was detected
         {

            LIN0ADR = LIN0ERR;
            error = LIN0DAT;              // Check the error register
            // Keep track of the number of errors received
            num_error++;
            // Handle your errors here, application dependent

            break;
         }

         LIN0ADR = LIN0CTRL;
         LIN0DAT |= 0x0C;                    // Reset interrupt request, error flag
      }

      if (tx_complete_flag == 1)
      {
         // Transmission was successful
         tx_complete_flag = 0;
         resend_last_req = 0;

         if (sent_numbytes < numbytes_to_send)
         {
            // For all packets except the last packet, 
            // wait for ack from master before sending next packet

            //if (Comm_Wait_For_Rx_Bytes(1) != RSP_OK)
            if (Comm_Wait_For_Rx_RSP_OK() != RSP_OK)
            {
               // Error: Unexpected response
            }
         }
      }
      else
      {
         // Transmission was unsuccessful
         resend_last_req = 1;
      }
   }

   SFRPAGE = SFRPAGE_save;             // Restore SFRPAGE
}
#endif
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
