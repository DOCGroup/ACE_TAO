#include "Collocated_Invocation.h"
#include "Collocation_Proxy_Broker.h"
#include "operation_details.h"

ACE_RCSID (tao,
           Collocated_Invocation,
           "$Id$")

namespace TAO
{

  Collocated_Invocation::Collocated_Invocation (CORBA::Object *p,
                                                TAO_Operation_Details &detail)
    : Invocation_Base (p,
                       detail)
  {
  }

  Invocation_Status
  Collocated_Invocation::invoke (Collocation_Proxy_Broker *cpb,
                                 CORBA::Object *obj,
                                 Collocation_Strategy strat
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    // @@ Time to call interceptors...
    cpb->dispatch (obj,
                   this->forwarded_to_.out (),
                   this->details_.args (),
                   this->details_.args_num (),
                   this->details_.opname (),
                   this->details_.opname_len (),
                   strat
                   ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (this->forwarded_to_.in () != 0)
      return TAO_INVOKE_RESTART;

    return TAO_INVOKE_SUCCESS;
  }
}
