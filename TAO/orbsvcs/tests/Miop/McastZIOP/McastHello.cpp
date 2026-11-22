#include "McastHello.h"

McastHello::McastHello (CORBA::ORB_ptr orb,
                        int instance)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    instance_ (instance),
    small_request_status_ (false),
    large_request_status_ (false)
{
}

void
McastHello::send_forty_two (CORBA::Long forty_two)
{
  if (!this->small_request_status_ &&
      forty_two == 42)
    {
      this->small_request_status_ = true;
    }
  else
  {
    this->small_request_status_ = false;
  }
}

void
McastHello::send_large_octet_array (const Test::Octets &payload)
{
  CORBA::Boolean valid_payload = true;
  for (CORBA::ULong i = 0; i < payload.length (); ++i)
    {
      if ((CORBA::ULong) payload [i] != i % 256)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: (%P|%t) McastHello::send_large_octet_array, "
                      "unexpected value at index %d (%d != %d)\n",
                      i, payload [i], i % 256));
          valid_payload = false;
          break;
        }
    }

  if (!this->large_request_status_ &&
      valid_payload == 1)
    {
      this->large_request_status_ = true;
    }
  else
  {
    this->large_request_status_ = false;
  }
}

void
McastHello::shutdown ()
{
  this->orb_->shutdown (false);
}

CORBA::Boolean
McastHello::get_status ()
{
  return this->large_request_status_ && this->small_request_status_;
}
