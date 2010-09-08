// $Id$

#ifndef TAO_Notify_Tests_COMMAND_FACTORY_T_CPP
#define TAO_Notify_Tests_COMMAND_FACTORY_T_CPP

#include "Command_Factory_T.h"



#include "ace/Dynamic_Service.h"
#include "tao/debug.h"
#include "Command_Builder.h"
#include "Name.h"

template <class COMMAND>
TAO_Notify_Tests_Command_Factory_T<COMMAND>::TAO_Notify_Tests_Command_Factory_T (void)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Creating command factory for %s\n", COMMAND::name()));
}

template <class COMMAND>
TAO_Notify_Tests_Command_Factory_T<COMMAND>::~TAO_Notify_Tests_Command_Factory_T ()
{
}

template <class COMMAND> int
TAO_Notify_Tests_Command_Factory_T<COMMAND>::init (int /*argc*/, ACE_TCHAR/*argv*/ *[])
{
  /// register with Command builder
  TAO_Notify_Tests_Command_Builder* cmd_builder =
    ACE_Dynamic_Service<TAO_Notify_Tests_Command_Builder>::instance (TAO_Notify_Tests_Name::command_builder);

  if (cmd_builder)
    cmd_builder->_register (COMMAND::name(), this);
  else
    ACE_DEBUG ((LM_DEBUG, "Could not register command builder %s\n", COMMAND::name()));
  return 0;
}

template <class COMMAND> int
TAO_Notify_Tests_Command_Factory_T<COMMAND>::fini (void)
{
  return 0;
}

template <class COMMAND> TAO_Notify_Tests_Command*
TAO_Notify_Tests_Command_Factory_T<COMMAND>::create (void)
{
  return new COMMAND ();
}

#endif /* TAO_Notify_Tests_COMMAND_FACTORY_T_CPP */
