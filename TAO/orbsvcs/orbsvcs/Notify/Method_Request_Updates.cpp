// $Id$

#include "Method_Request_Updates.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Updates.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Updates, "$Id$")

#include "tao/debug.h"
#include "Proxy.h"
#include "Peer.h"

TAO_NS_Method_Request_Updates::TAO_NS_Method_Request_Updates (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy)
  :added_ (added), removed_ (removed), proxy_ (proxy), refcountable_guard_ (*proxy)
{
}

TAO_NS_Method_Request_Updates::~TAO_NS_Method_Request_Updates ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Updates::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Updates (this->added_, this->removed_, this->proxy_);
}

int
TAO_NS_Method_Request_Updates::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->proxy_->has_shutdown ())
    return 0; // If we were shutdown while waiting in the queue, return with no action.

  ACE_TRY
    {
      TAO_NS_Peer* peer = this->proxy_->peer();

      if (peer != 0)
        {
          peer->dispatch_updates (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 0)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO_NS_Method_Request_Updates::execute error sending updates\n ");
    }
  ACE_ENDTRY;

  return 0;
}
