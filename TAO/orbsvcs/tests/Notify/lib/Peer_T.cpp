// $Id$
#ifndef TAO_NS_Peer_T_CPP
#define TAO_NS_Peer_T_CPP

#include "Peer_T.h"

#if ! defined (__ACE_INLINE__)
#include "Peer_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Peer_T, "$id$")

#include "ace/Arg_Shifter.h"
#include "Name.h"
#include "LookupManager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

template <class Peer_Traits>
TAO_NS_Peer_T<Peer_Traits>::TAO_NS_Peer_T (void)
  : proxy_id_ (0)
{
}

template <class Peer_Traits>
TAO_NS_Peer_T<Peer_Traits>::~TAO_NS_Peer_T ()
{
}

template <class Peer_Traits> TAO_NS_Peer_T<Peer_Traits>::Proxy_Traits_PTR
TAO_NS_Peer_T<Peer_Traits>::get_proxy (void)
{
  return proxy_.in ();
}

template <class Peer_Traits> TAO_NS_Peer_T<Peer_Traits>::Peer_Traits_PTR
TAO_NS_Peer_T<Peer_Traits>::activate (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::connect (Proxy_Traits_PTR proxy, Proxy_Traits_ID proxy_id ACE_ENV_ARG_DECL)
{
  PortableServer::ServantBase_var servant_var (this);

  ACE_TYPENAME Peer_Traits::VAR peer_var = this->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->connect_to_peer (proxy, peer_var.in () ACE_ENV_ARG_PARAMETER);

  // save the proxy
  this->proxy_ = Proxy_Traits_INTERFACE::_duplicate (proxy);
  this->proxy_id_ = proxy_id;

  // Register the proxy.
  if (this->proxy_name_.length () != 0)
    LOOKUP_MANAGER->_register (this->proxy_.in ()
                               , this->proxy_name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::connect (Admin_Traits_PTR admin_ptr ACE_ENV_ARG_DECL)
{
  ACE_TYPENAME Proxy_Traits::VAR proxy_var = this->obtain_proxy (admin_ptr ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_var.in ()));

  this->connect (proxy_var.in ()
                 , this->proxy_id_  ACE_ENV_ARG_PARAMETER);
}


template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa, this->poa_name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // set the POA
  this->set_poa (poa.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Resolve the admin
  ACE_TYPENAME Admin_Traits::VAR admin_var;

  LOOKUP_MANAGER->resolve (admin_var, this->admin_name_.c_str () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TYPENAME Admin_Ext_Traits::VAR admin_ext_var =
    Admin_Ext_Traits_INTERFACE::_narrow (admin_var.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TYPENAME Proxy_Traits::VAR proxy_var = this->obtain_proxy (admin_ext_var.in ()
                                                                 , this->qos_
                                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_var.in ()));

  // connect supplier to proxy,
  // also activates the servant as CORBA object in the POA specified.
  this->connect (proxy_var.in (), this->proxy_id_  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::set_qos (CosNotification::QoSProperties& qos ACE_ENV_ARG_DECL)
{
  this->get_proxy ()->set_qos (qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::status (ACE_ENV_SINGLE_ARG_DECL)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_TRY
    {
      CORBA::Boolean not_exist = this->get_proxy ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (not_exist == 1)
        {
          ACE_DEBUG ((LM_DEBUG, "Peer %s, Proxy does not exist\n",this->name_.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "Peer %s, Proxy exists\n",this->name_.c_str ()));
        }
    }
  ACE_CATCH(CORBA::TRANSIENT, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "");
      ACE_DEBUG ((LM_DEBUG, "Peer %s is_equivalent transient exception.", this->name_.c_str ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "");
      ACE_DEBUG ((LM_DEBUG, "Peer %s is_equivanent other exception.", this->name_.c_str ()));
    }
  ACE_ENDTRY;
#else
  return;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  this->disconnect_from_proxy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class Peer_Traits> PortableServer::POA_ptr
TAO_NS_Peer_T<Peer_Traits>::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

template <class Peer_Traits> void
TAO_NS_Peer_T<Peer_Traits>::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#endif /* TAO_NS_Peer_T_CPP */
