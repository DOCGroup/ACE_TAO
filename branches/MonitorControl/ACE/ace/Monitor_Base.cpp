// $Id$

#include "ace/Monitor_Base.h"
#include "ace/MC_Admin_Manager.h"
#include "ace/Monitor_Point_Registry.h"
#include "ace/Guard_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_sys_time.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Base::Monitor_Base (void)
    {
    }

    Monitor_Base::Monitor_Base (const char*  name)
      : name_ (name)
    {
    }

    Monitor_Base::~Monitor_Base (void)
    {
    }

    long
    Monitor_Base::add_constraint (const char* expression,
                                  Control_Action* action)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, -1);

      long id = Monitor_Point_Registry::instance ()->constraint_id ();
      
      CONSTRAINTS::value_type entry;
      entry.first = id;
      entry.second.expr = expression;
      entry.second.control_action = action;
      
      action->add_ref ();
      (void) this->constraints_.insert (entry);
      
      return id;
    }

    Control_Action*
    Monitor_Base::remove_constraint (const long constraint_id)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, guard, this->mutex_, 0);

      Control_Action* retval = 0;
      CONSTRAINT_ITERATOR i = this->constraints_.find (constraint_id);
      
      if (i != this->constraints_.end ())
        {
          retval = i->second.control_action;
          (void) this->constraints_.erase (constraint_id);
        }
        
      retval->remove_ref ();  
      return retval;
    }

    Monitor_Base::CONSTRAINTS&
    Monitor_Base::constraints (void)
    {
      // @todo make thread safe
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

    const char*
    Monitor_Base::name (void) const
    {
      return this->name_.fast_rep ();
    }

    void
    Monitor_Base::name (const char* new_name)
    {
      this->name_ = new_name;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

