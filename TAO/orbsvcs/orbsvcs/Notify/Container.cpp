// $Id$

#include "Container.h"
#include "ThreadPool_Task.h"
#include "POA_Helper.h"

#if ! defined (__ACE_INLINE__)
#include "Container.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Container, "$Id$")

TAO_NS_Container::TAO_NS_Container (void)
  :object_poa_ (0), proxy_poa_ (0), delete_object_poa_ (0), delete_proxy_poa_ (0)
{
}

TAO_NS_Container::~TAO_NS_Container ()
{
}

void
TAO_NS_Container::init (TAO_NS_POA_Helper* poa, TAO_NS_Worker_Task* worker_task, TAO_NS_POA_Helper* object_poa, TAO_NS_POA_Helper* proxy_poa ACE_ENV_ARG_DECL)
{
  TAO_NS_Object::init (poa, worker_task ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  object_poa_ = object_poa;
  proxy_poa_ = proxy_poa;
}

void
TAO_NS_Container::cleanup (ACE_ENV_SINGLE_ARG_DECL)
{
  if (delete_object_poa_ == 1)
    {
      object_poa_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      delete object_poa_; 
    }

  if (delete_proxy_poa_ == 1)
    {
      proxy_poa_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      delete proxy_poa_;
    }
}

void
TAO_NS_Container::object_poa (TAO_NS_POA_Helper* object_poa)
{
  object_poa_ = object_poa;
  delete_object_poa_ = 1;
}

TAO_NS_POA_Helper*
TAO_NS_Container::object_poa (void)
{
  return this->object_poa_;
}

void
TAO_NS_Container::proxy_poa (TAO_NS_POA_Helper* proxy_poa)
{
  proxy_poa_ = proxy_poa;
  delete_proxy_poa_ = 1;
}

TAO_NS_POA_Helper*
TAO_NS_Container::proxy_poa (void)
{
  return this->proxy_poa_;
}



