// $Id$

#include "server_i.h"

server_i::server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    client_ ()
{
}

server_i::~server_i (void)
{
}

void
server_i::set_client (client_ptr c)
{
  this->client_ = client::_duplicate (c);
}

void
server_i::test_request (const char *msg)
{
  this->client_->test_reply (msg);
}

void
server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
