// $Id$

#ifndef TAO_NS_COMMAND_FACTORY_T_CPP
#define TAO_NS_COMMAND_FACTORY_T_CPP

#include "Command_Factory_T.h"

ACE_RCSID(Notify, TAO_Command_Factory_T, "$id$")

#include "ace/Dynamic_Service.h"
#include "tao/debug.h"
#include "Command_Builder.h"
#include "Name.h"

template <class COMMAND>
TAO_NS_Command_Factory_T<COMMAND>::TAO_NS_Command_Factory_T (void)
{
  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "Creating command factory for %s\n", COMMAND::name()));
}

template <class COMMAND>
TAO_NS_Command_Factory_T<COMMAND>::~TAO_NS_Command_Factory_T ()
{
}

template <class COMMAND> int
TAO_NS_Command_Factory_T<COMMAND>::init (int /*argc*/, char/*argv*/ *[])
{
  /// register with Command builder
  TAO_NS_Command_Builder* cmd_builder =
    ACE_Dynamic_Service<TAO_NS_Command_Builder>::instance (TAO_NS_Name::command_builder);

  if (cmd_builder)
    cmd_builder->_register (COMMAND::name(), this);
  else
    ACE_DEBUG ((LM_DEBUG, "Could not register command builder %s\n", COMMAND::name()));
  return 0;
}

template <class COMMAND> int
TAO_NS_Command_Factory_T<COMMAND>::fini (void)
{
  return 0;
}

template <class COMMAND> TAO_NS_Command*
TAO_NS_Command_Factory_T<COMMAND>::create (void)
{
  return new COMMAND ();
}

#endif /* TAO_NS_COMMAND_FACTORY_T_CPP */
