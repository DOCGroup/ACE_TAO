// $Id$

#ifndef CIAO_EUCON_ADMIN_EXEC_H
#define CIAO_EUCON_ADMIN_EXEC_H

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
        class EUCON_CONTROLLER_EXEC_Export Admin_exec_i
          : public virtual ::CIAO::RACE::Controller::CCM_Admin,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Admin_exec_i (Controller *controller);

          virtual ~Admin_exec_i (void);

          // Operations from ::CIAO::RACE::Controller::Admin

          virtual ::CORBA::Boolean
            init_controller ();

          virtual ::CORBA::Boolean
            start_controller ();

          virtual ::CORBA::Boolean
            stop_controller ();

          private:

          Controller *controller_;
        };

      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_EUCON_ADMIN_EXEC_H */
