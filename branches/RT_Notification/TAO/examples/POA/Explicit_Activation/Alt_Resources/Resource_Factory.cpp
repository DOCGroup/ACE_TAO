// $Id$

#include "Resource_Factory.h"
#include "ace/WFMO_Reactor.h"

ACE_Reactor_Impl *
My_Resource_Factory::allocate_reactor_impl (void) const
{
  ACE_DEBUG ((LM_DEBUG,
              "Using alternate reactor\n"));

  return new ACE_WFMO_Reactor;
}

ACE_FACTORY_DEFINE (Alt_Resource_Factory, My_Resource_Factory)
