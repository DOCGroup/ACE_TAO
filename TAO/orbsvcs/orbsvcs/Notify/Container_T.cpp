// $Id$

#ifndef TAO_NS_CONTAINER_T_CPP
#define TAO_NS_CONTAINER_T_CPP

#include "Container_T.h"

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"
#include "Properties.h"
#include "Factory.h"
#include "POA_Helper.h"

#if ! defined (__ACE_INLINE__)
#include "Container_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Container_T, "$Id$")

/*************************************************************************************************/

template<class OBJECT>
TAO_NS_Object_Find_Worker_T<OBJECT>::TAO_NS_Object_Find_Worker_T (CORBA::Long id)
  :id_ (id), result_ (0)
{
}

/*************************************************************************************************/

template<class TYPE, class OBJECT, class PARENT>
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::TAO_NS_Container_T (void)
  : collection_ (0)
{
}

template<class TYPE, class OBJECT, class PARENT>
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::~TAO_NS_Container_T ()
{
  ///
  delete collection_;
}

template <class TYPE, class OBJECT, class PARENT> void
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Object_T<OBJECT, PARENT>::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ESF_Shutdown_Proxy<TYPE> shutdown_worker;

  this->collection_->for_each (&shutdown_worker ACE_ENV_ARG_PARAMETER);

  /// shutdown Container
  this->cleanup (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template<class TYPE, class OBJECT, class PARENT> void
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::insert (TYPE* type ACE_ENV_ARG_DECL)
{
  this->collection_->connected (type ACE_ENV_ARG_PARAMETER);
}

template<class TYPE, class OBJECT, class PARENT> void
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::remove (TYPE* type ACE_ENV_ARG_DECL)
{
  this->collection_->disconnected (type ACE_ENV_ARG_PARAMETER);
}

template<class TYPE, class OBJECT, class PARENT> void
TAO_NS_Container_T<TYPE, OBJECT, PARENT>::init_collection (ACE_ENV_SINGLE_ARG_DECL)
{
  // get the factory
  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();

  // Init variables
  factory->create (this->collection_ ACE_ENV_ARG_PARAMETER);
}
#endif /* TAO_NS_CONTAINER_T_CPP */
