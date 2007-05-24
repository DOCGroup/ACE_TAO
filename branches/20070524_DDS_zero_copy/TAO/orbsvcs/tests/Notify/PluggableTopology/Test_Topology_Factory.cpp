// $Id$

#include "Test_Topology_Factory.h"
#include "Test_Saver.h"

Test_Topology_Factory::Test_Topology_Factory()
{
}

TAO_Notify::Topology_Saver*
Test_Topology_Factory::create_saver ()
{
  TAO_Notify::Topology_Saver *saver = 0;
  ACE_NEW_RETURN (saver, Test_Saver, 0);
  return saver;
}

//virtual
TAO_Notify::Topology_Loader*
Test_Topology_Factory::create_loader ()
{
  return 0;
}


ACE_FACTORY_DEFINE (PLUGTOP, Test_Topology_Factory)
