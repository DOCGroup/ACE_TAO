//
// $Id$
//
#include "Oneway_Buffering_Admin.h"

ACE_RCSID(Oneway_Buffering, Oneway_Buffering_Admin, "$Id$")

Oneway_Buffering_Admin::Oneway_Buffering_Admin (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , request_count_ (0)
  , bytes_received_count_ (0)
{
}

CORBA::ULong
Oneway_Buffering_Admin::request_count (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_count_;
}

CORBA::ULong
Oneway_Buffering_Admin::bytes_received_count (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->bytes_received_count_;
}

void
Oneway_Buffering_Admin::request_received (CORBA::ULong payload_length
                                          ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->request_count_++;
  this->bytes_received_count_ += payload_length;
}

void
Oneway_Buffering_Admin::flush (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Oneway_Buffering_Admin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
