 /**
  * @file PIP_IO_Handler.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that manages network I/O
*/

#ifndef _PIP_IO_HANDLER_H_
#define _PIP_IO_HANDLER_H_

#include "ace/Message_Queue.h"
#include "ace/Mutex.h"
#include "ace/PIP_Messages.h"
#include "ace/Svc_Handler.h"
#include "ace/Thread_Mutex.h"

// Typedefs
typedef ACE_Message_Queue_Ex<ACE_PIP_Protocol_Message, ACE_NULL_SYNCH>
   PROTO_MESSAGE_QUEUE_TYPE;

/**
 * @class ACE_PIP_IO_Handler
 *
 * @brief Performs network I/O 
 *
 * @author John Moore <ljohn7@gmail.com>
 */
class ACE_Export ACE_PIP_IO_Handler : 
   public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_MT_SYNCH>
{
  public:
  
   /// Constructor
   ACE_PIP_IO_Handler ();
   
   /// Destructor
   virtual ~ACE_PIP_IO_Handler();

   /// Enqueue a message to be sent
   virtual int put_message (ACE_PIP_Protocol_Message* message) = 0;

   /// Initialize the priority of the handler, and inform the other end
   /// of the priority
   virtual void init(ACE_UINT32 site_id,
                     ACE_UINT32 destination_site_id,
                     ACE_UINT32 priority);
   
   /// Handles read event on socket.
   virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

   /// Handles read event on socket.
   virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

   /// Set the site_id
   void site_id(ACE_UINT32 site_id);

   /// Get the site id
   ACE_UINT32 site_id() const;

   /// Get the other end's site id
   ACE_UINT32 destination_site_id() const;
   
   /// Get the handler id
   void handler_id(ACE_UINT32 handler_id);

   /// Set the handler id
   ACE_UINT32 handler_id() const;

   /// Get the address of the remote site
   ACE_INET_Addr remote_address() const;

 protected:
   
   /// Reads priority from socket
   void extract_priority();
   
   // variables to track the state of the handler
   bool read_closed_;
   bool write_closed_;
   bool priority_set_;

   ACE_UINT32 site_id_;
   ACE_UINT32 handler_id_;
   ACE_UINT32 destination_site_id_;

   const ACE_Time_Value millisecond_;

   PROTO_MESSAGE_QUEUE_TYPE outgoing_message_queue_;
   ACE_Thread_Mutex big_lock_;
};

#if defined (__ACE_INLINE__)
#include "PIP_IO_Handler.inl"
#endif /* __ACE_INLINE__ */


#endif /* _PIP_IO_Handler_H_ */


