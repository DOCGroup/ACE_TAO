// $Id$

#include "server_i.h"

server_i::server_i (int quiet,
                    CORBA::ORB_ptr orb)
  : quiet_ (quiet),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
server_i::start (client_ptr c,
                 CORBA::UShort time_to_live
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->client_ = client::_duplicate (c);
  this->ping (time_to_live
              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  return;
}

void
server_i::ping (CORBA::UShort time_to_live
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!this->quiet_)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) server_i::ping -> time to live = %d\n",
                time_to_live));

  --time_to_live;

  if (time_to_live > 0)
    {
      this->client_->ping (time_to_live
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
