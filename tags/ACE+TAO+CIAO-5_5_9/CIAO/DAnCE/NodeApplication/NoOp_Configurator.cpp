// $Id$

#include "NoOp_Configurator.h"

CIAO::NoOp_Configurator::~NoOp_Configurator (void)
{
  // Not much to do.
}

int
CIAO::NoOp_Configurator::pre_orb_initialize ()
{
  return 0;
}

int
CIAO::NoOp_Configurator::post_orb_initialize (CORBA::ORB_ptr)
{
  return 0;
}

int
CIAO::NoOp_Configurator::init_resource_manager
(const ::Deployment::Properties & /*properties*/)
{
  // @@ Currently do thing.  We should go over the resource struct in
  // the future and throw exceptions if there are
  // un-recognizable/supported stuff in it.
  return 0;
}

CORBA::PolicyList *
CIAO::NoOp_Configurator::find_container_policies
(const ::Deployment::Properties & /*properties*/)
{
  // Not much to do.

  return 0;
}
