 /**
  * @file PIP_Active_IO_Handler.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that manages network I/O in a dedicated thread
*/


#ifndef _PIP_ACTIVE_IO_HANDLER_H_
#define _PIP_ACTIVE_IO_HANDLER_H_

#include "ace/PIP_IO_Handler.h"

/**
 * @class ACE_PIP_Active_IO_Handler
 *
 * @brief Performs network I/O in a dedicated thread
 *
 * @author John Moore <ljohn7@gmail.com>
 */
class ACE_Export ACE_PIP_Active_IO_Handler : 
   public ACE_PIP_IO_Handler
{
  public:
  
   /// Constructor
   ACE_PIP_Active_IO_Handler ();
   
   /// Enqueue a message to be sent
   virtual int put_message (ACE_PIP_Protocol_Message* message);

   /// Closes all remote connections.
   virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

   /// Performs message I/O
   virtual int svc();

   /// Shuts down the service. Result is handler deactivated and
   /// deleted
   void shutdown_svc();

   virtual int open(void* = 0);

 private:

   bool shutdown_;
   ACE_Mutex shutdown_lock_;
};

#endif /* _PIP_Active_IO_Handler_H_ */
