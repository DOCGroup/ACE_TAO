// $Id$

#include "global.h"
#include "Options.h"
#include "CM_Server.h"

// Creates and binds a UDP socket... 

int 
CM_Server::open (short port_number)
{
  int max_packet_size = UDP_PACKET_SIZE;

  this->sokfd = socket (PF_INET, SOCK_DGRAM, 0);

  if (this->sokfd < 0)
    return -1;
  
  ACE_OS::memset (&this->sin, sizeof this->sin, 0);
  this->sin.sin_family = AF_INET;
  this->sin.sin_port = htons (port_number);
  this->sin.sin_addr.s_addr = INADDR_ANY;

  // This call fails if an rflo daemon is already running. 
  if (ACE_OS::bind (this->sokfd,
                    (sockaddr *) &this->sin,
                    sizeof this->sin) < 0)
    return -1;

  if (ACE_OS::setsockopt (this->sokfd,
                          SOL_SOCKET,
                          SO_SNDBUF,
                          (char *) &max_packet_size,
                          sizeof max_packet_size) < 0)
    return -1;

  return 1;
}

int
CM_Server::receive (int)
{
  int sin_len = sizeof this->sin;

  if (Options::get_opt (Options::DEBUG) != 0)
    ACE_DEBUG ((LM_DEBUG, "waiting for client to send...\n"));

  ssize_t n = recvfrom (this->sokfd,
                        this->recv_packet,
                        UDP_PACKET_SIZE,
                        0,
                        (sockaddr *) &this->sin,
                        &sin_len);
  if (n == -1)
    return -1;

  if (Options::get_opt (Options::DEBUG) != 0)
    ACE_DEBUG ((LM_DEBUG,
                "receiving from client host %s\n",
                ACE_OS::inet_ntoa (this->sin.sin_addr)));

  if (this->demux (this->recv_packet, n) < 0)
    return -1;

  return 1;
}

int
CM_Server::send (void)
{
  int  packet_length = 0;
  
  if (this->mux (this->send_packet,
                 packet_length) < 0)
    return -1;

  if (Options::get_opt (Options::DEBUG) != 0)
    ACE_DEBUG ((LM_DEBUG,
                "sending to client host %s\n",
                ACE_OS::inet_ntoa (this->sin.sin_addr)));

  if (sendto (this->sokfd,
              this->send_packet,
              packet_length,
              0,
              (sockaddr *) &this->sin,
              sizeof this->sin) < 0)
    return -1;

  return 1;
}

CM_Server::CM_Server (void)
{
}

CM_Server::~CM_Server (void)
{
  if (Options::get_opt (Options::DEBUG))
    ACE_DEBUG ((LM_DEBUG,
                "CM_Server\n"));

  ACE_OS::closesocket (this->sokfd);
}
