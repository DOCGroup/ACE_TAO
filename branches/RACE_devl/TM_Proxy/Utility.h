#ifndef UTILITY_H
#define UTILITY_H

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
        class TM_PROXY_EXEC_Export Utility_exec_i
          : public virtual ::CIAO::RACE::TM_Proxy::CCM_Utility,
            public virtual TAO_Local_RefCounted_Object
        {
          public:
          Utility_exec_i (void);

          virtual ~Utility_exec_i (void);

        };
      }
    }
  }
}

#include /**/ "ace/post.h"

#endif /* UTILITY_H */
