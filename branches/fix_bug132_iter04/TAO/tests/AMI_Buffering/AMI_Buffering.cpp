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
AMI_Buffering::receive_data (const Test::Payload &the_payload,
                                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->request_received (the_payload.length (), ACE_TRY_ENV);
}

void
AMI_Buffering::flush (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
AMI_Buffering::sync (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_->flush (ACE_TRY_ENV);
}

void
AMI_Buffering::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
