// $Id$

#include "Options.h"
#include "SML_Client.h"

int
SML_Client::receive (int)
{
  if (sml_server.mux (this->recv_packet_, this->packet_length) < 0)
    return -1;

  if (this->demux (this->recv_packet_, this->packet_length) < 0)
    return -1;

  return 1;
}

int
SML_Client::send (void)
{
  if (this->mux (this->send_packet_, this->packet_length) < 0)
    return -1;

  if (sml_server.demux (this->send_packet_, this->packet_length) < 0)
    return -1;

  return 1;
}

SML_Client::SML_Client (void)
{
}

SML_Client::~SML_Client (void)
{
}
