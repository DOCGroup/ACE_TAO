 /**
  * @file PIP_IO_Handler.cpp
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that manages network I/O
*/


#ifndef _PIP_REACTIVE_IO_HANDLER_H_
#define _PIP_REACTIVE_IO_HANDLER_H_


#include "ace/Message_Queue.h"
#include "ace/PIP_IO_Handler.h"
#include "ace/PIP_Messages.h"

/**
 * @class ACE_PIP_Reactive_IO_Handler
 *
 * @brief Performs reactive network I/O in
 *        the context of a distributed system
 *        employing the the priority inheritance
 *        protocol
 *
 * @author John Moore <ljohn7@gmail.com>
 */
class ACE_Export ACE_PIP_Reactive_IO_Handler :
   public ACE_PIP_IO_Handler
{
  public:
  
   /// Constructor
   ACE_PIP_Reactive_IO_Handler ();
   ~ACE_PIP_Reactive_IO_Handler();
   
   /// Closes all remote connections.
   virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

   /// Enqueue a message to be sent
   virtual int put_message (ACE_PIP_Protocol_Message* message);

 private:

};



#endif /* _PIP_Reactive_IO_Handler_H_ */


