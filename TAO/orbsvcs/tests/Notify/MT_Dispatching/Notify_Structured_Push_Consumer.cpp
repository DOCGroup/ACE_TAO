// $Id$

#include "Notify_Structured_Push_Consumer.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
}


void
Notify_Structured_Push_Consumer::push_structured_event (
                          const CosNotification::StructuredEvent& /*event*/
                          TAO_ENV_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (++this->count_ == this->expected_)
    {
      this->done_ = 1;
    }
}

