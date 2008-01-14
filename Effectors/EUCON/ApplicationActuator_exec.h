// $Id$
#ifndef CIAO_APPLICATIONACTUATOR_EXEC_H
#define CIAO_APPLICATIONACTUATOR_EXEC_H

#include /**/ "ace/pre.h"

#include "EUCON_Effector_svnt.h"
#include "Logger.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EUCON_Effector_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace Effector
    {
      namespace CIDL_EUCON_Effector_Impl
      {
        class EUCON_EFFECTOR_EXEC_Export ApplicationActuator_exec_i
          : public virtual ::CIAO::RACE::Effector::CCM_ApplicationActuator,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          ApplicationActuator_exec_i
            (EUCON_Effector_Context *context,
             Logger &logger);

          virtual ~ApplicationActuator_exec_i (void);

          // Operations from ::CIAO::RACE::Effector::ApplicationActuator

          virtual ::CORBA::Boolean
          modifyApplications (
            const ::CIAO::RACE::OperationalStrings & opstrings);

          private:

          Logger &logger_;

          EUCON_Effector_Context *context_;
        };

      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_APPLICATIONACTUATOR_EXEC_H */
