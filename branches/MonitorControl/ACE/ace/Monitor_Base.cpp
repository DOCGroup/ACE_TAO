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
      : MC_Generic (name)
    {
    }

    Monitor_Base::~Monitor_Base (void)
    {
    }

    void
    Monitor_Base::constraints (const char* expression,
                               Control_Action* action)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX,
                 guard,
                 this->mutex_);

      MonitorControl_Types::Constraint constraint;
      constraint.expr = expression;
      constraint.control_action = action;
      (void) this->constraints_.push_back (constraint);
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
      ACE_GUARD (ACE_SYNCH_MUTEX,
                 guard,
                 this->mutex_);
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
      ACE_GUARD (ACE_SYNCH_MUTEX,
                 guard,
                 this->mutex_);

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
      ACE_GUARD (ACE_SYNCH_MUTEX,
                 guard,
                 this->mutex_);
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
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

