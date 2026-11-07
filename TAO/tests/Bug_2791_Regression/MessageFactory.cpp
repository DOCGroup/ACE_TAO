#include "MessageFactory.h"

int MessageFactory::destructorInvoked_ {};

MessageFactory::~MessageFactory ()
{
  ACE_DEBUG ((LM_DEBUG, "MessageFactory: destructor invoked\n"));
  destructorInvoked_ = 1;
}


CORBA::ValueBase *
MessageFactory::create_for_unmarshal ()
{
  // ValueFactoryBase transfers ownership of the allocated valuetype to TAO.
  return new MessageImpl ();
}
