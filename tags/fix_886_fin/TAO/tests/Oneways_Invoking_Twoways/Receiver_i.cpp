#include "Receiver_i.h"

ACE_RCSID(Oneways_Invoking_Twoways, Receiver_i, "$Id$")

Receiver_i::Receiver_i (CORBA::ORB_ptr orb,
                        Test::Sender_ptr sender,
                        CORBA::ULong iter)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    sender_ (Test::Sender::_duplicate (sender)),
    iteration_ (iter),
    no_calls_ (0)
{
}

Receiver_i::~Receiver_i (void)
{
}



CORBA::Long
Receiver_i::receive_call (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Doing a ping... \n"));

  this->sender_->ping (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return this->no_calls_++;
}

CORBA::Long
Receiver_i::get_call_count (CORBA::Environment & /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->iteration_;
}
