#include "Sender_i.h"

ACE_RCSID(Oneways_Invoking_Twoways, Sender_i, "$Id$")

Sender_i::Sender_i (CORBA::ORB_ptr orb,
                    CORBA::ULong no)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    number_ (no),
    active_objects_ (0)
{
}

Sender_i::~Sender_i (void)
{
}

void
Sender_i::active_objects (CORBA::Short no_threads
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER */ )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->active_objects_ = no_threads;
}


void
Sender_i::send_ready_message (Test::Receiver_ptr receiver
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Received a call ...\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Calling the Receiver ..\n"));

  for (CORBA::ULong i = 0;
       i < this->number_;
       ++i)
    {
      receiver->receive_call (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

}

void
Sender_i::ping (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}
