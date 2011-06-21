// $Id$

#ifndef TAO_Notify_Tests_Peer_T_CPP
#define TAO_Notify_Tests_Peer_T_CPP

#include "Peer_T.h"

#include "ace/Arg_Shifter.h"
#include "Name.h"
#include "LookupManager.h"
#include "Options_Parser.h"
#include "orbsvcs/NotifyExtC.h"

template <class Peer_Traits>
TAO_Notify_Tests_Peer_T<Peer_Traits>::TAO_Notify_Tests_Peer_T (void)
  : proxy_id_ (0)
{
}

template <class Peer_Traits>
TAO_Notify_Tests_Peer_T<Peer_Traits>::~TAO_Notify_Tests_Peer_T ()
{
}

template <class Peer_Traits>
typename TAO_Notify_Tests_Peer_T<Peer_Traits>::Proxy_Traits_PTR
TAO_Notify_Tests_Peer_T<Peer_Traits>::get_proxy (void)
{
  return proxy_.in ();
}

template <class Peer_Traits>
typename TAO_Notify_Tests_Peer_T<Peer_Traits>::Peer_Traits_PTR
TAO_Notify_Tests_Peer_T<Peer_Traits>::activate (void)
{
  return this->_this ();
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::connect (Proxy_Traits_PTR proxy,
                                               Proxy_Traits_ID proxy_id)
{
  // This will decr the ref count on exit.
  // Clients of this class should use raw pointers, not vars.
  PortableServer::ServantBase_var servant_var (this);

  typename Peer_Traits::VAR peer_var =
    this->activate ();

  this->connect_to_peer (proxy, peer_var.in ());

  // save the proxy
  this->proxy_ = Proxy_Traits_INTERFACE::_duplicate (proxy);
  this->proxy_id_ = proxy_id;

  // Register the proxy.
  if (this->proxy_name_.length () != 0)
    {
      LOOKUP_MANAGER->_register (this->proxy_.in (),
                                 this->proxy_name_.c_str ());
    }
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::connect (Admin_Traits_PTR admin_ptr)
{
  typename Proxy_Traits::VAR proxy_var =
    this->obtain_proxy (admin_ptr);

  ACE_ASSERT (!CORBA::is_nil (proxy_var.in ()));

  this->connect (proxy_var.in (),
                 this->proxy_id_);
}


template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::connect (void)
{
  // Get the POA
  PortableServer::POA_var poa;
  LOOKUP_MANAGER->resolve (poa,
                           this->poa_name_.c_str ());

  // set the POA
  this->set_poa (poa.in ());

  // Resolve the admin
  typename Admin_Traits::VAR admin_var;

  LOOKUP_MANAGER->resolve (admin_var,
                           this->admin_name_.c_str ());

  typename Admin_Ext_Traits::VAR admin_ext_var =
    Admin_Ext_Traits_INTERFACE::_narrow (admin_var.in ());

  typename Proxy_Traits::VAR proxy_var =
    this->obtain_proxy (admin_ext_var.in (),
                        this->qos_);

  ACE_ASSERT (!CORBA::is_nil (proxy_var.in ()));

  // connect supplier to proxy,
  // also activates the servant as CORBA object in the POA specified.
  this->connect (proxy_var.in (),
                 this->proxy_id_);
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::set_qos (
    CosNotification::QoSProperties& qos
  )
{
  this->get_proxy ()->set_qos (qos);
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::status (void)
{
#if (TAO_HAS_MINIMUM_CORBA == 0)
  try
    {
      CORBA::Boolean not_exist =
        this->get_proxy ()->_non_existent ();

      if (not_exist == 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Peer %s, Proxy does not exist\n",
                      this->name_.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Peer %s, Proxy exists\n",
                      this->name_.c_str ()));
        }
    }
  catch (const CORBA::TRANSIENT& ex)
    {
      ex._tao_print_exception ("Error: ");
      ACE_DEBUG ((LM_DEBUG,
                  "Peer %s is_equivalent transient exception.",
                  this->name_.c_str ()));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error: ");
      ACE_DEBUG ((LM_DEBUG,
                  "Peer %s is_equivanent other exception.",
                  this->name_.c_str ()));
    }
#else
;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::disconnect (void)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_.in ()));

  try
    {
      this->disconnect_from_proxy ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Peer %s failed to disconnect from proxy.",
                  this->name_.c_str ()));
    }

  try
    {
      this->deactivate ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Peer %s failed to deactivate.",
                  this->name_.c_str ()));
    }
}

template <class Peer_Traits>
PortableServer::POA_ptr
TAO_Notify_Tests_Peer_T<Peer_Traits>::_default_POA (
  )
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

template <class Peer_Traits>
void
TAO_Notify_Tests_Peer_T<Peer_Traits>::deactivate (void)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

#endif /* TAO_Notify_Tests_Peer_T_CPP */
