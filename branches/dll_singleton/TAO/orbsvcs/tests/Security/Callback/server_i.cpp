#include "server_i.h"


ACE_RCSID (Callback,
           server_i,
           "$Id$")


server_i::server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    client_ ()
{
}

server_i::~server_i (void)
{
}

void
server_i::set_client (client_ptr c
                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->client_ = client::_duplicate (c);
}

void
server_i::test_request (const char *msg
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->client_->test_reply (msg ACE_ENV_ARG_PARAMETER);
}

void
server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
