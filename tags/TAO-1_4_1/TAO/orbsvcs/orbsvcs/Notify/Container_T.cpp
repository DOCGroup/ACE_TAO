// $Id$

#ifndef TAO_Notify_CONTAINER_T_CPP
#define TAO_Notify_CONTAINER_T_CPP

#include "Container_T.h"

#include "Properties.h"
#include "Factory.h"
#include "POA_Helper.h"

#include "orbsvcs/orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/orbsvcs/ESF/ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "Container_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, 
           Notify_Container_T, 
           "$Id$")

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
TAO_Notify_Container_T<TYPE>::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_ESF_Shutdown_Proxy<TYPE> shutdown_worker;

  this->collection_->for_each (&shutdown_worker ACE_ENV_ARG_PARAMETER);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::insert (TYPE* type ACE_ENV_ARG_DECL)
{
  this->collection_->connected (type ACE_ENV_ARG_PARAMETER);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::remove (TYPE* type ACE_ENV_ARG_DECL)
{
  this->collection_->disconnected (type ACE_ENV_ARG_PARAMETER);
}

template<class TYPE> void
TAO_Notify_Container_T<TYPE>::init (ACE_ENV_SINGLE_ARG_DECL)
{
  // get the factory
  TAO_Notify_Factory* factory = TAO_Notify_PROPERTIES::instance ()->factory ();

  // Init variables
  factory->create (this->collection_ ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_Notify_CONTAINER_T_CPP */
