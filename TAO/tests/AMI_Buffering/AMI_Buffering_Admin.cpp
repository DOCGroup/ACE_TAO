//
// $Id$
//
#include "AMI_Buffering_Admin.h"

ACE_RCSID(AMI_Buffering, AMI_Buffering_Admin, "$Id$")

AMI_Buffering_Admin::AMI_Buffering_Admin (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , request_count_ (0)
  , bytes_received_count_ (0)
{
}

CORBA::ULong
AMI_Buffering_Admin::request_count (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_count_;
}

CORBA::ULong
AMI_Buffering_Admin::bytes_received_count (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->bytes_received_count_;
}

void
AMI_Buffering_Admin::request_received (CORBA::ULong payload_length,
                                          CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->request_count_++;
  this->bytes_received_count_ += payload_length;
}

void
AMI_Buffering_Admin::flush (CORBA::Environment &)
  ACE_THROW_SPEC (())
{
}

void
AMI_Buffering_Admin::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0, ACE_TRY_ENV);
}
