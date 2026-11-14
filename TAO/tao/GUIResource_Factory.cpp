#include "tao/GUIResource_Factory.h"
#include "ace/Reactor.h"
#include "ace/Guard_T.h"
#include "tao/debug.h"
#include <cerrno>
#include <memory>
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  GUIResource_Factory::~GUIResource_Factory () = default;

  ACE_Reactor *
  GUIResource_Factory::get_reactor ()
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
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, nullptr);

    std::unique_ptr<ACE_Reactor> reactor {
      new (std::nothrow) ACE_Reactor (this->reactor_impl (), true)
    };

    if (!reactor)
    {
      errno = ENOMEM;
      return nullptr;
    }

    if (reactor->initialized () == 0)
    {
      return nullptr;
    }

    this->dynamically_allocated_reactor_ = true;

    return reactor.release ();
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
