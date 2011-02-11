// $Id$

#include "orbsvcs/Event/EC_Factory.h"
#include "ace/Dynamic_Service.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTEvent_Serv_Export unsigned short TAO_EC_debug_level;

TAO_EC_Factory::TAO_EC_Factory (void)
{
  TAO_EC_debug_level = 0;
}

TAO_EC_Factory::~TAO_EC_Factory (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
