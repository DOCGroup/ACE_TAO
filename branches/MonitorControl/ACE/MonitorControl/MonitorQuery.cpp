// $Id$

#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Base.h"
#include "ace/Control_Action.h"

#include "MonitorControl/MonitorQuery.h"
#include "MonitorControl/Constraint_Interpreter.h"
#include "MonitorControl/Constraint_Visitor.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    int
    MonitorPointAutoQuery::handle_timeout (
      const ACE_Time_Value& /* current */,
      const void* monitor_query)
    {
      const MonitorQuery* const_mq =
        reinterpret_cast<const MonitorQuery*> (monitor_query);
        
      MonitorQuery* mq = const_cast<MonitorQuery*> (const_mq);
      
      mq->query ();
      
      return 0;
    }
    
    //=========================================================
    
    MonitorQuery::MonitorQuery (const char* monitor_name)
      : monitor_ (0)
    {
      ACE_CString name_str (monitor_name, 0, false);
      this->monitor_ = Monitor_Point_Registry::instance ()->get (name_str);
      
      if (this->monitor_ == 0)
        {
          ACE_ERROR ((LM_ERROR, "MonitorQuery - monitor lookup failed\n"));
        }
    }
    
    void
    MonitorQuery::query (void)
    {
      MonitorControl_Types::Constraint* constraint = 0;
      
      if (this->monitor_ == 0)
        {
          ACE_ERROR ((LM_ERROR, "MonitorQuery::query - null monitor\n"));
          return;
        }
      
      for (Monitor_Base::CONSTRAINT_ITERATOR i (
             this->monitor_->constraints ());
           !i.done ();
           i.advance ())
        {
          i.next (constraint);
          
          Constraint_Interpreter interpreter;
          interpreter.build_tree (constraint->expr.fast_rep ());
          
          Constraint_Visitor visitor (this->monitor_->retrieve ());
          bool satisfied = interpreter.evaluate (visitor);
          
          if (satisfied && constraint->control_action != 0)
            {
              constraint->control_action->execute ();
            }
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


