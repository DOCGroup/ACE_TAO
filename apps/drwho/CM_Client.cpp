// $Id$
#include "global.h"
#include "Options.h"
#include "Multicast_Manager.h"
#include "CM_Client.h"

// Creates and binds a UDP socket...

int
CM_Client::open (short port_number)
{
  int max_packet_size = UDP_PACKET_SIZE;
  
  Comm_Manager::sokfd = ACE_OS::socket (PF_INET, SOCK_DGRAM, 0);

  if (Comm_Manager::sokfd == ACE_INVALID_HANDLE)
    return -1;
  
  ACE_OS::memset ((char *) &this->sin, 0, sizeof this->sin);
  this->sin.sin_family = AF_INET;
  this->sin.sin_port = htons (port_number);

  return 1;
}

int
CM_Client::receive (int timeout)
{
  FD_ZERO (&this->read_fd_);
  FD_SET (Comm_Manager::sokfd, &this->read_fd_);

  if (timeout > 0)
    {
      this->time_out_.tv_sec = timeout;
      this->time_out_.tv_usec = 0;
      this->top_ = &time_out;
    }

  while (Multicast_Manager::outstanding_hosts_remain ())
    {
      if (ACE_OS:;select (Comm_Manager::sokfd + 1,
                          &this->read_fd_, 0, 0, 
                          this->top_) <= 0)
	break;
      else
	{
	  int sin_len = sizeof this->sin;
	  ssize_t n = ACE_OS::recvfrom (Comm_Manager::sokfd,
                                        this->recv_packet_, UDP_PACKET_SIZE,
                                        0,
                                        (sockaddr *) &this->sin,
                                        &sin_len);
      
	  if (n < 0)
	    return -1;
	  else
	    {
	      if (Options::get_opt (Options::DEBUG) != 0)
		{
		  hostent *np = ACE_OS::gethostbyaddr ((char *) &this->sin_.sin_addr,
                                                       sizeof this->sin_.sin_addr,
                                                       AF_INET);

		  ACE_DEBUG ((LM_DEBUG,
                              "receiving from server host %s (%s)\n",
                              np->h_name,
                              inet_ntoa (this->sin.sin_addr)));
		}

	      if (this->demux (this->recv_packet, n) < 0)
		return -1;

	      Multicast_Manager::checkoff_host (this->sin.sin_addr);
	    }
	}
    }

  for (char *host_name;
       Multicast_Manager::get_next_non_responding_host (host_name); 
       )
    ACE_DEBUG ((LM_DEBUG, 
                "%s did not respond\n",
                host_name));
  return 1;
}

int
CM_Client::send (void)
{
  int packet_length = 0;
  
  if (this->mux (this->send_packet, packet_length) < 0)
    return -1;
  
  // Ship off the info to all the hosts.

  while (Multicast_Manager::get_next_host_addr (this->sin.sin_addr) != 0)
    {
      if (Options::get_opt (Options::DEBUG) != 0)
	{
	  hostent *np = ACE_OS::gethostbyaddr ((char *) &this->sin.sin_addr,
                                               sizeof this->sin.sin_addr,
                                               AF_INET);
          
	  ACE_DEBUG ((LM_DEBUG, 
                      "sending to server host %s (%s)\n",
                      np->h_name,
                      inet_ntoa (this->sin.sin_addr)));
	}

      if (sendto (Comm_Manager::sokfd, this->send_packet, packet_length, 0, (sockaddr *) &this->sin, sizeof this->sin) < 0)
	return -1;
    }
  return 1;
}

CM_Client::CM_Client (void)
  : top (0)
{
}

CM_Client::~CM_Client (void)
{
  if (Options::get_opt (Options::DEBUG))
    ACE_DEBUG ((LM_DEBUG, "disposing CM_Client\n"));

  ACE_OS::closesocket (Comm_Manager::sokfd);
}

