// $Id$

#include "Proxy.h"

ACE_INLINE void
TAO_NS_Subscription_Change_Worker::work (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL)
{
  proxy->admin_subscription (this->added_, this->removed_ ACE_ENV_ARG_PARAMETER);
}
