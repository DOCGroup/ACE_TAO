// $Id$

// client.C

#include "Log_Wrapper.h"

ACE_RCSID(Multicast, Log_Wrapper, "$Id$")

Log_Wrapper::Log_Wrapper (void)
{
  sequence_number_ = 0;
  this->log_msg_.app_id = ACE_OS::getpid ();
}

Log_Wrapper::~Log_Wrapper (void) 
{
}

// Set the log_msg_ host address.

int
Log_Wrapper::open (const int port, const char *mcast_addr) 
{
  struct hostent *host_info;
  struct utsname host_data;

  if (ACE_OS::uname (&host_data) < 0) 
    return -1;
      
  if ((host_info = ACE_OS::gethostbyname (host_data.nodename)) == NULL) 
    return -1;
  else 
    ACE_OS::memcpy ((char *) &this->log_msg_.host, 
                    (char *) host_info->h_addr, 
                    host_info->h_length);

  // This starts out initialized to all zeros!
  server_ = ACE_INET_Addr(port, mcast_addr);

  if (logger_.subscribe (server_) == -1)
      perror("can't subscribe to multicast group"), exit(1);

  // success.
  return 0;
}

// Send the message to a logger object.
// This wrapper fills in all the log_record info for you.
// uses iovector stuff to make contiguous header and message.

int
Log_Wrapper::log_message (ACE_Log_Priority type, char *message)
{ 
  sequence_number_++;

  this->log_msg_.type = type;
  this->log_msg_.time = time (0);
  this->log_msg_.msg_length = strlen(message)+1;
  this->log_msg_.sequence_number = htonl(sequence_number_);

  iovec iovp[2];
  iovp[0].iov_base = ACE_reinterpret_cast (char*, &log_msg_);
  iovp[0].iov_len  = sizeof (log_msg_);
  iovp[1].iov_base = message;
  iovp[1].iov_len  = log_msg_.msg_length;

  logger_.send (iovp, 2);

  // success.
  return 0;
}

