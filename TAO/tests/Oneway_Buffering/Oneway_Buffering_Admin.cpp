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
Oneway_Buffering_Admin::request_count (void)
{
  return this->request_count_;
}

CORBA::ULong
Oneway_Buffering_Admin::bytes_received_count (void)
{
  return this->bytes_received_count_;
}

void
Oneway_Buffering_Admin::request_received (CORBA::ULong payload_length)
{
  this->request_count_++;
  this->bytes_received_count_ += payload_length;
}

void
Oneway_Buffering_Admin::flush (void)
{
}

void
Oneway_Buffering_Admin::shutdown (void)
{
  this->orb_->shutdown (0);
}
