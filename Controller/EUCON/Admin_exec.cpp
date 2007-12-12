// $Id$
#include "Admin_exec.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      namespace CIDL_EUCON_Controller_Impl
      {
        Admin_exec_i::Admin_exec_i (Controller *controller)
          : controller_ (controller)
        {
        }

        Admin_exec_i::~Admin_exec_i (void)
        {
        }

         ::CORBA::Boolean
        Admin_exec_i::init_controller ()
        {
          return false;
          //return this->controller_->init_controller ();

        }

        ::CORBA::Boolean
        Admin_exec_i::start_controller ()
        {
          return false;
          //return this->controller_->start_controller ();
        }

        ::CORBA::Boolean
        Admin_exec_i::stop_controller ()
        {
          return false;
          //return this->controller_->stop_controller ();
        }
      }
    }
  }
}
