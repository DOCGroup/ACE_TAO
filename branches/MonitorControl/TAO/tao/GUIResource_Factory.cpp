//$Id$
#include "tao/GUIResource_Factory.h"
#include "ace/Reactor.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  GUIResource_Factory::GUIResource_Factory (void)
    : dynamically_allocated_reactor_ (false)
  {
  }

  GUIResource_Factory::~GUIResource_Factory (void)
  {
  }

  ACE_Reactor *
  GUIResource_Factory::get_reactor (void)
  {
    // @@Marek, do we need a lock here??
    // @Bala, I suppose we don't need locking for any
    //   reasonable use case as this
    //   factory is intended to be a variable in TSS.
    //   I can imagine that    someone may try to use it in distinct
    //   threads, though I do not know
    //   what for. Nevertheless, just for a case  I sync the creation of reactor.
    //   I think, that double checked locking is
    //   not necessary, because the performance is not an issue here.
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

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
      this->dynamically_allocated_reactor_ = true;

    return reactor;
  }

  void
  GUIResource_Factory::reclaim_reactor (ACE_Reactor *reactor)
  {
    ACE_GUARD ( TAO_SYNCH_MUTEX, ace_mon, this->lock_ );

    if (this->dynamically_allocated_reactor_)
      delete reactor;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
