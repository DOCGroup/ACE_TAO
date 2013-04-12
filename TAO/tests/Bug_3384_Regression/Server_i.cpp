// $Id $

#include "Server_i.h"

Server::~Server () {}

void Server::setup (
  ::Test::Client_ptr cli)
{
  this->cli_ = Test::Client::_duplicate (cli);
}

void Server::request (
  ::CORBA::Long sel)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server::request -> %d\n", sel));
  if (! CORBA::is_nil (this->cli_))
  {
    this->cli_->reply ("Hello world!");
  }
}

void Server::shutdown (
  void)
{
  this->orb_->shutdown (0);
}

