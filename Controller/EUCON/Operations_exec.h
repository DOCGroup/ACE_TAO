// $Id$

#ifndef CIAO_EUCON_OPERATIONS_EXEC_H
#define CIAO_EUCON_OPERATIONS_EXEC_H

#include /**/ "ace/pre.h"

#include "EUCON_ControllerEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EUCON_Controller_exec_export.h"
#include "tao/LocalObject.h"
#include "Controller.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Controller
    {
      namespace CIDL_EUCON_Controller_Impl
      {
        class EUCON_CONTROLLER_EXEC_Export Control_Operations_exec_i
          : public virtual ::CIAO::RACE::Controller::CCM_Control_Operations,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Control_Operations_exec_i (Controller *controller);

          virtual ~Control_Operations_exec_i (void);

          // Operations from ::CIAO::RACE::Controller::Control_Operations

          virtual ::CORBA::Boolean
            register_string (
              const ::CIAO::RACE::OperationalString & opstring,
              ::CORBA::String_out ID);

          virtual ::CORBA::Boolean
            unregister_string (
              const char * ID);

          private:
          Controller *controller_;
        };

      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_EUCON_OPERATIONS_EXEC_H */
