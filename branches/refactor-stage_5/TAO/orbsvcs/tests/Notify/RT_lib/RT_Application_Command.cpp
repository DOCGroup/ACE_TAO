// $Id$

#include "RT_Application_Command.h"

ACE_RCSID(lib, TAO_RT_Application_Command, "$id$")

#include "../lib/LookupManager.h"
#include "RT_Priority_Mapping.h"

TAO_Notify_Tests_RT_Application_Command::TAO_Notify_Tests_RT_Application_Command (void)
{
}

TAO_Notify_Tests_RT_Application_Command::~TAO_Notify_Tests_RT_Application_Command ()
{
}

void
TAO_Notify_Tests_RT_Application_Command::handle_init (ACE_ENV_SINGLE_ARG_DECL)
{
  // Call the Base class.
  TAO_Notify_Tests_Application_Command::handle_init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ORB_var orb;

  LOOKUP_MANAGER->resolve (orb);

  TAO_Notify_Tests_RT_Priority_Mapping* mapping = new TAO_Notify_Tests_RT_Priority_Mapping ();

  mapping->init (orb.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  LOOKUP_MANAGER->_register (mapping);
}
