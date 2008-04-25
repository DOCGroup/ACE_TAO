// $Id$

#include "ace/Monitor_Control_Types.h"

#if defined (ACE_HAS_MONITOR_FRAMEWORK)

#include "ace/Monitor_Control_Action.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    Monitor_Control_Types::Data::Data (void)
      : timestamp_ (ACE_Time_Value::zero),
        value_ (0.0)
    {}

    //=============================================================

    Monitor_Control_Types::Constraint::Constraint (void)
      : control_action (0)
    {}

    Monitor_Control_Types::Constraint::Constraint (const Constraint& rhs)
      : expr (rhs.expr),
        control_action (rhs.control_action)
    {
      if (control_action != 0)
        {
          control_action->add_ref ();
        }
    }

    Monitor_Control_Types::Constraint::~Constraint (void)
    {
      if (this->control_action != 0)
        {
          this->control_action->remove_ref ();
        }
    }

    Monitor_Control_Types::Constraint&
    Monitor_Control_Types::Constraint::operator= (const Constraint& rhs)
    {
      if (this->control_action != 0)
        {
          this->control_action->remove_ref ();
        }

      this->expr = rhs.expr;
      this->control_action = rhs.control_action;

      if (this->control_action != 0)
        {
          this->control_action->add_ref ();
        }

      return *this;
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_MONITOR_FRAMEWORK*/


