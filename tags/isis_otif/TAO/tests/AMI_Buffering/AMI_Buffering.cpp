//
// $Id$
//
#include "AMI_Buffering.h"

ACE_RCSID(AMI_Buffering, AMI_Buffering, "$Id$")

AMI_Buffering::AMI_Buffering (CORBA::ORB_ptr orb,
                              Test::AMI_Buffering_Admin_ptr admin)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , admin_ (Test::AMI_Buffering_Admin::_duplicate (admin))
{
}

void
AMI_Buffering::receive_data (const Test::Payload &the_payload
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->request_received (the_payload.length () ACE_ENV_ARG_PARAMETER);
}

void
AMI_Buffering::flush (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
AMI_Buffering::sync (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->flush (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
AMI_Buffering::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
