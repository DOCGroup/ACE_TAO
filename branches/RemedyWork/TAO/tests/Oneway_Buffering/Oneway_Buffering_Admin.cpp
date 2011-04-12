//
// $Id$
//
#include "Oneway_Buffering_Admin.h"
#include "ace/OS_NS_unistd.h"

namespace
{
  const unsigned int timeout_in_sec= 2u;
}

Oneway_Buffering_Admin::Oneway_Buffering_Admin (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , request_count_ (0)
  , bytes_received_count_ (0)
{
}

CORBA::ULong
Oneway_Buffering_Admin::request_count (CORBA::ULong expected_request_count)
{
  ACE_Time_Value timeout (timeout_in_sec);
  while ( (this->request_count_ < expected_request_count)
        && (ACE_Time_Value::zero < timeout) )
    {
      orb_->perform_work (timeout); // Do some work, decreases timeout for amount done.
    }
  return this->request_count_;
}

CORBA::ULong
Oneway_Buffering_Admin::bytes_received_count (CORBA::ULong expected_bytes_received_count)
{
  ACE_Time_Value timeout (timeout_in_sec);
  while ( (this->bytes_received_count_ < expected_bytes_received_count)
        && (ACE_Time_Value::zero < timeout) )
    {
      orb_->perform_work (timeout); // Do some work, decreases timeout for amount done.
    }
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
