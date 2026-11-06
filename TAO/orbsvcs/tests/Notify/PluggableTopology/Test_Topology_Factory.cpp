#include "Test_Topology_Factory.h"
#include "Test_Saver.h"

TAO_Notify::Topology_Saver*
Test_Topology_Factory::create_saver ()
{
  TAO_Notify::Topology_Saver* saver {};
  ACE_NEW_RETURN (saver, Test_Saver, nullptr);
  return saver;
}

TAO_Notify::Topology_Loader*
Test_Topology_Factory::create_loader ()
{
  return nullptr;
}

ACE_FACTORY_DEFINE (PLUGTOP, Test_Topology_Factory)
