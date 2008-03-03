// $Id$

#include "ace/OS_NS_sys_time.h"
#include "ace/Guard_T.h"
#include "ace/Control_Action.h"

#include "MonitorControl/MonitorPoint.h"
#include "MonitorControl/Constraint_Interpreter.h"
#include "MonitorControl/Constraint_Visitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    MonitorPoint<true>::MonitorPoint (const char* name)
      : Monitor_Base (name),
        control_action_ (0)
    {}
    
    void
    MonitorPoint<true>::constraint (const char* constraint)
    {
      if (this->interpreter_.build_tree (constraint) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "MonitorPoint<true>::constraint() - "
                      "Constraint_Interpreter::build_tree() failed\n"));
        }
    }
    
    void
    MonitorPoint<true>::control_action (Control_Action* action,
                                        const char* command)
    {
      this->control_action_ = action;
      
      if (command != 0)
        {
          this->command_ = command;
        }
    }
    
    void
    MonitorPoint<true>::receive (double data)
    {
      ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);
      this->data_.timestamp_ = ACE_OS::gettimeofday ();
      this->data_.value_ = data;
      
      Constraint_Visitor visitor (this->data_);
      
      if (this->interpreter_.evaluate (visitor))
        {
          if (this->control_action_ != 0)
            {
              this->control_action_->execute (this->command_.c_str ());
            }
        }
    }
    
    void
    MonitorPoint<true>::receive (size_t /* data */)
    {
    }
    
    void
    MonitorPoint<true>::clear (void)
    {
      // TODO
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

