//
// $Id$
//
#include "Oneway_Buffering.h"

ACE_RCSID(Oneway_Buffering, Oneway_Buffering, "$Id$")

Oneway_Buffering::Oneway_Buffering (CORBA::ORB_ptr orb,
                                    Test::Oneway_Buffering_Admin_ptr admin)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , admin_ (Test::Oneway_Buffering_Admin::_duplicate (admin))
{
}

void
Oneway_Buffering::receive_data (const Test::Payload &the_payload
                                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->request_received (the_payload.length () TAO_ENV_ARG_PARAMETER);
}

void
Oneway_Buffering::flush (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Oneway_Buffering::sync (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->flush (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
Oneway_Buffering::shutdown (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 TAO_ENV_ARG_PARAMETER);
}
