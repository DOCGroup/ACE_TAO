#include "EC_Gateway.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Gateway::TAO_EC_Gateway ()
  :  handle_ (0)
{
}

TAO_EC_Gateway::~TAO_EC_Gateway ()
{
}

void
TAO_EC_Gateway::observer_handle (RtecEventChannelAdmin::Observer_Handle h)
{
  this->handle_ = h;
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Gateway::observer_handle () const
{
  return this->handle_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
