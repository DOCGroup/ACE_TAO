// $Id$

#include "Application_Command_Factory.h"

#if ! defined (__ACE_INLINE__)
#include "Application_Command_Factory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(lib, TAO_Application_Command_Factory, "$id$")

#include "ace/Dynamic_Service.h"
#include "Name.h"
#include "Command_Builder.h"
#include "Application_Command.h"

TAO_NS_Application_Command_Factory::TAO_NS_Application_Command_Factory (void)
{
}

TAO_NS_Application_Command_Factory::~TAO_NS_Application_Command_Factory ()
{
}

int
TAO_NS_Application_Command_Factory::init (int argc, char *argv[])
{
  /// register with Command builder
  TAO_NS_Command_Builder* cmd_builder =
    ACE_Dynamic_Service<TAO_NS_Command_Builder>::instance (TAO_NS_Name.command_builder);

  cmd_builder->_register (TAO_NS_Name.application_command, this);

  return 0;
}

int
TAO_NS_Application_Command_Factory::fini (void)
{
        return 0;
}

TAO_NS_Command*
TAO_NS_Application_Command_Factory::create (void)
{
  return new TAO_NS_Application_Command ();
}

ACE_STATIC_SVC_DEFINE(TAO_NS_Application_Command_Factory,
                      TAO_NS_Name.application_command_factory,
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (TAO_NS_Application_Command_Factory),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                      0)

ACE_FACTORY_DEFINE (TAO_NOTIFY_TEST, TAO_NS_Application_Command_Factory)

ACE_STATIC_SVC_REQUIRE (TAO_NS_Application_Command_Factory)
