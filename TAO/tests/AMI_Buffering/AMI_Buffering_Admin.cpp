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
AMI_Buffering_Admin::request_count (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_count_;
}

CORBA::ULong
AMI_Buffering_Admin::bytes_received_count (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->bytes_received_count_;
}

void
AMI_Buffering_Admin::request_received (CORBA::ULong payload_length
                                          ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->request_count_++;
  this->bytes_received_count_ += payload_length;
}

void
AMI_Buffering_Admin::flush (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
AMI_Buffering_Admin::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
