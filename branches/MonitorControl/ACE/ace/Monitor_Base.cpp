// $Id$

#include "ace/Monitor_Base.h"

#if defined (ACE_ENABLE_MONITORS)

#include "ace/MC_Admin_Manager.h"
#include "ace/Monitor_Point_Registry.h"
#include "ace/Guard_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_sys_time.h"

#if !defined (__ACE_INLINE__)
#include "ace/Monitor_Base.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Base::Monitor_Base (void)
      : ACE_Refcountable_T<ACE_SYNCH_MUTEX> (1)
    {
    }

    Monitor_Base::Monitor_Base (const char* name)
      : ACE_Refcountable_T<ACE_SYNCH_MUTEX> (1)
      , name_ (name)
    {
    }

    Monitor_Base::~Monitor_Base (void)
    {
    }

    long
    Monitor_Base::add_constraint (const char* expression,
                                  Control_Action* action)
    {
      /// Thread-safe and guaranteed to be unique.
      long id = Monitor_Point_Registry::instance ()->constraint_id ();

      CONSTRAINTS::value_type entry;
      entry.first = id;
      entry.second.expr = expression;
      entry.second.control_action = action;

      /// This is thread-safe on its own so we don't have
      /// to guard it here.
      action->add_ref ();
      
      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, -1);
         
        /// Since we know external key is unique,
        /// we don't check for failure. 
        (void) this->constraints_.insert (entry);
      }
      
      return id;
    }

    Control_Action*
    Monitor_Base::remove_constraint (const long constraint_id)
    {
      Control_Action* retval = 0;

      {
        ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);

        CONSTRAINT_ITERATOR i = this->constraints_.find (constraint_id);

        if (i != this->constraints_.end ())
          {
            retval = i->second.control_action;
            (void) this->constraints_.erase (constraint_id);
          }
      }
      
      return retval;
    }

    Monitor_Base::CONSTRAINTS&
    Monitor_Base::constraints (void)
    {
      // @todo make thread safe
      /// @@ (JP) We're returning a reference that can change after
      /// this method returns - why do we need local thread safety?
      return this->constraints_;
    }

    void
    Monitor_Base::retrieve (MonitorControl_Types::Data& data) const
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      data = this->data_;
    }

    void
    Monitor_Base::receive (size_t value)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);

      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = static_cast<double> (value);
    }

    void
    Monitor_Base::receive (double data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = data;
    }

    void
    Monitor_Base::clear (void)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);

      this->clear_i ();
    }

    void
    Monitor_Base::clear_i (void)
    {
      this->data_.value_ = 0.0;
      this->data_.timestamp_ = ACE_Time_Value::zero;
    }


    void
    Monitor_Base::retrieve_and_clear (MonitorControl_Types::Data& data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);

      data = this->data_;
      this->clear_i ();
    }

    void
    Monitor_Base::add_to_registry (const ACE_Time_Value& time)
    {
      MC_ADMINMANAGER *mgr =
        ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

      if (!mgr->admin ().monitor_point (this, time))
        {
          ACE_ERROR ((LM_ERROR,
                      "monitor point %s registration failed\n",
                      this->name ()));
        }
    }

    void
    Monitor_Base::remove_from_registry (void)
    {
      if (!Monitor_Point_Registry::instance ()->remove (this->name ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "monitor point %s unregistration failed\n",
                      this->name ()));
        }
    }

    void
    Monitor_Base::add_ref (void)
    {
      (void) this->increment ();
    }

    void
    Monitor_Base::remove_ref (void)
    {
      const long new_count = this->decrement ();

      if (new_count == 0)
        {
          delete this;
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ENABLE_MONITORS */

