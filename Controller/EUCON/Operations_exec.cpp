// $Id$
#include "Operations_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      namespace CIDL_EUCON_Controller_Impl
      {
        Control_Operations_exec_i::Control_Operations_exec_i
        (Controller *controller)
          : controller_ (controller)
        {
        }

        Control_Operations_exec_i::~Control_Operations_exec_i (void)
        {
        }

        ::CORBA::Boolean
        Control_Operations_exec_i::register_string (
          const ::CIAO::RACE::OperationalString & opstring,
          ::CORBA::String_out ID)
        {
          return this->controller_->register_string (opstring, ID);
        }

          ::CORBA::Boolean
        Control_Operations_exec_i::unregister_string (
          const char * ID)
        {
          return this->controller_->unregister_string (ID);
        }

      }
    }
  }
}
