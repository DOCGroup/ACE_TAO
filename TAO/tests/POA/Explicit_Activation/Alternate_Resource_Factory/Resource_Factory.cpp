// $Id$

#include "Resource_Factory.h"

My_Resource_Factory::My_Reactors::My_Reactors (void)
  : wfmo_reactor_ (),
    reactor_ (&wfmo_reactor_)
{
}

ACE_Reactor *
My_Resource_Factory::get_reactor (void)
{
  ACE_DEBUG ((LM_DEBUG, "Using alternate reactor\n"));

  switch (this->resource_source_)
    {
    case TAO_GLOBAL:
      return &GLOBAL_REACTORS::instance ()->reactor_;
    case TAO_TSS:
      return &TSS_REACTORS::instance ()->reactor_;
    }
  return 0;
}

ACE_FACTORY_DEFINE (Alt_Resource_Factory, My_Resource_Factory)
