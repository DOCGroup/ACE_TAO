//$Id$
#include "AbstractBase_Invocation_Adapter.h"
#include "AbstractBase.h"

ACE_RCSID (Valuetype,
           AbstractBase_Invocation_Adapter,
           "$Id$")

namespace TAO
{
  AbstractBase_Invocation_Adapter::AbstractBase_Invocation_Adapter (
      CORBA::AbstractBase_ptr target,
      Argument **args,
      int arg_number,
      char *operation,
      int op_len,
      Collocation_Proxy_Broker *p,
      Invocation_Type type,
      Invocation_Mode mode)
    : Invocation_Adapter (target->equivalent_objref (),
                          args,
                          arg_number,
                          operation,
                          op_len,
                          p,
                          type,
                          mode)
  {
  }
} // End namespace TAO
