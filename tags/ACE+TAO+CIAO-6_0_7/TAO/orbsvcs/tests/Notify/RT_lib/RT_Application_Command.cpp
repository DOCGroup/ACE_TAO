// $Id$

#include "RT_Application_Command.h"



#include "../lib/LookupManager.h"
#include "RT_Priority_Mapping.h"

TAO_Notify_Tests_RT_Application_Command::TAO_Notify_Tests_RT_Application_Command (void)
{
}

TAO_Notify_Tests_RT_Application_Command::~TAO_Notify_Tests_RT_Application_Command ()
{
}

void
TAO_Notify_Tests_RT_Application_Command::handle_init (void)
{
  // Call the Base class.
  TAO_Notify_Tests_Application_Command::handle_init ();

  CORBA::ORB_var orb;

  LOOKUP_MANAGER->resolve (orb);

  TAO_Notify_Tests_RT_Priority_Mapping* mapping = new TAO_Notify_Tests_RT_Priority_Mapping ();

  mapping->init (orb.in ());

  LOOKUP_MANAGER->_register (mapping);
}
