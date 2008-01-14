// $Id$
#ifndef CIAO_SYSTEMACTUATOR_EXEC_H
#define CIAO_SYSTEMACTUATOR_EXEC_H

#include /**/ "ace/pre.h"

#include "EUCON_EffectorEC.h"

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
        class EUCON_EFFECTOR_EXEC_Export SystemActuator_exec_i
          : public virtual ::CIAO::RACE::Effector::CCM_SystemActuator,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          SystemActuator_exec_i (void);

          virtual ~SystemActuator_exec_i (void);

          // Operations from ::CIAO::RACE::Effector::SystemActuator
        };
      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_SYSTEMACTUATOR_EXEC_H */
