/*
 * File:   commands.c
 * Process commands from the RS232 bus
 * 
 * Author: Dave
 *
 * Created on August 9, 2017, 7:57 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "main.h" 
#include "mcc_generated_files/eusart1.h"
#include "mcc_generated_files/pin_manager.h"

void CommandProcessor (uint8_t ch)
{
   uint8_t  cmd = 0;
   uint8_t  relayState = 0;      // Digital I/O state
   uint8_t  channelNumber = 0;   // Digital I/O channel
   switch (ch)
   {
      case '?':
         printf ("Express 8 simple demo\n");
         break;
         
      case DIGITAL_IO:

         // Digital I/O
         // Get the next character which indicates the channel and the state
         
         while (eusart1RxCount == 0)
         {
            ;
         }
         
         cmd = EUSART1_Read ();
         
         // MSB is the state, rest is the relay number. Starting number is 16
         relayState = cmd >> 7;
         channelNumber = cmd & 0x7F;

         // 0 -> 8 are switch inputs
         // 0x10 -> 0x18 are relays
         // 0x20 -> 0x28 are LED
         // Just handling a relay for now
         if (channelNumber >= 0x10)
         {
            switch (channelNumber)
            {
               case 16:
               case 32:
                  if (relayState == 1)
                  {
                     IO_RA4_SetHigh ();
                  }
                  else
                  {
                     IO_RA4_SetLow ();
                  }

                  break;

               case 17:
               case 33:
                  if (relayState == 1)
                  {
                     IO_RA5_SetHigh ();
                  }
                  else
                  {
                     IO_RA5_SetLow ();
                  }
                  break;

               default:

                  // Invalid relay
                  break;
            }
            
            // printf ("Ch %u\n", channelNumber);
         }
         
         break;

      default:
         break;
   }
}
