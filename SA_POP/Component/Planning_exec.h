#ifndef SA_POP_COMPONENT_PLANNING_EXEC_H
#define SA_POP_COMPONENT_PLANNING_EXEC_H

#include /**/ "ace/pre.h"

#include "SA_POP_Component_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SA_POP_Component_exec_export.h"
#include "tao/LocalObject.h"


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
          generate_opstring (
            const ::CIAO::RACE::GoalStructure & goal,
            ::CIAO::RACE::OperationalString_out opstring
          );
        };
      }
    }
  }
}

#endif /* SA_POP_COMPONENT_PLANNING_EXEC_H */
