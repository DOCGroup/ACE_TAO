//
// $Id$
//

#include "Stock_i.h"

Quoter_Stock_i::Quoter_Stock_i (const char *symbol,
                                const char *full_name,
                                CORBA::Double price)
  :  supplier_personality_ (this)
{
  this->data_.symbol = symbol;
  this->data_.full_name = full_name;
  this->data_.price = price;
}

char *
Quoter_Stock_i::symbol () throw (CORBA::SystemException)
{
  return CORBA::string_dup (this->data_.symbol.in ());
}

char *
Quoter_Stock_i::full_name () throw (CORBA::SystemException)
{
  return CORBA::string_dup (this->data_.full_name.in ());
}

CORBA::Double
Quoter_Stock_i::price () throw (CORBA::SystemException)
{
  return this->data_.price;
}

void
Quoter_Stock_i::set_price (CORBA::Double new_price)
  throw (CORBA::SystemException)
{
  this->data_.price = new_price;
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  CORBA::Any event;
  event <<= this->data_;
  this->consumer_proxy_->push (event);
}

void
Quoter_Stock_i::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  throw (CORBA::SystemException)
{
  // Forget about the consumer it is not there anymore
  this->consumer_proxy_ =
    CosEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
Quoter_Stock_i::connect (CosEventChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer ();
  CosEventComm::PushSupplier_var supplier =
    this->supplier_personality_._this ();
  this->consumer_proxy_->connect_push_supplier (supplier.in ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class POA_CosEventComm::PushSupplier_tie<Quoter_Stock_i>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate POA_CosEventComm::PushSupplier_tie<Quoter_Stock_i>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
