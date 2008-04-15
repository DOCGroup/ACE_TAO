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
      if (this->monitor_ == 0)
        {
          ACE_ERROR ((LM_ERROR, "MonitorQuery::query - null monitor\n"));
          return;
        }
        
      Monitor_Base::CONSTRAINTS& list = this->monitor_->constraints ();

      for (Monitor_Base::CONSTRAINT_ITERATOR i (list.begin ());
           i != list.end ();
           ++i)
        {
          Constraint_Interpreter interpreter;
          interpreter.build_tree (i->second.expr.fast_rep ());

          MonitorControl_Types::Data data;
          this->monitor_->retrieve (data);
          Constraint_Visitor visitor (data);
          bool satisfied = interpreter.evaluate (visitor);

          if (satisfied && i->second.control_action != 0)
            {
              i->second.control_action->execute ();
            }
        }
    }
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL


