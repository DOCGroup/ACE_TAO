#include "Event_Reactor.h"
#include "Event_Reactor_Interface.h"
#include "Event_Reactor_Impl.h"
#include "ace/Object_Manager.h"
#include "ace/Synch.h"


#if !defined (__ACE_INLINE__)
#include "Event_Reactor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, Event_Reactor, "$Id$")

TAO_ACE_Event_Reactor::TAO_ACE_Event_Reactor (
    TAO_ACE_Event_Reactor_Interface *impl,
    int delete_implementation)
  : reactor_interface_ (impl),
    delete_implementation_ (delete_implementation)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::TAO_ACE_Event_Reactor");

  if (this->reactor_interface_ == 0)
    {
      // For the present let us instantiate the only reactor we
      // have.
      ACE_NEW (this->reactor_interface_,
               TAO_ACE_Event_Reactor_Impl ());
      this->delete_implementation_ = 1;
    }
}

TAO_ACE_Event_Reactor::TAO_ACE_Event_Reactor (void)
  : reactor_interface_ (0),
    delete_implementation_ (0)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::TAO_ACE_Event_Reactor");

  // For the present let us instantiate the only reactor we
  // have.
  ACE_NEW (this->reactor_interface_,
           TAO_ACE_Event_Reactor_Impl ());
  this->delete_implementation_ = 1;
}


TAO_ACE_Event_Reactor::~TAO_ACE_Event_Reactor (void)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::~TAO_ACE_Event_Reactor");
  if (this->delete_implementation_)
    delete this->reactor_interface_;
}

// Process-wide ACE_Reactor.
TAO_ACE_Event_Reactor *TAO_ACE_Event_Reactor::reactor_ = 0;

// Controls whether the Reactor is deleted when we shut down (we can
// only delete it safely if we created it!)
int TAO_ACE_Event_Reactor::delete_reactor_ = 0;


TAO_ACE_Event_Reactor *
TAO_ACE_Event_Reactor::instance (void)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::instance");

  if (TAO_ACE_Event_Reactor::reactor_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (TAO_ACE_Event_Reactor::reactor_ == 0)
        {
          ACE_NEW_RETURN (TAO_ACE_Event_Reactor::reactor_,
                          TAO_ACE_Event_Reactor,
                          0);
          TAO_ACE_Event_Reactor::delete_reactor_ = 1;
        }
    }
  return TAO_ACE_Event_Reactor::reactor_;
}

TAO_ACE_Event_Reactor *
TAO_ACE_Event_Reactor::instance (TAO_ACE_Event_Reactor *r,
                                 int delete_reactor)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::instance");

  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));

  TAO_ACE_Event_Reactor *t = TAO_ACE_Event_Reactor::reactor_;
  if (delete_reactor != 0)
    TAO_ACE_Event_Reactor::delete_reactor_ = 1;
  else
    // We can't safely delete it since we don't know who created it!
    TAO_ACE_Event_Reactor::delete_reactor_ = 0;

  TAO_ACE_Event_Reactor::reactor_ = r;
  return t;
}

void
TAO_ACE_Event_Reactor::close_singleton (void)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::close_singleton");

  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (TAO_ACE_Event_Reactor::delete_reactor_)
    {
      delete TAO_ACE_Event_Reactor::reactor_;
      TAO_ACE_Event_Reactor::reactor_ = 0;
      TAO_ACE_Event_Reactor::delete_reactor_ = 0;
    }
}


int
TAO_ACE_Event_Reactor::register_io_handler (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::register_io_handler");
  return this->reactor_interface_->register_io_handler (eh,
                                                        mask);
}

int
TAO_ACE_Event_Reactor::remove_io_handler (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::register_io_handler");
  return this->reactor_interface_->remove_io_handler (eh,
                                                      mask);
}

int
TAO_ACE_Event_Reactor::add_event_masks (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::add_event_masks");
  return this->reactor_interface_->add_event_masks (eh,
                                                    mask);
}

int
TAO_ACE_Event_Reactor::clr_event_masks (
    TAO_ACE_IO_Event_Handler *eh,
    TAO_ACE_IO_Event_Masks::Event_Masks mask)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::clr_event_masks");
  return this->reactor_interface_->clr_event_masks (eh,
                                                    mask);
}


int
TAO_ACE_Event_Reactor::handle_events (ACE_Time_Value *time)
{
  ACE_TRACE ("TAO_ACE_Event_Reactor::handle_events");
  return this->reactor_interface_->handle_events (time);
}
