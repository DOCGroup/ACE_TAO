// $Id$
#include "global.h"
#include "Options.h"
#include "CM_Server.h"

/* Creates and binds a UDP socket... */

int 
CM_Server::open (short port_number)
{
  int max_packet_size = UDP_PACKET_SIZE;

  if ((this->sokfd = socket (PF_INET, SOCK_DGRAM, 0)) < 0)
    return -1;
  
  bzero (&this->sin, sizeof this->sin);
  this->sin.sin_family      = AF_INET;
  this->sin.sin_port        = htons (port_number);
  this->sin.sin_addr.s_addr = INADDR_ANY;

  /* This call fails if an rflo daemon is already running. */
  if (bind (this->sokfd, (sockaddr *) &this->sin, sizeof this->sin) < 0)
    return -1;

#ifndef i386
  /* Allow for very large datagrams. */

  if (setsockopt (this->sokfd, SOL_SOCKET, SO_SNDBUF, (char *) &max_packet_size, sizeof max_packet_size) < 0)
    return -1;
#endif

  return 1;
}

int
CM_Server::receive (int)
{
  /* SUPPRESS 175 */
  int  sin_len = sizeof this->sin;
  int  n;

  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "waiting for client to send...\n");

  if ((n = recvfrom (this->sokfd, this->recv_packet, UDP_PACKET_SIZE, 0, (sockaddr *) &this->sin, &sin_len)) < 0)
    return -1;

  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "receiving from client host %s\n", inet_ntoa (this->sin.sin_addr));

  if (this->demux (this->recv_packet, n) < 0)
    return -1;

  return 1;
}

int
CM_Server::send (void)
{
  /* SUPPRESS 175 */
  int  packet_length = 0;
  
  if (this->mux (this->send_packet, packet_length) < 0)
    return -1;

  if (Options::get_opt (Options::DEBUG) != 0)
    fprintf (stderr, "sending to client host %s\n", inet_ntoa (this->sin.sin_addr));

  if (sendto (this->sokfd, this->send_packet, packet_length, 0, (sockaddr *) &this->sin, sizeof this->sin) < 0)
    return -1;

  return 1;
}

#ifndef __OPTIMIZE__
CM_Server::CM_Server (void)
{}

CM_Server::~CM_Server (void)
{
  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "CM_Server\n");
  close (this->sokfd);
}
#endif /* __OPTIMIZE__ */
