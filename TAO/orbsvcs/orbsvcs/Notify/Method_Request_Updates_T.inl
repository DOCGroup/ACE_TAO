// $Id$

#include "Peer.h"

template <class SEQ, class PROXY, class SEQ_PARAM, class PROXY_PARAM> ACE_INLINE int
TAO_Notify_Method_Request_Updates_T<SEQ, PROXY, SEQ_PARAM, PROXY_PARAM>::execute_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  ACE_TRY
    {
      TAO_Notify_Peer* peer = this->proxy_->peer();

      if (peer != 0)
        {
          peer->dispatch_updates (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_Notify_Method_Request_Updates::execute error sending updates\n ");
    }
  ACE_ENDTRY;

  return 0;
}
