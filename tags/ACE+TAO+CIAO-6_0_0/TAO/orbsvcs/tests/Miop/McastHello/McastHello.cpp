//
// $Id$
//
#include "McastHello.h"



McastHello::McastHello (CORBA::ORB_ptr orb,
                        int instance)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    instance_ (instance),
    small_request_status_ (0),
    large_request_status_ (0)
{
}

void
McastHello::send_forty_two (CORBA::Long forty_two)
{
  if (this->small_request_status_ == 0 &&
      forty_two == 42)
    {
      this->small_request_status_ = 1;
    }
  else
    this->small_request_status_ = 0;
}

void
McastHello::send_large_octet_array (const Test::Octets &payload)
{
  CORBA::Boolean valid_payload = 1;
  for (CORBA::ULong i = 0; i < payload.length (); ++i)
    {
      if ((CORBA::ULong) payload [i] != i % 256)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: (%P|%t) McastHello::send_large_octet_array, "
                      "unexpected value at index %d (%d != %d)\n",
                      i, payload [i], i % 256));
          valid_payload = 0;
          break;
        }
    }

  if (this->large_request_status_ == 0 &&
      valid_payload == 1)
    {
      this->large_request_status_ = 1;
    }
  else
    this->large_request_status_ = 0;
}

void
McastHello::shutdown (void)
{
  this->orb_->shutdown (0);
}

CORBA::Boolean
McastHello::get_status ()
{
  return this->large_request_status_ && this->small_request_status_;
}
