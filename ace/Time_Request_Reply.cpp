// Time_Request_Reply.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Time_Request_Reply.h"

// Default "do nothing" constructor.

ACE_Time_Request::ACE_Time_Request (void)
{
  ACE_TRACE ("ACE_Time_Request::ACE_Time_Request");
}

// Create a ACE_Time_Request message.  

ACE_Time_Request::ACE_Time_Request (ACE_UINT32 t, // Type of request.
				    const ACE_UINT32 time,
				    ACE_Time_Value *timeout) // Max time waiting for request.
{
  ACE_TRACE ("ACE_Time_Request::ACE_Time_Request");
  this->msg_type (t);

  // If timeout is a NULL pointer, then block forever...
  if (timeout == 0)
    {
      this->transfer_.block_forever_ = 1;
      this->transfer_.sec_timeout_   = 0;
      this->transfer_.usec_timeout_  = 0;
    }
  else // Do a "timed wait."
    {
      this->block_forever (0);
      // Keep track of how long client is willing to wait.
      this->transfer_.sec_timeout_ = timeout->sec ();
      this->transfer_.usec_timeout_ = timeout->usec ();
    }

  // Copy time into request
  this->time_ = this->transfer_.time_ = time;
}

// Initialize length_ in order to avoid problems with byte-ordering
void
ACE_Time_Request::init (void)
{
  ACE_TRACE ("ACE_Time_Request::init");
//  this->length (sizeof this->transfer_);
}

// Get the fixed size of message
ssize_t
ACE_Time_Request::size (void) const
{
  ACE_TRACE ("ACE_Time_Request::size");
  return sizeof (this->transfer_);
}

// = Set/get the type of the message.
ACE_UINT32 
ACE_Time_Request::msg_type (void) const
{
  ACE_TRACE ("ACE_Time_Request::msg_type");
  return this->transfer_.msg_type_;
}

void 
ACE_Time_Request::msg_type (ACE_UINT32 t)
{
  ACE_TRACE ("ACE_Time_Request::msg_type");
  this->transfer_.msg_type_ = t;
}

// = Set/get the blocking semantics.
ACE_UINT32
ACE_Time_Request::block_forever (void) const
{
  ACE_TRACE ("ACE_Time_Request::block_forever");
  return this->transfer_.block_forever_;
}

void 
ACE_Time_Request::block_forever (ACE_UINT32 bs)
{
  ACE_TRACE ("ACE_Time_Request::block_forever");
  this->transfer_.block_forever_ = bs;
}

// = Set/get the timeout.
ACE_Time_Value
ACE_Time_Request::timeout (void) const
{
  ACE_TRACE ("ACE_Time_Request::timeout");
  return ACE_Time_Value (this->transfer_.sec_timeout_, this->transfer_.usec_timeout_);
}

void 
ACE_Time_Request::timeout (const ACE_Time_Value timeout)
{
  ACE_TRACE ("ACE_Time_Request::timeout");
  this->transfer_.sec_timeout_  = timeout.sec ();
  this->transfer_.usec_timeout_ = timeout.usec ();
}

// = Set/get the time
ACE_UINT32
ACE_Time_Request::time (void) const
{
  ACE_TRACE ("ACE_Time_Request::time");
  return this->time_;
}

void 
ACE_Time_Request::time (ACE_UINT32 t)
{
  ACE_TRACE ("ACE_Time_Request::time");
  this->time_ = t;
}

// Encode the transfer buffer into network byte order 
// so that it can be sent to the server.
int
ACE_Time_Request::encode (void *&buf)
{
  ACE_TRACE ("ACE_Time_Request::encode");
  // Compute the length *before* doing the marshaling.

  buf = (void *) &this->transfer_;
  this->transfer_.block_forever_ = htonl (this->transfer_.block_forever_);
  this->transfer_.usec_timeout_	 = htonl (this->transfer_.usec_timeout_);
  this->transfer_.sec_timeout_	 = htonl (this->transfer_.sec_timeout_);
  this->transfer_.msg_type_	 = htonl (this->transfer_.msg_type_);
  this->transfer_.time_	         = htonl (this->transfer_.time_);

  return this->size ();  // Always fixed
}

// Decode the transfer buffer into host byte byte order 
// so that it can be used by the server.
int
ACE_Time_Request::decode (void)
{
  ACE_TRACE ("ACE_Time_Request::decode");
  // Decode
  this->transfer_.block_forever_ = ntohl (this->transfer_.block_forever_);
  this->transfer_.usec_timeout_	 = ntohl (this->transfer_.usec_timeout_);
  this->transfer_.sec_timeout_	 = ntohl (this->transfer_.sec_timeout_);
  this->transfer_.msg_type_	 = ntohl (this->transfer_.msg_type_);
  this->transfer_.time_	         = ntohl (this->transfer_.time_);

  this->time_ = this->transfer_.time_;
  return 0;
}

// Print out the current values of the ACE_Time_Request.

void
ACE_Time_Request::dump (void) const
{
  ACE_TRACE ("ACE_Time_Request::dump");
  ACE_DEBUG ((LM_DEBUG, "*******\nlength = %d\n", 
	      this->size ()));
  ACE_DEBUG ((LM_DEBUG, "message-type = "));

  switch (this->msg_type ())
    {
    case ACE_Time_Request::TIME_UPDATE:
      ACE_DEBUG ((LM_DEBUG, "TIME_UPDATE\n"));
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, "<unknown type> = %d\n", this->msg_type ()));
      break;
    }

  if (this->block_forever ())
    ACE_DEBUG ((LM_DEBUG, "blocking forever\n"));
  else
    {
      ACE_Time_Value tv = this->timeout ();
      ACE_DEBUG ((LM_DEBUG, "waiting for %ld secs and %ld usecs\n", 
		  tv.sec (), tv.usec ()));
    }
  ACE_DEBUG ((LM_DEBUG, "*******\ntime = %d\n", 
	      this->time ()));
  ACE_DEBUG ((LM_DEBUG, "+++++++\n"));
}

