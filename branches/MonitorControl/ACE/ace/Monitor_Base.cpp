// $Id$

#include "ace/Monitor_Base.h"
#include "ace/Guard_T.h"

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
      MonitorControl_Types::Constraint constraint;
      constraint.expr = expression;
      constraint.control_action = action;
      (void) this->constraints_.push_back (constraint);
    }
    
    Monitor_Base::CONSTRAINTS&
    Monitor_Base::constraints (void)
    {
      return this->constraints_;
    }

    MonitorControl_Types::Data
    Monitor_Base::retrieve (void) const
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                        guard,
                        this->mutex_,
                        this->data_);
      MonitorControl_Types::Data copy = this->data_;
      return copy;
    }

    MonitorControl_Types::Data
    Monitor_Base::retrieve_and_clear (void)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                        guard,
                        this->mutex_,
                        this->data_);
      MonitorControl_Types::Data copy = this->data_;
      this->clear ();
      return copy;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

