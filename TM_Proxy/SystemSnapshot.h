// $Id:$
#ifndef SYSTEMSNAPSHOT_H
#define SYSTEMSNAPSHOT_H

#include /**/ "ace/pre.h"

#include "TM_ProxyEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TM_Proxy_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace TM_Proxy
    {
      namespace CIDL_TM_Proxy_Component_Impl
      {
        class TM_PROXY_EXEC_Export SystemSnapshot_exec_i
          : public virtual ::CIAO::RACE::TM_Proxy::CCM_SystemSnapshot,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          SystemSnapshot_exec_i (void);

          virtual ~SystemSnapshot_exec_i (void);

        };
      }
    }
  }
}
#include /**/ "ace/post.h"

#endif /* SYSTEMSNAPSHOT_H */
