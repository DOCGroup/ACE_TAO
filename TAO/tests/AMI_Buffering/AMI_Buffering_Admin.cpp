#include "AMI_Buffering_Admin.h"

AMI_Buffering_Admin::AMI_Buffering_Admin (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , request_count_ (0)
  , bytes_received_count_ (0)
{
}

CORBA::ULong
AMI_Buffering_Admin::request_count ()
{
  return this->request_count_;
}

CORBA::ULong
AMI_Buffering_Admin::bytes_received_count ()
{
  return this->bytes_received_count_;
}

void
AMI_Buffering_Admin::request_received (CORBA::ULong payload_length)
{
  this->request_count_++;
  this->bytes_received_count_ += payload_length;
}

void
AMI_Buffering_Admin::flush ()
{
}

void
AMI_Buffering_Admin::shutdown ()
{
  this->orb_->shutdown (false);
}
