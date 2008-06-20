// $Id$

#include "ace/Monitor_Base.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK) && (ACE_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Admin_Manager.h"
#include "ace/Monitor_Control_Action.h"
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
  namespace Monitor_Control
  {
    Monitor_Base::Monitor_Base (const char* name, Information_Type type)
      : ACE_Refcountable_T<ACE_SYNCH_MUTEX> (1)
      , name_ (name)
      , type_ (type)
      , index_ (0UL)
      , minimum_set_ (false)
      , minimum_ (0.0)
      , maximum_ (0.0)
      , sum_ (0.0)
      , sum_of_squares_ (0.0)
      , last_ (0.0)
    {
    }

    Monitor_Base::~Monitor_Base (void)
    {
    }
    
    void
    Monitor_Base::update (void)
    {
      /// Overridden in derived classes.
    }

    void
    Monitor_Base::receive (double data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = data;

      if (this->type_ != MC_COUNTER)
        {
          this->sum_ += data;
          this->sum_of_squares_ += (data * data);
          ++this->index_;
        }

      if (this->type_ == MC_COUNTER)
        {
          ++this->last_;
          this->maximum_ = this->last_;
        }
      else
        {
          this->last_ = data;

          if (!this->minimum_set_)
            {
              this->minimum_set_ = true;
              this->minimum_ = data;
            }
          else if (this->minimum_ > data)
            {
              this->minimum_ = data;
            }

          if (this->maximum_ < data)
            {
              this->maximum_ = data;
            }
        }
    }

    void
    Monitor_Base::receive (size_t data)
    {
      this->receive (static_cast<double> (data));
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
      return this->constraints_;
    }

    void
    Monitor_Base::retrieve (Monitor_Control_Types::Data& data) const
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      
      data = this->data_;
    }

    void
    Monitor_Base::clear (void)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);

      this->clear_i ();
    }

    void
    Monitor_Base::retrieve_and_clear (Monitor_Control_Types::Data& data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);

      data = this->data_;
      this->clear ();
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
// (JP) There is a problem with this failing on a single ACE_Message_Queue
//      monitor per process. I think it is the message queue associated
//      with the default reactor, maybe because at that low level, ACE
//      is using malloc with placement, then free, which may bypass the
//      normal destructors. In any case, it happens only at shutdown
//      and there seems to be no memory leak.
//          ACE_ERROR ((LM_ERROR,
//                      "monitor point %s unregistration failed\n",
//                      this->name ()));
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
      long const new_count = this->decrement ();

      if (new_count == 0)
        {
          delete this;
        }
    }
  
    double
    Monitor_Base::average (void) const
    {
      if (this->type_ == MC_COUNTER || this->type_ == MC_GROUP)
        {
          return 0.0;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0.0);

      return (this->index_== 0UL ? 0.0 : this->sum_ / this->index_);
    }

    double
    Monitor_Base::sum_of_squares (void) const
    {
      if (this->type_ == MC_COUNTER || this->type_ == MC_GROUP)
        {
          return 0.0;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0.0);

      return this->sum_of_squares_;
    }

    size_t
    Monitor_Base::count (void) const
    {
      if (this->type_ == MC_GROUP)
        {
          return 0UL;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0UL);

      return (this->type_ == MC_COUNTER
              ? static_cast<size_t> (this->last_)
              : this->index_);
    }

    double
    Monitor_Base::minimum_sample (void) const
    {
      if (this->type_ == MC_GROUP)
        {
          return 0.0;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0.0);

      return this->minimum_;
    }

    double
    Monitor_Base::maximum_sample (void) const
    {
      if (this->type_ == MC_GROUP)
        {
          return 0.0;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0.0);

      return this->maximum_;
    }

    double
    Monitor_Base::last_sample (void) const
    {
      if (this->type_ == MC_GROUP)
        {
          return 0.0;
        }

      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0.0);

      return this->last_;
    }

    Monitor_Base::Information_Type
    Monitor_Base::type (void) const
    {
      return this->type_;
    }

    void
    Monitor_Base::clear_i (void)
    {
      this->data_.value_ = 0.0;
      this->data_.timestamp_ = ACE_Time_Value::zero;
      this->index_ = 0UL;
      this->minimum_set_ = false;
      this->minimum_ = 0.0;
      this->maximum_ = 0.0;
      this->sum_ = 0.0;
      this->sum_of_squares_ = 0.0;
      this->last_ = 0.0;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK==1 */

