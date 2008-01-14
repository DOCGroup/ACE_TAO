// $Id$
#include "ApplicationActuator_exec.h"
#include "ciao/CIAO_common.h"
#include <sstream>

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        ApplicationActuator_exec_i::ApplicationActuator_exec_i
        (EUCON_Effector_Context *context,
         Logger &logger)
          : context_ (context),
            logger_ (logger)

        {
        }

        ApplicationActuator_exec_i::~ApplicationActuator_exec_i (void)
        {
        }


        ::CORBA::Boolean
        ApplicationActuator_exec_i::modifyApplications (
          const ::CIAO::RACE::OperationalStrings & /* opstrings */)
        {
          std::stringstream msg ;
          msg << "Entering ApplicationActuator_exec_i::modifyApplications ().\n";
          msg << "Exiting ApplicationActuator_exec_i::modifyApplications ().\n"; 
          this->logger_.log (msg.str());
          // Your code here.
          return false;
        }
      }
    }
  }
}
