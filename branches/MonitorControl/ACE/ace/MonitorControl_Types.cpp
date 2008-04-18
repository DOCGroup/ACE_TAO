// $Id$

#include "ace/MonitorControl_Types.h"

#if defined (ACE_ENABLE_MONITORS)

#include "ace/Control_Action.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorControl_Types::Data::Data (void)
      : timestamp_ (ACE_Time_Value::zero),
        value_ (0.0)
    {}
    
    //=============================================================
    
    MonitorControl_Types::Constraint::Constraint (void)
      : control_action (0)
    {}
    
    MonitorControl_Types::Constraint::Constraint (const Constraint& rhs)
      : expr (rhs.expr),
        control_action (rhs.control_action)
    {
      if (control_action != 0)
        {
          control_action->add_ref ();
        }
    }
    
    MonitorControl_Types::Constraint::~Constraint (void)
    {
      if (this->control_action != 0)
        {
          this->control_action->remove_ref ();
        }
    }
    
    MonitorControl_Types::Constraint&
    MonitorControl_Types::Constraint::operator= (const Constraint& rhs)
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

#endif /* ACE_ENABLE_MONITORS */


