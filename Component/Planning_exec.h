// -*- C++ -*-
// $Id$

#ifndef SA_POP_COMPONENT_PLANNING_EXEC_H
#define SA_POP_COMPONENT_PLANNING_EXEC_H

#include /**/ "ace/pre.h"

#include "SA_POP_Component_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SA_POP_Component_exec_export.h"
#include "tao/LocalObject.h"

#include "common/OpStringC.h"

#include "Planner.h"
#include "LogScreenOut.h"


namespace CIAO
{
  namespace RACE
  {
    namespace SA_POP
    {
      namespace CIDL_SA_POP_Component_Impl
      {
        class SA_POP_COMPONENT_EXEC_Export Planning_exec_i
          : public virtual ::CIAO::RACE::SA_POP::CCM_Planning,
            public virtual TAO_Local_RefCounted_Object
        {
        public:
          Planning_exec_i (void);
          virtual ~Planning_exec_i (void);

          // Operations from ::CIAO::RACE::SA_POP::Planning

          virtual ::CORBA::Boolean
          init (
            const char * task_net_file,
            const char * task_map_file
          );
          virtual ::CORBA::Boolean
          generate_opstring (
            const ::CIAO::RACE::GoalStructure & goal,
            ::CIAO::RACE::OperationalString_out opstring
          );

        protected:
          /// Convert SA-POP operational string to IDL operational string.
          /**
           * @param opstring  SA-POP operational string to convert.
           *
           * @param opstring_idl  Empty IDL operational string structure
           *                      to be populated.
           */
          virtual void opstring_to_idl (const ::SA_POP::Plan &opstring,
                                        ::CIAO::RACE::OperationalString &opstring_idl);

          /// Convert IDL goal to SA-POP goal.
          /**
           * @param goal_idl  IDL goal to convert.
           *
           * @param goal  Empty SA-POP goal structure to be populated.
           */
          virtual void goal_from_idl (const ::CIAO::RACE::GoalStructure &goal_idl,
                                      ::SA_POP::Goal &goal);

        private:
          /// Flag indicating whether SA-POP planner is initialized.
          bool is_init_;

          /// Pointer to SA-POP planner.
          ::SA_POP::Planner *planner_;

          /// Screen output adapter for SA-POP.
          ::SA_POP::LogScreenOut screen_out_;

        };
      }
    }
  }
}

#endif /* SA_POP_COMPONENT_PLANNING_EXEC_H */
