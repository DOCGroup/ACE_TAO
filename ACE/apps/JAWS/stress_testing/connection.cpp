#include "connection.h"
#include "ace/OS_NS_netdb.h"
#include "ace/os_include/netinet/os_tcp.h"
#include <string>

// Make the connection to the WEB server

int connection::connect (char const *hostname_opt_port, int tcp_nodelay, int sockbufsiz)
{
  if (hostname_opt_port == nullptr)
    return 1;

  std::string hostname_with_port (hostname_opt_port);
  // Check to see if portnumber is specified in the hostnameport
  // If not, append :80
  if (hostname_with_port.find (':') == std::string::npos)
    hostname_with_port += ":80";

  // Beyond this point, hostname_with_port is of the form hostname:port

  ACE_INET_Addr server_addr (hostname_with_port.c_str ());

  // Connect to server

  ACE_SOCK_Connector con;

  if (con.connect (stream_, server_addr) == -1)
    {
      ACE_OS::perror ("ACE_SOCK_Connector::connect");
      return 1;
    }


  // tcp_nodelay processing.

  // turn off weird ack things
  if (tcp_nodelay)
    {
      struct protoent *const p = ACE_OS::getprotobyname ("tcp");
      int one = 1;

      if (p && stream_.set_option (p->p_proto,
                                   TCP_NODELAY,
                                   &one,
                                   sizeof (one)))
        {
          ACE_OS::perror ("tcp_nodelay");
          return 1;
        }
    }

  if (sockbufsiz
      && stream_.set_option (SOL_SOCKET,
                             SO_RCVBUF,
                             &sockbufsiz,
                             sizeof sockbufsiz) == -1)
    {
      ACE_OS::perror ("socket_queue_size");
      return 1;
    }

  return 0;
}

int connection::read (void *buffer, size_t maxlen, unsigned int timeout_seconds)
{
  ACE_UNUSED_ARG (timeout_seconds);
  return stream_.recv (buffer, maxlen);
}

int connection::write (void const *buffer, size_t maxlen, unsigned int timeout_seconds)
{
  ACE_UNUSED_ARG (timeout_seconds);
  return stream_.send (buffer, maxlen);
}

int connection::write_n (void const *buffer, size_t len, unsigned int timeout_seconds)
{
  ACE_UNUSED_ARG (timeout_seconds);
  if (stream_.send_n (buffer, len) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Write failed\n"), 1);
  return 0;
}

int connection::read_n (void *buffer, size_t maxlen, unsigned int timeout_seconds)
{
  ACE_UNUSED_ARG (timeout_seconds);
  if (stream_.recv_n (buffer, maxlen) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Read failed\n"), 1);
  return 1;
}

int connection::close ()
{
  this->stream_.close_reader ();
  this->stream_.close_writer ();
  this->stream_.close ();
  return 0;
}

connection::~connection ()
{
  this->close ();
}
