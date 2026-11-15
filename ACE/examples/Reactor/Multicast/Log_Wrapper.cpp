// client.C

#include "Log_Wrapper.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_utsname.h"
#include "ace/OS_NS_netdb.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdlib.h"

#include <cstring>

/// Sets the host address in the log record.
int
Log_Wrapper::open (int port, char const *mcast_addr)
{
  struct hostent *host_info {};
  ACE_utsname host_data;

  if (ACE_OS::uname (&host_data) < 0)
  {
    return -1;
  }

  if ((host_info = ACE_OS::gethostbyname (host_data.nodename)) == nullptr)
  {
    return -1;
  }

  std::memcpy (&this->log_msg_.host, host_info->h_addr, host_info->h_length);
  this->log_msg_.app_id = ACE_OS::getpid ();

  // This starts out initialized to all zeros!
  server_ = ACE_INET_Addr (port, mcast_addr);

  if (logger_.join (server_) == -1)
  {
    ACE_OS::perror("can't join to multicast group"), ACE_OS::exit(1);
  }

  // success.
  return 0;
}

/// Sends the message to the logger and fills in the log record metadata.
/// An iovec keeps the header and message contiguous for the send operation.
int
Log_Wrapper::log_message (Log_Priority type, char *message)
{
  ++sequence_number_;

  this->log_msg_.type = type;
  // Casting time() to long will start causing bad results sometime in 2038
  // but the receiver isn't looking at the time, so who cares?
  this->log_msg_.time = static_cast<long> (ACE_OS::time (nullptr));
  this->log_msg_.msg_length =
    ACE_Utils::truncate_cast<ACE_INT32> (std::strlen (message) + 1);
  this->log_msg_.sequence_number = ACE_HTONL(sequence_number_);

  iovec iovp[2];
  iovp[0].iov_base = reinterpret_cast<char*> (&log_msg_);
  iovp[0].iov_len  = sizeof (log_msg_);
  iovp[1].iov_base = message;
  iovp[1].iov_len  = log_msg_.msg_length;

  logger_.send (iovp, 2);

  // success.
  return 0;
}

