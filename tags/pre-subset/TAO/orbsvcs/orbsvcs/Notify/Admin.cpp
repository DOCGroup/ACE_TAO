// $Id$
#include "Admin.h"

#if ! defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Admin, "$Id$")

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "Container_T.h"
#include "Proxy.h"
#include "EventChannel.h"

TAO_NS_Admin::TAO_NS_Admin (void)
  : ec_ (0)
  , proxy_container_ (0)
  , filter_operator_ (CosNotifyChannelAdmin::OR_OP)
{
  // Initialize all Admin objects to initially be subscribed for all events.
  // This is a reasonable default and is required to allow Cos Event consumers/suppliers to send/receive events,
  this->subscribed_types_.insert (TAO_NS_EventType::special ());
}

TAO_NS_Admin::~TAO_NS_Admin ()
{
  this->ec_->_decr_refcnt ();
}

void
TAO_NS_Admin::init (TAO_NS_EventChannel *ec ACE_ENV_ARG_DECL)
{
  this->ec_ = ec;

  this->ec_->_incr_refcnt ();

  this->TAO_NS_Object::init (ec);

  ACE_NEW_THROW_EX (this->proxy_container_,
                    TAO_NS_Proxy_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->proxy_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_NS_Admin::remove (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL)
{
  this->proxy_container_->remove (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Admin::subscribed_types (TAO_NS_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // copy
  subscribed_types = this->subscribed_types_;
}

int
TAO_NS_Admin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  this->proxy_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  delete this->proxy_container_;

  return 0;
}

void
TAO_NS_Admin::insert (TAO_NS_Proxy* proxy ACE_ENV_ARG_DECL)
{
  this->proxy_container_->insert (proxy ACE_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Shutdown_Proxy<TAO_NS_Proxy>;
template class TAO_NS_Container_T <TAO_NS_Proxy>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma TAO_ESF_Shutdown_Proxy<TAO_NS_Proxy>
#pragma instantiate TAO_NS_Container_T <TAO_NS_Proxy>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
