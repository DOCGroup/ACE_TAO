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
       i < TAO_Collocation_Strategies::CS_LAST;
       ++i)
    this->proxy_cache_[i] = 0;
}

TAO_Strategized_Object_Proxy_Broker::~TAO_Strategized_Object_Proxy_Broker (void)
{
  for (int i = 0;
       i < TAO_Collocation_Strategies::CS_LAST;
       ++i)
    delete this->proxy_cache_[i];
}

TAO_Object_Proxy_Impl &
TAO_Strategized_Object_Proxy_Broker::select_proxy (CORBA::Object_ptr object
                                                   ACE_ENV_ARG_DECL)
{
  int strategy =
    TAO_ORB_Core::collocation_strategy (object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (*this->proxy_cache_[strategy]);

  if (this->proxy_cache_[strategy] != 0)
    return *this->proxy_cache_[strategy];

  this->create_proxy (strategy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (*this->proxy_cache_[strategy]);

  return *this->proxy_cache_[strategy];
}

void
TAO_Strategized_Object_Proxy_Broker::create_proxy (int strategy
                                                   ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->mutex_);

  if (this->proxy_cache_[strategy] == 0)
    {
      switch (strategy)
        {
        case TAO_Collocation_Strategies::CS_THRU_POA_STRATEGY:
          {
            ACE_NEW_THROW_EX (this->proxy_cache_[strategy],
                              TAO_ThruPOA_Object_Proxy_Impl,
                              CORBA::NO_MEMORY ());
            ACE_CHECK;
            break;
          }
        case TAO_Collocation_Strategies::CS_DIRECT_STRATEGY:
          {
            ACE_NEW_THROW_EX (this->proxy_cache_[strategy],
                              TAO_Direct_Object_Proxy_Impl,
                              CORBA::NO_MEMORY ());
            ACE_CHECK;
            break;
          }
        default:
        case TAO_Collocation_Strategies::CS_REMOTE_STRATEGY:
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

TAO_Object_Proxy_Broker * _TAO_collocation_Object_Proxy_Broker_Factory (
                                                         CORBA::Object_ptr obj
                                                         )
{
  ACE_UNUSED_ARG (obj);
  return the_tao_strategized_object_proxy_broker ();
}

int _TAO_collocation_Object_Proxy_Broker_Factory_Initializer (long dummy)
{
  ACE_UNUSED_ARG (dummy);

  _TAO_collocation_Object_Proxy_Broker_Factory_function_pointer =
    _TAO_collocation_Object_Proxy_Broker_Factory;

  return 0;
}

static int
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer_Scarecrow =
_TAO_collocation_Object_Proxy_Broker_Factory_Initializer (ACE_reinterpret_cast (long,
                                                                                _TAO_collocation_Object_Proxy_Broker_Factory_Initializer));
