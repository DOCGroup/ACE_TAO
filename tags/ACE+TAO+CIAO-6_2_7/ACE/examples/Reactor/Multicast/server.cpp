// $Id$

// server.cpp (written by Tim Harrison)

// Listens to multicast address for client log messages.  Prints
// Mbits/sec received from client.

#include "ace/OS_main.h"
#include "ace/SOCK_Dgram.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "Log_Wrapper.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"



#if defined (ACE_HAS_IP_MULTICAST)
class Server_Events : public ACE_Event_Handler
{
public:
  Server_Events (u_short port,
                 const char *mcast_addr,
                 long time_interval = 0);
  ~Server_Events (void);

  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  virtual ACE_HANDLE get_handle (void) const;

  ACE_Time_Value *wait_time (void);

private:
  char *message_;
  Log_Wrapper::Log_Record *log_record_;
  char buf_[4 * BUFSIZ];
  char hostname_[MAXHOSTNAMELEN];

  int initialized_;
  int count_;
  long interval_;
  // time interval to log messages

  ACE_Time_Value *how_long_;
  ACE_Reactor *reactor_;
  ACE_SOCK_Dgram_Mcast mcast_dgram_;
  ACE_INET_Addr remote_addr_;
  ACE_INET_Addr mcast_addr_;

  // = statistics on messages received
  double total_bytes_received_;
  int total_messages_received_;
  int last_sequence_number_;
};

static const char MCAST_ADDR[] = ACE_DEFAULT_MULTICAST_ADDR;
static const int UDP_PORT = ACE_DEFAULT_MULTICAST_PORT;
static const int DURATION = 5;

ACE_HANDLE
Server_Events::get_handle (void) const
{
  return this->mcast_dgram_.get_handle ();
}

ACE_Time_Value *
Server_Events::wait_time (void)
{
  return this->how_long_;
}

Server_Events::Server_Events (u_short port,
                              const char *mcast_addr,
                              long time_interval)
  : initialized_ (0),
    count_ (1),
    interval_ (time_interval),
    mcast_addr_ (port, mcast_addr),
    total_bytes_received_ (0)
{
  // Use ACE_SOCK_Dgram_Mcast factory to subscribe to multicast group.

  if (ACE_OS::hostname (this->hostname_,
                        MAXHOSTNAMELEN) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "hostname"));

  else if (this->mcast_dgram_.join (this->mcast_addr_) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "join"));
  else
    {
      // Point to NULL so that we block in the beginning.
      this->how_long_ = 0;

      this->log_record_ = (Log_Wrapper::Log_Record *) &buf_;
      this->message_ = &buf_[sizeof (Log_Wrapper::Log_Record)];
    }
}

// A destructor that emacs refuses to color blue ;-)

Server_Events::~Server_Events (void)
{
  this->mcast_dgram_.leave (this->mcast_addr_);

  ACE_DEBUG ((LM_DEBUG,
              "total bytes received = %d after %d second\n",
              this->total_bytes_received_,
              this->interval_));

  ACE_DEBUG ((LM_DEBUG,
              "Mbits/sec = %.2f\n",
              (float) (total_bytes_received_ * 8 / (float) (1024*1024*interval_))));

  ACE_DEBUG ((LM_DEBUG,
              "last sequence number = %d\ntotal messages received = %d\ndiff = %d\n",
              this->last_sequence_number_,
              this->total_messages_received_,
              this->last_sequence_number_ - total_messages_received_));
}

int
Server_Events::handle_timeout (const ACE_Time_Value &,
                               const void *arg)
{
  ACE_DEBUG ((LM_DEBUG, "\t%d timeout%s occurred for %s.\n",
              this->count_,
              this->count_ == 1 ? "" : "s",
              (char *) arg));

  // Don't let the timeouts continue if there's no activity since
  // otherwise we use up a lot of CPU time unnecessarily.
  if (this->count_ == 5)
    {
      reactor ()->cancel_timer (this);
      this->initialized_ = 0;

      ACE_DEBUG ((LM_DEBUG,
                  "\tcancelled timeout for %s to avoid busy waiting.\n",
                  (char *) arg));
    }

  this->count_++;
  return 0;
}

int
Server_Events::handle_input (ACE_HANDLE)
{
  // Receive message from multicast group.
  iovec iovp[2];
  iovp[0].iov_base = buf_;
  iovp[0].iov_len  = sizeof (log_record_);
  iovp[1].iov_base = &buf_[sizeof (log_record_)];
  iovp[1].iov_len  = 4 * BUFSIZ - sizeof (log_record_);

  ssize_t retcode =
    this->mcast_dgram_.recv (iovp,
                             2,
                             this->remote_addr_);
  if (retcode != -1)
    {
      total_messages_received_++;
      total_bytes_received_ += retcode;
      last_sequence_number_ =
        ACE_NTOHL (log_record_->sequence_number);

      for (char *message_end = this->message_ + ACE_OS::strlen (this->message_) - 1;
           ACE_OS::strchr ("\r\n \t", *message_end) != 0;
           )
        {
          *message_end-- = '\0';
          if (message_end == this->message_)
            break;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "sequence number = %d\n",
                  last_sequence_number_));
      ACE_DEBUG ((LM_DEBUG,
                  "message = '%s'\n",
                  this->message_));

      if (this->initialized_ == 0)
        {
          // Restart the timer since we've received events again.
          if (reactor()->schedule_timer (this,
                                         (void *) this->hostname_,
                                         ACE_Time_Value::zero,
                                         ACE_Time_Value (DURATION)) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "schedule_timer"),
                              -1);
          this->initialized_ = 1;
        }

      this->count_ = 1;
      return 0;
    }
  else
    return -1;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Instantiate a server which will receive messages for DURATION
  // seconds.
  Server_Events server_events (UDP_PORT,
                               MCAST_ADDR,
                               DURATION);
  // Instance of the ACE_Reactor.
  ACE_Reactor reactor;

  if (reactor.register_handler (&server_events,
                                ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n%a",
                "register_handler",
                1));

  ACE_DEBUG ((LM_DEBUG,
              "starting up server\n"));

  for (;;)
    reactor.handle_events (server_events.wait_time ());

  ACE_NOTREACHED (return 0;)
}
#else
int
ACE_TMAIN(int, ACE_TCHAR *argv[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "error: %s must be run on a platform that support IP multicast\n",
                     argv[0]), -1);
}
#endif /* ACE_HAS_IP_MULTICAST */
