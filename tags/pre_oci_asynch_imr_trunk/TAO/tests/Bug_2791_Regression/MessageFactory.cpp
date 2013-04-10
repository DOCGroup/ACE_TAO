// $Id$

#include "MessageFactory.h"


int MessageFactory::destructorInvoked_ = 0;


MessageFactory::~MessageFactory ()
{
  ACE_DEBUG ((LM_DEBUG, "MessageFactory: destructor invoked\n"));
  destructorInvoked_ = 1;
}


CORBA::ValueBase *
MessageFactory::create_for_unmarshal ()
{
  return new MessageImpl ();
}
