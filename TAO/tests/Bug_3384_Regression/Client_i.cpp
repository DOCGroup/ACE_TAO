// $Id $

#include "Client_i.h"

Client::~Client () {}

void Client::reply (const char * msg)
{
  this->reply_count_++;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client::reply -> %s\n", msg));
}
