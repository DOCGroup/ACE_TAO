#include "Local_Monitor.h"
#include "ace/Reactor.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/ORB_Core.h"
namespace CIAO
{

  namespace RACE
  {

    Local_Monitor::Local_Monitor (const char *,
                                  CORBA::ORB_ptr orb)
      : handler_ (new Event_Handler ()),
        reactor_ (orb->orb_core ()->reactor ()),
        interval_ (10),
        start_time_ (10),
        initialized_ (false)
    {
      this->delays_.length (0);
      if (this->resolve_central_monitor (orb) == 0 &&
          this->register_timer () == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Local_Monitor object has been "
                      "successfully initialized.\n"));
          this->initialized_ = true;
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "[CIAO::RACE] ERROR while initializing "
                      "Local_Monitor object\n"));
        }
    }

    int
    Local_Monitor::resolve_central_monitor (CORBA::ORB_ptr orb)
    {
      /// First try to get the object reference of the central monitor.
      try
        {
          /// Trying to get the object reference from the Naming Service.
          CORBA::Object_var naming_obj =
            orb->resolve_initial_references ("NameService");

          if (CORBA::is_nil (naming_obj.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR, "[CIAO::RACE] Local_Monitor: "
                                 "Unable to resolve the Name Service.\n"),
                                -1);
            }

          CosNaming::NamingContext_var namingContext =
            CosNaming::NamingContext::_narrow (naming_obj.in ());

          CosNaming::Name name (1);
          name.length (1);
          name[0].id = CORBA::string_dup ("RACE_QoS_Monitor");

          CORBA::Object_var component = namingContext->resolve (name);

          Central_Monitor_var central_monitor =
            Central_Monitor::_narrow (component.in ());

          if (CORBA::is_nil (central_monitor.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR, "[CIAO::RACE] Local_Monitor: "
                                 "Object reference of the central "
                                 "monitor component is nil!\n") , -1);
            }


          CORBA::Object_var facet = central_monitor->provide_Monitor ();

          this->monitor_ = Execution_Time_Monitor::_narrow (facet.in ());

          if (CORBA::is_nil (this->monitor_.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR, "[CIAO::RACE] Local_Monitor: "
                                 "Object reference of the Monitor facet of the "
                                 "central monitor component is nil!\n") , -1);
            }
        }

      catch (::CosNaming::NamingContext::NotFound &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "[CIAO::RACE]: Exception caught in "
                               "Local_Monitor::init()");
          return -1;
        }

      catch (::CosNaming::NamingContext::InvalidName &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "[CIAO::RACE}: Exception caught in "
                               "Local_Monitor::init()");
          return -1;
        }

      catch (::CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "[CIAO::RACE}: Exception caught in "
                               "Local_Monitor::init()");
          return -1;
        }
      return 0;
    }

    int
    Local_Monitor::register_timer ()
    {
      /// Now set up the "periodic update task".
      this->reactor_->owner (ACE_Thread::self ());
      this->timer_id_ =
        this->reactor_->schedule_timer (this->handler_,
                                        this,
                                        this->start_time_,
                                        this->interval_);
      if (this->timer_id_ < 0)
        {
          ACE_ERROR ((LM_ERROR, "[CIAO::RACE] Local_Monitor: Could not "
                      "register periodic task with the reactor!\n"));
          return -1;
        }
      ACE_DEBUG ((LM_DEBUG, "[CIAO::RACE] Local_Monitor: Registered periodic "
                  "task with id %d.\n", this->timer_id_));
      return 0;
    }

    Local_Monitor::~Local_Monitor ()
    {
      this->reactor_->cancel_timer (this->timer_id_);
      delete this->handler_;
    }

    void
    Local_Monitor::start (const char *id)
    {
      if (!this->initialized_)
        {
          return;
        }
      if (!this->instance_id_.in ())
        {
          this->instance_id_ = CORBA::string_dup (id);
        }
      this->timer_.start ();
    }

    void
    Local_Monitor::stop ()
    {
      if (!this->initialized_)
        {
          return;
        }
	  this->timer_.stop ();
	  ACE_Time_Value time;
	  this->timer_.elapsed_time (time);
      
      this->mutex_.acquire ();

      this->delays_.length (this->delays_.length () + 1);
      this->delays_ [this->delays_.length () - 1] = time.msec ();

      this->mutex_.release ();
    }

    void
    Local_Monitor::dump ()
    {
      if (!this->initialized_)
        {
          return;
        }
      ACE_DEBUG ((LM_DEBUG, "In Trigger::dump %s.\n",
                  this->instance_id_.in ()));

      this->mutex_.acquire ();

      for (CORBA::ULong itr = 0; itr < this->delays_.length (); ++itr)
        {
          ACE_DEBUG ((LM_DEBUG, "%d\n", this->delays_[itr]));
        }

      this->mutex_.release ();
    }

    int
    Local_Monitor::push ()
    {
      if (!this->initialized_)
        {
          return -1;
        }
      try
        {
          this->mutex_.acquire ();
          this->monitor_->push_delays (this->instance_id_.in (),
                                       this->delays_);
          this->delays_.length (0);
          this->mutex_.release ();
          return 0;
        }

      catch (CORBA::Exception &ex)
        {
          ACE_PRINT_EXCEPTION (ex, "[CIAO::RACE] Local_Monitor:: "
                               "Exception caught in push ().\n");
          this->mutex_.release ();
          return -1;
        }
    }


    int
    Event_Handler::handle_timeout (const ACE_Time_Value &,
                                   const void *ref)
    {
      const Local_Monitor *const_monitor =
        reinterpret_cast <const Local_Monitor *> (ref);
      Local_Monitor *monitor = const_cast <Local_Monitor *> (const_monitor);

      return monitor->push ();
    }

  };
};
