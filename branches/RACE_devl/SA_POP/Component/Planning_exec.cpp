#include "Planning_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace SA_POP
    {
      namespace CIDL_SA_POP_Component_Impl
      {
        //==================================================================
        // Facet Executor Implementation Class:   Planning_exec_i
        //==================================================================

        Planning_exec_i::Planning_exec_i (void)
        {
        }

        Planning_exec_i::~Planning_exec_i (void)
        {
        }

        // Operations from ::CIAO::RACE::SA_POP::Planning

        ::CORBA::Boolean
        Planning_exec_i::generate_opstring (
          const ::CIAO::RACE::GoalStructure & /* goal */,
          ::CIAO::RACE::OperationalString_out /* opstring */
        )
        {
          // Your code here.
          return false;
        }
      }
    }
  }
}

