// $Id$

#ifndef TAO_Notify_CONTAINER_T_CPP
#define TAO_Notify_CONTAINER_T_CPP

#include "orbsvcs/Notify/Container_T.h"

#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/Notify/POA_Helper.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Container_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::Destroyer::work (TYPE* type)
{
  type->destroy ();
}

template<class TYPE>
TAO_Notify_Container_T<TYPE>::TAO_Notify_Container_T (void)
  : collection_ (0)
{
}

template<class TYPE>
TAO_Notify_Container_T<TYPE>::~TAO_Notify_Container_T ()
{
  delete collection_;
}

template <class TYPE> void
TAO_Notify_Container_T<TYPE>::shutdown (void)
{
  TAO_ESF_Shutdown_Proxy<TYPE> shutdown_worker;

  this->collection_->for_each (&shutdown_worker);
}

template <class TYPE> void
TAO_Notify_Container_T<TYPE>::destroy (void)
{
  Destroyer destroyer;
  this->collection_->for_each (&destroyer);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::insert (TYPE* type)
{
  this->collection_->connected (type);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::remove (TYPE* type)
{
  this->collection_->disconnected (type);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::init (void)
{
  // get the factory
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  // Init variables
  factory->create (this->collection_);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_CONTAINER_T_CPP */
