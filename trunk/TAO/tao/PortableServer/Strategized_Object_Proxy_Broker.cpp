// $Id$

#include "Strategized_Object_Proxy_Broker.h"
#include "Direct_Object_Proxy_Impl.h"
#include "ThruPOA_Object_Proxy_Impl.h"

#include "tao/Remote_Object_Proxy_Impl.h"


ACE_RCSID (tao, TAO_Strategized_Object_Proxy_Broker, "$Id$")

TAO_Strategized_Object_Proxy_Broker *
the_tao_strategized_object_proxy_broker (void)
{
  static TAO_Strategized_Object_Proxy_Broker the_broker;
  return &the_broker;
}

TAO_Strategized_Object_Proxy_Broker::TAO_Strategized_Object_Proxy_Broker (void)
{
  for (int i = 0;
       i < TAO_ORB_Core::COLLOCATION_STRATEGIES_NUM;
       ++i)
    this->proxy_cache_[i] = 0;
}

TAO_Strategized_Object_Proxy_Broker::~TAO_Strategized_Object_Proxy_Broker (void)
{
  for (int i = 0;
       i < TAO_ORB_Core::COLLOCATION_STRATEGIES_NUM;
       ++i)
    delete this->proxy_cache_[i];
}

TAO_Object_Proxy_Impl &
TAO_Strategized_Object_Proxy_Broker::select_proxy (CORBA::Object_ptr object,
                                                   CORBA::Environment &ACE_TRY_ENV)
{
  TAO_ORB_Core::TAO_Collocation_Strategies strategy =
    TAO_ORB_Core::collocation_strategy (object);

  if (this->proxy_cache_[strategy] != 0)
    return *this->proxy_cache_[strategy];

  this->create_proxy (strategy, ACE_TRY_ENV);
  ACE_CHECK_RETURN (*this->proxy_cache_[strategy]);

  return *this->proxy_cache_[strategy];
}

void
TAO_Strategized_Object_Proxy_Broker::create_proxy (TAO_ORB_Core::TAO_Collocation_Strategies strategy,
                                                   CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX,
             guard,
             this->mutex_);

  if (this->proxy_cache_[strategy] == 0)
    {
      switch (strategy)
        {
        case TAO_ORB_Core::THRU_POA_STRATEGY:
          {
            ACE_NEW_THROW_EX (this->proxy_cache_[strategy],
                              TAO_ThruPOA_Object_Proxy_Impl,
                              CORBA::NO_MEMORY ());
            ACE_CHECK;
            break;
          }
        case TAO_ORB_Core::DIRECT_STRATEGY:
          {
            ACE_NEW_THROW_EX (this->proxy_cache_[strategy],
                              TAO_Direct_Object_Proxy_Impl,
                              CORBA::NO_MEMORY ());
            ACE_CHECK;
            break;
          }
        default:
        case TAO_ORB_Core::REMOTE_STRATEGY:
          {
            ACE_NEW_THROW_EX (this->proxy_cache_[strategy],
                              TAO_Remote_Object_Proxy_Impl,
                              CORBA::NO_MEMORY ());
            ACE_CHECK;
            break;
          }

        }
    }
}
