//$Id$
#include /**/ "ace/Reactor.h"
#include /**/ "GUIResource_Factory.h"
#include /**/ "debug.h"

namespace TAO
{
  GUIResource_Factory::GUIResource_Factory ():
    dynamically_allocated_reactor_ (0)
  {
  }

  GUIResource_Factory::~GUIResource_Factory ()
  {
  }

  ACE_Reactor *
  GUIResource_Factory::get_reactor (void)
  {
    ACE_Reactor *reactor = 0;
    ACE_NEW_RETURN (reactor,
                    ACE_Reactor (this->reactor_impl (), 1),
                    0);

    if (reactor->initialized () == 0)
    {
      delete reactor;
      reactor = 0;
    }
    else
      this->dynamically_allocated_reactor_ = 1;

    return reactor;
  }

  void
  GUIResource_Factory::reclaim_reactor (ACE_Reactor *reactor)
  {
    if (this->dynamically_allocated_reactor_ == 1)
      delete reactor;
  }
}
