// $Id$

#include "NoOp_Configurator.h"

namespace CIAO
{
  namespace Deployment
  {

    NoOp_Configurator::~NoOp_Configurator (void)
    {
      // Not much to do.
    }

    int
    NoOp_Configurator::pre_orb_initialize ()
    {
      return 0;
    }

    int
    NoOp_Configurator::post_orb_initialize (CORBA::ORB_ptr)
    {
      return 0;
    }

    int
    NoOp_Configurator::init_resource_manager
    (const ::Components::ConfigValues & /*properties*/)
    {
      // @@ Currently do thing.  We should go over the resource struct in
      // the future and throw exceptions if there are
      // un-recognizable/supported stuff in it.
      return 0;
    }

    CORBA::PolicyList *
    NoOp_Configurator::find_container_policies
    (const ::Components::ConfigValues & /*properties*/)
    {
      // Not much to do.

      return 0;
    }
  }
}
