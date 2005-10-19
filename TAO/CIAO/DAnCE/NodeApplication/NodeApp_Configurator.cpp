// $Id$

#include "NodeApp_Configurator.h"

CIAO::NodeApp_Configurator::NodeApp_Configurator (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}
